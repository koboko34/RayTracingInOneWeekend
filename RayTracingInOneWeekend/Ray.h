#pragma once

#ifndef RAY_H
#define RAY_H

#include "Vec3.h"

class Ray {
public:
	Ray() {}
	Ray(const Point3& origin, const Vec3 direction) : orig(origin), dir(direction) {}

	const Point3& origin() const { return orig; }
	const Vec3& direction() const { return dir; }

	Point3 at(double t) const
	{
		return orig + t * dir;
	}

private:
	Point3 orig;
	Vec3 dir;
};

#endif