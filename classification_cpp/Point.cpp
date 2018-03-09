#include "Point.h"
#include <cmath>

Point operator+(const Point & lhs, const Point & rhs)
{
	return { lhs.first + rhs.first, lhs.second + rhs.second };
}

Point operator-(const Point & lhs, const Point & rhs)
{
	return { lhs.first - rhs.first, lhs.second - rhs.second };
}

Point operator/(const Point & point, const int divisor)
{
	return { point.first / divisor, point.second / divisor };
}

double Distance(const Point & lhs, const Point & rhs)
{
	return hypot(lhs.first - rhs.first, lhs.second - rhs.second);
}

