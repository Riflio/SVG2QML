#ifndef EQUAL_H
#define EQUAL_H

#include <limits>

#define NAN_DOUBLE std::numeric_limits<double>::quiet_NaN()
#define MAX_DOUBLE 99999999
#define MIN_DOUBLE -99999999

class Equal
{
public:
    Equal();
    constexpr static const double EPS = 0.00000001;
    static bool almostEqual(double a, double b, double tolerance = EPS);
};

#endif // EQUAL_H
