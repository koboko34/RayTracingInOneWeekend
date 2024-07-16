#pragma once

#ifndef HITTABLE_H
#define HITTABLE_H

#include "Ray.h"

struct Hit_Record
{
	Point3 p;
	Vec3 normal;
	double t;
};

class Hittable
{
public:
	virtual ~Hittable() = default;

	virtual bool hit(const Ray& r, double ray_tmin, double ray_tmax, Hit_Record& rec) const = 0;
};

#endif