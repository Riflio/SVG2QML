#include "svgparser.h"
#include <QRegularExpression>
#include <QDebug>

SVGParser::SVGParser(QObject *parent) : QObject(parent)
{
    _rootItem = nullptr;
}

CPrimitive * SVGParser::rootItem() const
{
    return _rootItem;
}

CDefs SVGParser::defs() const
{
    return _defs;
}

/**
* @brief Разбираем переданный файл
* @param file
* @return
*/
SVGParser::ParseStatus SVGParser::parse(QIODevice * device)
{
    if ( !device->isOpen() && !device->open(QIODevice::ReadOnly|QIODevice::Text) ) { qWarning()<<"Device not opened!"; return PS_NOFILE; }

    _cssParser = new CSS::CssParser(this); //-- Styles parser

    _rootItem = new CDoc();

    CNodeInterface * currentLevel = _rootItem;
    CNodeInterface * prevLevel = nullptr; //-- Запоминаем предыдущий

    CPrimitive * curItem = nullptr; //-- Текущим разобранный итем

    bool inDefs = false; //-- Мы в секции defs
    int defsLvl = 0; //-- Подсчитываем на каком уровне в defs, что бы знать прямых наследников
    QStringList fPrimitiveTokens = {"g", "path", "line", "rect", "circle", "ellipse"}; //-- Что из примитивов заносим в FPrimitive, если они в секции defs
    QStringList toDefsForse = { "lineargradient", "radialgradient", "clippath", "style" }; //-- Что принудительно заносим в defs, да же если они вне этой секции

    _xml = new QXmlStreamReader(device);

    while ( !_xml->atEnd() ) {
        if ( _xml->hasError() ) { device->close(); return PS_SYNTAXERROR; }

        _xml->readNext();
        if ( _xml->tokenType()==QXmlStreamReader::StartDocument ) continue;

        //-- Detect start elements
        if ( _xml->tokenType()==QXmlStreamReader::StartElement ) {

            QString elName = _xml->name().toString().toLower();

            //-- Что принудительно заносить в defs
            if ( (defsLvl==0) && (toDefsForse.indexOf(elName)>-1) ) {
                inDefs = false; //-- Мы будем не в реальной секции, что бы как только кончится итем сразу выйти
                defsLvl = 1;
            }

            if  ( defsLvl>0 ) {
                if ( defsLvl==1 ) { //-- Каждого прямого наследника из defs считаем как отдельный
                    prevLevel = currentLevel;
                    if ( fPrimitiveTokens.indexOf(elName)>-1 ) {
                        FPrimitive * fPrim = new FPrimitive();
                        currentLevel = fPrim->primitive;
                    }
                }
                defsLvl++;
            }

            if ( elName=="svg" ) { //-- svg сам по себе как группа, парсим
                curItem = parseSVG(&currentLevel, _xml);
            } else
            if ( elName=="g" ) {
                curItem = parseGroup(&currentLevel, _xml);
            } else
            if ( elName=="path" ) {
                curItem = parsePath(currentLevel, _xml);
            } else
            if ( elName=="rect" ) {
                curItem = parseRect(currentLevel, _xml);
            } else
            if ( elName=="line" ) {
                curItem = parseLine(currentLevel, _xml);
            } else
            if ( elName=="image" ) {
                curItem = parseImage(currentLevel, _xml);
            } else
            if ( elName=="style" ) {
                parseCss(currentLevel, _xml);
            } else
            if ( elName=="defs" ) {
                inDefs = true;
                defsLvl = 1;
            } else
            if ( elName=="title" ) {
                if ( curItem!=nullptr ) { curItem->setTitle(_xml->readElementText()); }
            } else
            if ( elName=="desc" ) {
                if ( curItem!=nullptr ) { curItem->setDescr(_xml->readElementText()); }
            } else
            if ( elName=="circle" ) {
                curItem = parseCircle(currentLevel, _xml);
            } else
            if ( elName=="ellipse" ) {
                curItem = parseEllipse(currentLevel, _xml);
            } else
            if ( elName=="lineargradient" ) {
                curItem = parseLinearGradient(&currentLevel, _xml);
            } else
            if ( elName=="radialgradient" ) {
                curItem = parseRadialGradient(&currentLevel, _xml);
            } else
            if ( elName=="polyline" ) {
                curItem = parsePolyline(currentLevel, _xml);
            } else
            if ( elName=="polygon" ) {
                curItem = parsePolygon(currentLevel, _xml);
            } else
            if ( elName=="clippath" ) {
                curItem = parseClipPath(&currentLevel, _xml);
            }
            else {
                qWarning()<<"Unsupported element"<<_xml->name();
            }

            //if ( curItem->ID() )

        }

        //-- Detect ends elements
        if ( _xml->tokenType() == QXmlStreamReader::EndElement ) {

            QString elName = _xml->name().toString().toLower();

            if ( defsLvl>0 ) {
                defsLvl--;
                if ( defsLvl==1 ) { //-- Когда закончили со вложенными, возвращаем прежний уровень
                    currentLevel = prevLevel;
                    if ( !inDefs ) { //-- Если нас не было в реальной секции, то сразу и завершаем
                        defsLvl = 0;
                        prevLevel = nullptr;
                    }
                }
            }

            if ( elName=="svg" ) {
                break;
            } else
            if ( elName=="g" ) {
                currentLevel = static_cast<CPrimitive*>(CNodeInterface::levelUp(currentLevel));
            } else
            if ( elName=="defs" ) {
                inDefs = false;
                defsLvl = 0;
                prevLevel = nullptr;
            }
        }
    }

    //-- Переходим на самый-самый верх т.к. _rootItem будет в конце указывать на первый элемент
    if ( _rootItem->up!=nullptr ) { _rootItem = static_cast<CDoc*>(_rootItem->up); }

    if ( _dependsCDef.count()>0 ) {
        qWarning()<<"After parsing left unresolved hrefs: "<<_dependsCDef.keys();
    }

    return PS_OK;
}

/**
* @brief Парсим трансформации и отдаём в виде матрицы
* @param xml
* @param attrName
* @return
*/
CMatrix SVGParser::parseTransform(QXmlStreamReader * xml, QString attrName)
{
    CMatrix matrix = CMatrix::identity(3, 3);

    QString transform = xml->attributes().value(attrName).toString();
    if ( transform.isEmpty() ) { return matrix; }


    QRegularExpression rxTransform("(\\w*)(?:[ ]+)?\\((.+)\\)"); //-- Получаем тип трансформации и её параметры
    rxTransform.setPatternOptions(QRegularExpression::InvertedGreedinessOption);
    QRegularExpressionMatchIterator rxTransformIter =  rxTransform.globalMatch(transform);

    QRegularExpression rxTransformParams(RX_PARAMS); //-- Разбираем параметры на отдельные

    if ( !rxTransformIter.hasNext() ) { throw 33; }

    typedef  QPair<QString, QString> TCommand;
    QList<TCommand> commandsList;

    while ( rxTransformIter.hasNext()  ) {
        QRegularExpressionMatch match = rxTransformIter.next();
        commandsList.append(TCommand(match.captured(1).toLower(), match.captured(2)));
    }

    foreach ( TCommand command, commandsList) {
        QString commandStr = command.first;
        QString paramsStr = command.second;

        QList<double> params;

        QRegularExpressionMatchIterator rxTransformParamsIter = rxTransformParams.globalMatch(paramsStr);
        while ( rxTransformParamsIter.hasNext() ) {
            QRegularExpressionMatch match = rxTransformParamsIter.next();
            params.append(match.captured(1).toDouble());
        }

        if ( commandStr=="matrix" ) { //-- Matrix 2x3
            if ( params.count()!=6 ) { throw 23; }
            matrix.setBy(3, 2, params, CMatrix::SET_BY_COLS);
        } else
        if ( commandStr=="translate" ) {
            if ( params.count()!=2 ) { throw 23; }
            matrix.translate(params[0], params[1]);
        } else
        if ( commandStr=="rotate" ) {
            if ( params.count()!=1 ) { throw 23; }
            matrix.rotateD(params[0]);
        } else
        if ( commandStr=="scale" ) {
            if ( (params.count()==0) || (params.count()>2) ) { throw 23; }
            if ( params.count()==1 ) { params.append(params[0]); }
            matrix.scale(params[0], params[1]);
        } else {
            throw 25;
        }

    }

    return matrix;
}

/**
* @brief Parse SVG element
* @param level
* @param xml
* @return
*/
CPrimitive* SVGParser::parseSVG(CNodeInterface** level, QXmlStreamReader* xml)
{
    CSVG * svg = new CSVG();
    *level = CNodeInterface::levelDown(*level, svg);
    parseBaseAttributes(svg, xml);

    //-- Parse size //TODO: Measure units
    svg->setSize(CSize(xml->attributes().value("width").toDouble(), xml->attributes().value("height").toDouble()));

    //-- Parse viewBox //TODO: Measure units
    QList<double> viewBoxParams = parseParams("viewBox", xml);
    svg->setViewBox(QRectF(
        (viewBoxParams.count()>0)? viewBoxParams[0] : 0,
        (viewBoxParams.count()>1)? viewBoxParams[1] : 0,
        (viewBoxParams.count()>2)? viewBoxParams[2] : 0,
        (viewBoxParams.count()>3)? viewBoxParams[3] : 0
    ));

    return svg;
}

/**
* @brief Парсим группу
* @param level
* @param xml
* @return
*/
CPrimitive *SVGParser::parseGroup(CNodeInterface **level, QXmlStreamReader * xml)
{
    CGroup * g = new CGroup();
    *level = CNodeInterface::levelDown(*level, g);
    parseBaseAttributes(g, xml);
    return g;
}

/**
* @brief Парсим путь
* @param level
* @param xml
* @return
*/
CPrimitive *SVGParser::parsePath(CNodeInterface *level, QXmlStreamReader * xml)
{
    CPath * path = new CPath();
    CNodeInterface::addNext(level, path);
    parseBaseAttributes(path, xml);

    QString pathD = xml->attributes().value("d").toString();

    CSS::Style style = path->styles(); //-- Задаём всем вложенным те же стили, что и у нас

    CPoint openPathCoords(0,0); //-- Запоминаем в каких координатах открыли путь, что бы потом можно было закрыть

    QString allowedCommands = "mcslvhvaqzMCSLVHAQZ";

    QRegularExpressionMatchIterator rxCommands = QRegularExpression(QString("([%1])([^%1]*)").arg(allowedCommands)).globalMatch(pathD); //-- Ищем команды и берём их параметры

    QList<CPoint> prevPoints; //-- Запоминаем список точек от предыдущей команды, т.к. они могут потребоваться для текущей

    CPoint lastPoint(0,0); //-- Крайняя точка крайнего перемещения, нужна для всех относительных команд

    while ( rxCommands.hasNext() ) {
        QRegularExpressionMatch rxCommand = rxCommands.next();

        QString command = rxCommand.captured(1);

        //-- Парсим все параметры команды в один список       
        QList<double> params = parseParams(rxCommand.captured(2));

        //-- Если текущий путь уже закрыт, значит дальше создаём составной путь
        if ( path->isClosed() ) {
            CPath * nextPath = new CPath();
            nextPath->setStyles(style);
            CNodeInterface::addNext(path, nextPath);
            path = nextPath;
        }

        //-- Ну и погнали парсить сами команды
        if ( (command=="M")||(command=="m") ) { //-- Перенос пера абсолютный/относительный
            if ( params.count()%2!=0 ) { throw 45; } //-- Должно быть кратно 2
            for (int pi=0;  pi<params.count(); pi+=2) {
                CPoint p1(params[pi], params[pi+1]);

                if ( command=="m" ) {
                    p1.add(lastPoint);
                }
                lastPoint = p1;

                if ( !openPathCoords.isZero() ) { //-- Будем считать, что следующие параметры после первых двух это линии
                    CLine * line = new CLine(_globalCoords, p1);
                    line->setStyles(style);
                    CNodeInterface::addNext(path, line);
                } else {
                    openPathCoords.set(lastPoint);
                }

                prevPoints.clear();
                prevPoints<<p1;

                _globalCoords = p1;
            }
        } else
        if ( (command=="C") || (command=="c") ) { //-- Cubic Bezier
            if (params.count()%6!=0) { throw 45; } //-- Должно быть кратно 6

            if ( openPathCoords.isZero() ) { openPathCoords = lastPoint; }
            for (int pi=0;  pi<params.count(); pi+=6) {
                CPoint p1(_globalCoords);
                CPoint p2(params[0+pi], params[1+pi]);
                CPoint p3(params[2+pi], params[3+pi]);
                CPoint p4(params[4+pi], params[5+pi]);
                if ( command=="c" ) {
                    p2.add(lastPoint);
                    p3.add(lastPoint);
                    p4.add(lastPoint);
                }
                lastPoint = p4;

                prevPoints.clear();
                prevPoints<<p1<<p2<<p3<<p4;

                CBezier * bezier = new CBezier(p1, p2, p3, p4);
                bezier->setStyles(style);
                CNodeInterface::addNext(path, bezier);
                _globalCoords = p4;
            }

        } else
        if ( (command=="Q") || (command=="q") ) { //-- Quadratic Bezier
            if ( params.count()%4!=0 ) { throw 45; } //-- Должно быть кратно 7 параметрам

            if ( openPathCoords.isZero() ) { openPathCoords = lastPoint; }
            for (int pi=0;  pi<params.count(); pi+=4) {
                CPoint p1(_globalCoords);
                CPoint p2(params[0+pi], params[1+pi]);
                CPoint p3(params[2+pi], params[3+pi]);
                if ( command=="q" ) {
                    p2.add(lastPoint);
                    p3.add(lastPoint);
                }
                lastPoint = p3;

                prevPoints.clear();
                prevPoints<<p1<<p2<<p3;

                CBezier * bezier = new CBezier(p1, p2, p3);
                bezier->setStyles(style);
                CNodeInterface::addNext(path, bezier);
                _globalCoords = p3;
            }

        } else
        if ( (command=="S") || (command=="s") ) { //-- Cubic Bezier with start on end prev
            if ( params.count()%4!=0 ) { throw 45; } //-- Должно быть кратно 4
            if ( openPathCoords.isZero() ) openPathCoords = lastPoint;

            for (int pi=0; pi<params.count(); pi+=4) {
                CPoint p1 = prevPoints[prevPoints.count()-1];
                CPoint p2 = (prevPoints.count()<=1)? _globalCoords : prevPoints[prevPoints.count()-2];
                CPoint p3(params[0+pi], params[1+pi]);
                CPoint p4(params[2+pi], params[3+pi]);
                p2.reflectP(p1); //-- Отражаем контрол относительно стартовой точки
                if ( command=="s" ) {
                    p3.add(lastPoint);
                    p4.add(lastPoint);
                }
                lastPoint = p4;

                prevPoints.clear();
                prevPoints<<p1<<p2<<p3<<p4;

                CBezier * bezier = new CBezier(p1, p2, p3, p4);
                bezier->setStyles(style);

                CNodeInterface::addNext(path, bezier);
                _globalCoords = p4;
            }
        } else
        if ( (command=="L") || (command=="l") ) { //-- Линия
            if ( params.count()%2!=0 ) { throw 45; } //-- Должно быть кратно 2

            for (int pi=0; pi<params.count(); pi+=2) {
                CPoint p(params[pi], params[pi+1]);

                if (command == "l") {
                    p.add(lastPoint);
                }
                lastPoint = p;
                if (openPathCoords.isZero()) openPathCoords.set(p);

                prevPoints.clear();
                prevPoints<<p;

                CLine * line = new CLine(_globalCoords, p);
                line->setStyles(style);
                CNodeInterface::addNext(path, line);
                _globalCoords = p;
            }
        } else
        if ( (command=="H") || (command=="h") ) { //-- Горизонтальная линия
            if ( params.count()==0 ) { throw 45; } //-- Хотя бы один параметр должен быть
            for (int pi=0; pi<params.count(); ++pi) {
                CPoint p(params[pi], lastPoint.y());
                if (command=="h") {
                    p.add(lastPoint.x(), 0);
                }
                lastPoint = p;
                if (openPathCoords.isZero()) openPathCoords.set(p);

                prevPoints.clear();
                prevPoints<<p;

                CLine * line = new CLine(_globalCoords, p);
                line->setStyles(style);
                CNodeInterface::addNext(path, line);
                _globalCoords = p;
            }
        } else
        if ( (command=="V") || (command=="v") ) { //-- Вертикальная линия
            if ( params.count()==0 ) { throw 45; } //-- Хотя бы один параметр должен быть
            for (int pi=0; pi<params.count(); ++pi) {
                CPoint p(lastPoint.x(), params[pi]);
                if ( command=="v" ) {
                     p.add(0, lastPoint.y());
                }
                lastPoint = p;
                if (openPathCoords.isZero()) openPathCoords.set(p);

                prevPoints.clear();
                prevPoints<<p;

                CLine * line = new CLine(_globalCoords, p);
                line->setStyles(style);
                CNodeInterface::addNext(path, line);
                _globalCoords = p;
            }
        } else
        if ( (command=="A") || (command=="a") ) {
            if ( params.count()%7!=0 ) { throw 45; } //-- Должно быть кратно 7 параметрам
            if ( openPathCoords.isZero() ) openPathCoords = lastPoint;

            for (int pi=0; pi<params.count(); pi+=7) {

                CPoint pStart(_globalCoords);
                double rx = params[0+pi];
                double ry = params[1+pi];
                double rotation = params[2+pi];
                bool largeArc = params[3+pi];
                bool sweep = params[4+pi];
                CPoint pEnd(params[5+pi], params[6+pi]);

                if ( command=="a" ) {
                    pEnd.add(lastPoint);
                }
                lastPoint = pEnd;

                prevPoints.clear();
                prevPoints<<pStart<<pEnd;

                CArc * arc = new CArc(pStart, rx, ry, rotation, largeArc, sweep, pEnd);
                arc->setStyles(style);
                CNodeInterface::addNext(path, arc);
                _globalCoords = pEnd;
            }

        } else
        if ( (command=="Z") || (command=="z") ) { //-- Закрываем путь

            path->setIsClosed(true);

            CPoint pe = openPathCoords;
            CPoint ps = lastPoint;

            if ( !ps.isEq(pe) ) { //-- Линию к началу, если отличается
                CLine * line = new CLine(ps, pe);
                line->setStyles(style);
                CNodeInterface::addNext(path, line);
            }

            lastPoint.set(openPathCoords);
            _globalCoords.set(pe);
            openPathCoords.set(0,0);

            //-- Всё, что дальше, будем считать новым путём

        } else { //-- Какая-то другая комманда, о которой мы не знаем
            qWarning()<<"Unknow command"<<command;
            throw 35;
        }

    }

    if ( lastPoint.isEq(openPathCoords) ) { path->setIsClosed(true); }

    if ( path->first==nullptr ) { //-- Что бы не болтался лишний раз, если нет ничего
        CNodeInterface::removeFromLevel(path);
        delete path;
    }

    return path;
}

/**
* @brief Парсим прямоугольник
* @param level
* @param xml
* @return
*/
CPrimitive *SVGParser::parseRect(CNodeInterface *level, QXmlStreamReader *xml)
{
    CRect * rect = new CRect();
    CNodeInterface::addNext(level, rect);
    parseBaseAttributes(rect, xml);
    if ( xml->attributes().hasAttribute("x") ) { rect->setX(xml->attributes().value("x").toDouble()); }
    if ( xml->attributes().hasAttribute("y") ) { rect->setY(xml->attributes().value("y").toDouble()); }
    if ( xml->attributes().hasAttribute("width") ) { rect->setWidth(xml->attributes().value("width").toDouble());  }
    if ( xml->attributes().hasAttribute("height") ) { rect->setHeight(xml->attributes().value("height").toDouble()); }
    if ( xml->attributes().hasAttribute("rx") ) { rect->setRX(xml->attributes().value("rx").toDouble()); }
    if ( xml->attributes().hasAttribute("rx") ) { rect->setRY(xml->attributes().value("ry").toDouble()); }
    return rect;
}

/**
* @brief Собираем все в кучу стили для конкретного итема (локально заданные и глобальные)
* @param xml
* @return
*/
CSS::Style SVGParser::parseStyle(QXmlStreamReader * xml)
{
    QString styles = xml->attributes().value("style").toString();
    QString nameClass = xml->attributes().value("class").toString();
    QString nameID = xml->attributes().value("id").toString();

    CSS::Block block(styles); //-- Распарсим локально заданные элементы стиля

    if ( !block.parse() ) { qWarning()<<"Problem parse css block"; }

    //-- У элемента так же могут быть заданы стили как атрибуты элемента - занесём их в стили
    QStringList cssTokens = {
        "stroke", "stroke-width", "stroke-dasharray", "stroke-opacity",
        "stroke-linejoin", "stroke-linecap", "stroke-miterlimit",
        "fill", "stop-color", "stop-opacity", "clip-path", "fill-opacity"
    };

    foreach(QString cssToken, cssTokens) {
        if ( xml->attributes().hasAttribute(cssToken) ) {
            QString val = xml->attributes().value(cssToken).toString();
            if ( !val.isEmpty() && val!="none" ) { block.set(cssToken, val); }
        }
    }

    CSS::Style style = _cssParser->applyStyles(QString(".%1").arg(nameClass), block); //-- Получим все стили для этого элемента, накладывая поверх локально заданные

    return style;
}

/**
* @brief Парсим контур обрезки
* @param level
* @param xml
* @return
*/
CPrimitive * SVGParser::parseClipPath(CNodeInterface **level, QXmlStreamReader *xml)
{
    FClipPath * clipPath = new FClipPath();

   _defs[xml->attributes().value("id").toString()] = clipPath;

   *level = clipPath->clipPath;

   parseBaseAttributes(clipPath->clipPath, xml);

   //-- Дальше парсится как обычно, но добавляется тупо к clipPath->clipPath, т.к. в clipPath могут быть и примитивы и хз что ещё.

   return nullptr;
}

/**
* @brief Парсим линейный градиент
* @param level
* @param xml
* @return
*/
CPrimitive * SVGParser::parseLinearGradient(CNodeInterface **level, QXmlStreamReader *xml)
{
    FLinearGradient * linearGradient = new FLinearGradient();
    *level = linearGradient;

    parseBaseAttributes(linearGradient, xml);

    _defs[linearGradient->ID()] = linearGradient;

    parseHREF(linearGradient, xml);

    CMatrix matrix = parseTransform(xml, "gradientTransform");
    linearGradient->setTransform(matrix);

    if ( xml->attributes().hasAttribute("x1") ) { linearGradient->setX1(CSS::MeasureUnit(xml->attributes().value("x1").toFloat()).val()); }
    if ( xml->attributes().hasAttribute("y1") ) { linearGradient->setY1(CSS::MeasureUnit(xml->attributes().value("y1").toFloat()).val()); }
    if ( xml->attributes().hasAttribute("x2") ) { linearGradient->setX2(CSS::MeasureUnit(xml->attributes().value("x2").toFloat()).val()); }
    if ( xml->attributes().hasAttribute("y2") ) { linearGradient->setY2(CSS::MeasureUnit(xml->attributes().value("y2").toFloat()).val()); }

    bool gs = parseGradientStops(linearGradient, xml);

    if ( !gs ) { return nullptr; }


    updateDependsHREFS(linearGradient);

    return nullptr;
}

/**
* @brief Parsing radial gradient
* @param level
* @param xml
* @return
*/
CPrimitive * SVGParser::parseRadialGradient(CNodeInterface **level, QXmlStreamReader *xml)
{
    FRadialGradient * radialGradient = new FRadialGradient();
    *level = radialGradient;

    parseBaseAttributes(radialGradient, xml);

    _defs[radialGradient->ID()] = radialGradient;

    parseHREF(radialGradient, xml);

    CMatrix matrix = parseTransform(xml, "gradientTransform");
    radialGradient->setTransform(matrix);

    if ( xml->attributes().hasAttribute("cx") ) { radialGradient->setCX(CSS::MeasureUnit(xml->attributes().value("cx").toFloat()).val()); }
    if ( xml->attributes().hasAttribute("cy") ) { radialGradient->setCY(CSS::MeasureUnit(xml->attributes().value("cy").toFloat()).val()); }
    if ( xml->attributes().hasAttribute("r") ) { radialGradient->setRadius(CSS::MeasureUnit(xml->attributes().value("r").toFloat()).val()); }
    if ( xml->attributes().hasAttribute("fx") ) { radialGradient->setFX(CSS::MeasureUnit(xml->attributes().value("fx").toFloat()).val()); }
    if ( xml->attributes().hasAttribute("fy") ) { radialGradient->setFY(CSS::MeasureUnit(xml->attributes().value("fy").toFloat()).val()); }
    if ( xml->attributes().hasAttribute("fr") ) { radialGradient->setFocalRadius(CSS::MeasureUnit(xml->attributes().value("fr").toFloat()).val()); }

    bool gs = parseGradientStops(radialGradient, xml);

    if ( !gs ) { return nullptr; }

    updateDependsHREFS(radialGradient);

    return nullptr;
}

/**
* @brief Парсим stop ы у градиента
* @param gradient
* @param xml
* @return
*/
bool SVGParser::parseGradientStops(FGradient *gradient, QXmlStreamReader *xml)
{
    QString gradientTagName = xml->name().toString();

    while ( !xml->atEnd() ) {
        if ( xml->hasError() ) { return false; }
        xml->readNext();

        if ( xml->tokenType()==QXmlStreamReader::StartElement ) {

            if ( xml->name().compare(QLatin1String("stop"))==0 )  {
                FGradient::TGradientStop gs;

                CSS::Style style = parseStyle(xml);

                gs.position = xml->attributes().value("offset").toFloat();

                if ( style.has("stop-color") ) {
                    gs.color = style.get("stop-color").value<QColor>();
                }

                if ( style.has("stop-opacity") ) {
                    gs.color.setAlphaF(style.get("stop-opacity").value<CSS::MeasureUnit>().val());
                }

                gradient->addStop(gs);
            }

        }

        if ( xml->tokenType()==QXmlStreamReader::EndElement ) {
            if ( xml->name()==gradientTagName ) {
                break;
            }
        }
    }

    return true;
}

/**
* @brief Разбираем базовые атрибуты элемента: id, class, style, transform
* @param itm
* @param xml
*/
void SVGParser::parseBaseAttributes(CPrimitive *itm, QXmlStreamReader *xml)
{
    //-- ID & Class name
    if ( xml->attributes().hasAttribute("id") ) { itm->setID(xml->attributes().value("id").toString()); }
    if ( xml->attributes().hasAttribute("class")) { itm->setClassSVG(xml->attributes().value("class").toString()); }

    //-- Styles
    CSS::Style styles = parseStyle(xml);
    itm->setStyles(styles);

    //-- Transforms    
    CMatrix transforms = parseTransform(xml);

    if ( nullptr!=itm->up ) { //-- Immediately combine with the transformation of the parent
        transforms = static_cast<CPrimitive*>(itm->up)->transform().clon().multiplication(transforms);
    }

    itm->setTransform(transforms);
}

/**
* @brief Parse href attribute
* @param el
* @param xml
*/
void SVGParser::parseHREF(CDef* el, QXmlStreamReader* xml)
{
    QString link;

    if ( xml->attributes().hasAttribute("href") ) { link = xml->attributes().value("href").toString(); }
    else if ( xml->attributes().hasAttribute("xlink:href") )  { link = xml->attributes().value("xlink:href").toString(); }

    if ( link.isEmpty() ) { return; }

    CDef * def = _defs.get(QUrl(link));
    if ( def!=nullptr ) { //-- Referenced element exists.
        el->setRelDef(def);
    } else { //-- Not exists, wait parsing
        _dependsCDef[link] = el;
    }
}

/**
* @brief Set href rel after parsing
* @param el
*/
void SVGParser::updateDependsHREFS(CDef* el)
{
    QString link = QString("#%1").arg(el->ID());
    if ( !_dependsCDef.contains(link) ) { return; }

    QList<CDef*> dependsEl = _dependsCDef.values(link);
    foreach (CDef* def, dependsEl) {
        def->setRelDef(el);
    }

    _dependsCDef.remove(link);
}

/**
* @brief Парсим линию
* @param level
* @param xml
* @return
*/
CPrimitive * SVGParser::parseLine(CNodeInterface * level, QXmlStreamReader * xml)
{
    QString x1 = xml->attributes().value("x1").toString();
    QString y1 = xml->attributes().value("y1").toString();
    QString x2 = xml->attributes().value("x2").toString();
    QString y2 = xml->attributes().value("y2").toString();

    CPoint p1(x1.toDouble(), y1.toDouble());
    CPoint p2(x2.toDouble(), y2.toDouble());

    CLine * line = new CLine(p1, p2);
    CNodeInterface::addNext(level, line);

    parseBaseAttributes(line, xml);

    return line;
}

/**
* @brief Парсим картинку
* @param level
* @param xml
* @return
*/
CPrimitive * SVGParser::parseImage(CNodeInterface *level, QXmlStreamReader * xml)
{
    //TODO: Доделать
    QString width = xml->attributes().value("width").toString();
    QString height = xml->attributes().value("height").toString();
    QString data = xml->attributes().value("xlink:href").toString();    

    CPoint p1(0, 0, 0.0001);

    QRegularExpressionMatch rxType = QRegularExpression("data:(.+);(.+),").match(data);

    if ( !rxType.hasMatch() ) {//-- Нипонятно что это есть
        qWarning()<<"Unknow image type!";
        return nullptr;
    }
    data = data.remove(0, rxType.captured(0).length()); //--Убираем начало, т.к. это не часть данных
    data = data.remove(QRegularExpression("(\\n|\\t| )")); //-- Убираем всё лишнее

    CImage * image = new CImage(p1, rxType.captured(1), rxType.captured(2), data.toLocal8Bit());
    CNodeInterface::addNext(level, image);
    parseBaseAttributes(image, xml);

    return image;
}

/**
* @brief Парсим стили
* @param level
* @param xml
* @return
*/
bool SVGParser::parseCss(CNodeInterface * level, QXmlStreamReader * xml)
{
    Q_UNUSED(level);
    return _cssParser->parse(xml->readElementText());
}

/**
* @brief Парсим окружность
* @param level
* @param xml
* @return
*/
CPrimitive *SVGParser::parseCircle(CNodeInterface *level, QXmlStreamReader *xml)
{
    double cx = xml->attributes().value("cx").toDouble();
    double cy = xml->attributes().value("cy").toDouble();
    double r = xml->attributes().value("r").toDouble();

    CPoint center(cx, cy);
    CCircle * circle = new CCircle(center, r);

    CNodeInterface::addNext(level, circle);
    parseBaseAttributes(circle, xml);

    return circle;
}

/**
* @brief Парсим эллипс
* @param level
* @param xml
* @return
*/
CPrimitive *SVGParser::parseEllipse(CNodeInterface *level, QXmlStreamReader *xml)
{
    CEllipse * ellipse = new CEllipse();
    CNodeInterface::addNext(level, ellipse);
    parseBaseAttributes(ellipse, xml);

    if ( xml->attributes().hasAttribute("cx") ) { ellipse->setCX(xml->attributes().value("cx").toDouble()); }
    if ( xml->attributes().hasAttribute("cy") ) { ellipse->setCY(xml->attributes().value("cy").toDouble()); }
    if ( xml->attributes().hasAttribute("rx") ) { ellipse->setRX(xml->attributes().value("rx").toDouble()); }
    if ( xml->attributes().hasAttribute("ry") ) { ellipse->setRY(xml->attributes().value("ry").toDouble()); }

    return ellipse;
}

/**
* @brief Парсим набор отрезков
* @param level
* @param xml
* @return
*/
CPrimitive *SVGParser::parsePolyline(CNodeInterface *level, QXmlStreamReader *xml)
{
    QList<double> points = parseParams("points", xml);
    if ( (points.count()==0) || (points.count()%2!=0) ) return nullptr;

    CPolyline * polyline = new CPolyline();
    CNodeInterface::addNext(level, polyline);
    parseBaseAttributes(polyline, xml);

    for(int i=0; i<points.count()-1; i+=2) {
        polyline->addPoint(CPoint(points[i], points[i+1]));
    }


    return polyline;
}

/**
* @brief Парсим многоугольник
* @param level
* @param xml
* @return
*/
CPrimitive * SVGParser::parsePolygon(CNodeInterface *level, QXmlStreamReader *xml)
{
    QList<double> points = parseParams("points", xml);
    if ( (points.count()==0) || (points.count()%2!=0) ) return nullptr;

    CPolygon * polygon = new CPolygon();
    CNodeInterface::addNext(level, polygon);
    parseBaseAttributes(polygon, xml);

    for(int i=0; i<points.count()-1; i+=2) {
        polygon->addPoint(CPoint(points[i], points[i+1]));
    }

    return polygon;
}

/**
* @brief Парсим значение атрибута как массив числовых значений
* @param attr
* @param xml
* @return
*/
QList<double> SVGParser::parseParams(QString attr, QXmlStreamReader *xml) const
{
    QString attrVal = xml->attributes().value(attr).toString();
    QList<double> list;
    list = parseParams(attrVal);
    return list;
}

QList<double> SVGParser::parseParams(QString params) const
{
    QList<double> list;
    if ( params.isEmpty() ) { return list; }

    QRegularExpressionMatchIterator rxParams = QRegularExpression(RX_PARAMS).globalMatch(params);

    while ( rxParams.hasNext() ) {
        QRegularExpressionMatch rxParam = rxParams.next();
        list.append(rxParam.captured(1).toDouble());
    }

    return list;
}
