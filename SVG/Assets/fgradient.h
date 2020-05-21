#ifndef FGRADIENT_H
#define FGRADIENT_H

#include "cdef.h"
#include <QList>
#include <QColor>

class FGradient: public CDef
{
public:
    FGradient();
    FGradient(const FGradient &other);

    struct TGradientStop {
        float position;
        QColor color;
    };

    void addStop(const TGradientStop &gs);

    QList<TGradientStop> stops() const;


protected:
    QList<TGradientStop> _stops;

};

#endif // FGRADIENT_H
