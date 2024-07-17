#pragma once

#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "Hittable.h"

#include <vector>

class HittableList : public Hittable
{
public:
	std::vector<std::shared_ptr<Hittable>> objects;
	
	HittableList() {}
	HittableList(std::shared_ptr<Hittable> object) { add(object); }

	void clear() { objects.clear(); }
	void add(std::shared_ptr<Hittable> object) { objects.push_back(object); }

	bool hit(const Ray& r, double ray_tmin, double ray_tmax, HitRecord& rec) const override
	{
		HitRecord temp_rec;
		bool hit_anything = false;
		double closest_so_far = ray_tmax;

		for (const std::shared_ptr<Hittable>& object : objects)
		{
			if (object->hit(r, ray_tmin, closest_so_far, temp_rec))
			{
				hit_anything = true;
				closest_so_far = temp_rec.t;
				rec = temp_rec;
			}
		}

		return hit_anything;
	}
};

#endif
