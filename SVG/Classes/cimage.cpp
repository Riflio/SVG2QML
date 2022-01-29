#include "cimage.h"
#include <QDebug>

CImage::CImage(CPoint topLeft, QString imgType, QString encoding, QByteArray data):
    CPrimitive(PT_IMAGE), _imgType(imgType), _encoding(encoding), _data(data)
{
    qDebug()<<"IMAGE"<<imgType<<encoding<<data.left(80);

    CPoints p;
    p.add(topLeft);
}

/**
* @brief Отрисовываем картинку
* @param painter
* @param scale
*/
void CImage::cpaint(QPainter *painter, const CBoundingBox &area)
{
    Q_UNUSED(area);
    qDebug()<<"Draw IMAGE";
    QImage image;

    if ( (_imgType=="image/jpeg") && (!image.loadFromData(QByteArray::fromBase64(_data), "jpg")) ) return;

    CMatrix resultMatrix;
    CMatrix mScale(3, 3);
    //mScale.scale(scale, scale);


    resultMatrix = _transformMatrix.clon().multiplication(mScale);

    resultMatrix.setAt(0,2, points().p1().x());
    resultMatrix.setAt(1,2, points().p1().y());

    //--Фиксим поворот не в ту сторону
    resultMatrix.setAt(0,1, -1 * resultMatrix.getAt(0,1));
    resultMatrix.setAt(1,0, -1 * resultMatrix.getAt(1,0));

    painter->save();

    painter->setTransform(QTransform(
        resultMatrix.getAt(0,0), resultMatrix.getAt(1,0), resultMatrix.getAt(2,0),
        resultMatrix.getAt(0,1), resultMatrix.getAt(1,1), resultMatrix.getAt(2,1),
        resultMatrix.getAt(0,2), resultMatrix.getAt(1,2), resultMatrix.getAt(2,2)
    ), false);
    painter->drawImage(0,0, image);

    painter->restore();

}

void CImage::cstream(QDataStream &dataStream, double scale)
{
    Q_UNUSED(dataStream);
    Q_UNUSED(scale);
    //TODO: Пора бы сделать
}

CPoints CImage::lianirize(double tol) const
{
    Q_UNUSED(tol);
    CPoints res;
    return res;
}

QString CImage::imageType() const
{
    return _imgType;
}
