#ifndef CPATH_H
#define CPATH_H

#include "SVG/Assets/cprimitive.h"

class CPath: public CPrimitive
{
public:
    CPath();
    bool isClosed() const;
    void setIsClosed(bool closed);

    void reverse();

    bool toPath() { return true; }

    CPath * makeOffset(double d);

private:
    bool _isClosed;
};

#endif // CPATH_H
