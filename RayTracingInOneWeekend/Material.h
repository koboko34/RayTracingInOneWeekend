#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

struct HitRecord;

class Material
{
public:
	virtual ~Material() = default;

	virtual bool scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& r_out) const
	{
		return false;
	}
};

class Lambertian : public Material
{
public:
	Lambertian(const Color& albedo) : albedo(albedo) {}

	bool scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& r_out) const override
	{
		Vec3 scatter_direction = rec.normal + random_unit_vector();

		// catch degenerate scatter direction
		if (scatter_direction.near_zero())
			scatter_direction = rec.normal;

		r_out = Ray(rec.p, scatter_direction);
		attenuation = albedo;
		return true;
	}

private:
	Color albedo;
};

class Metal : public Material
{
public:
	Metal(const Color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

	bool scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& r_out) const override
	{
		Vec3 reflected = reflect(r_in.direction(), rec.normal);
		reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
		r_out = Ray(rec.p, reflected);
		attenuation = albedo;
		return (dot(r_out.direction(), rec.normal) > 0);
	}

private:
	Color albedo;
	double fuzz;
};

class Dielectric : public Material
{
public:
	Dielectric(double refraction_index) : refraction_index(refraction_index) {}

	bool scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& r_out) const override
	{
		attenuation = Color(1.0, 1.0, 1.0);
		double ri = rec.front_face ? (1.0 / refraction_index) : refraction_index;

		Vec3 unit_dir = unit_vector(r_in.direction());
		double cos_theta = fmin(dot(-unit_dir, rec.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

		bool cannot_refract = ri * sin_theta > 1.0;
		Vec3 dir;

		if (cannot_refract || reflectance(cos_theta, ri) > random_double())
		{
			dir = reflect(unit_dir, rec.normal);
		}
		else
		{
			dir = refract(unit_dir, rec.normal, ri);
		}

		r_out = Ray(rec.p, dir);
		return true;
	}

private:
	double refraction_index;

	static double reflectance(double cosine, double refraction_index)
	{
		// Schlick's approximation for reflectance
		double r0 = (1 - refraction_index) / (1 + refraction_index);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
};

#endif