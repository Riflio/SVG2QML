#include "cdef.h"

CDef::CDef() : CPrimitive(PT_DEF), _relDef(nullptr)
{

}

bool CDef::hasRelDef() const
{
    return ( _relDef!=nullptr );
}

CDef* CDef::relDef() const
{
    return _relDef;
}

void CDef::setRelDef(CDef* relDef)
{
    _relDef = relDef;
}
