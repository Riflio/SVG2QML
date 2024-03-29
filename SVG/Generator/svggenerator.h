#ifndef SVGGENERATOR_H
#define SVGGENERATOR_H

#include <QObject>

#include <QFile>
#include <QXmlStreamWriter>

#include "CSS/cssparser.h"

#include "Algebra/cmatrix.h"
#include "Algebra/cpoint.h"

#include "SVG/Classes/cbezier.h"
#include "SVG/Classes/cline.h"
#include "SVG/Classes/cpath.h"
#include "SVG/Classes/cgroup.h"
#include "SVG/Classes/cimage.h"
#include "SVG/Classes/carc.h"
#include "SVG/Classes/ccircle.h"
#include "SVG/Classes/crect.h"
#include "SVG/Classes/cellipse.h"
#include "SVG/Classes/cpolyline.h"
#include "SVG/Classes/cpolygon.h"

#include "SVG/Classes/flineargradient.h"
#include "SVG/Classes/fradialgradient.h"
#include "SVG/Classes/fclippath.h"

#include "SVG/Assets/cdefs.h"

class SVGGenerator : public QObject
{
    Q_OBJECT
public:
    explicit SVGGenerator(QObject *parent = nullptr);

    enum GenerateStatus {
        GS_OK,
        GS_NOFILE,
        GS_SYNTAXERROR
    };

    GenerateStatus generate(QIODevice *device, IPrimitive * rootItm);

signals:

public slots:

protected:
    QString generatePath(IPrimitive* item);

private:
    QXmlStreamWriter * _xml;
};

#endif // SVGGENERATOR_H
