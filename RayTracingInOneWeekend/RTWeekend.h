#pragma once

#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

const double INF = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932385;

inline double degrees_to_radians(double degrees)
{
	return degrees * PI / 180.0;
}

#include "Interval.h"
#include "Vec3.h"
#include "Color.h"
#include "Ray.h"

#endif
