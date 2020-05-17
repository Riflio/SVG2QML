#include "equal.h"
#include <QtMath>

Equal::Equal()
{

}

bool Equal::almostEqual(double a, double b, double tolerance)
{
    return (qAbs(a-b)<tolerance);
}
