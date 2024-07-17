#pragma once

#ifndef HITTABLE_H
#define HITTABLE_H

struct HitRecord
{
	Point3 p;
	Vec3 normal;
	double t;
	bool front_face;

	void set_face_normal(const Ray& r, const Vec3& outward_normal)
	{
		// outward_normal is assumed to have unit length

		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class Hittable
{
public:
	virtual ~Hittable() = default;

	virtual bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const = 0;
};

#endif