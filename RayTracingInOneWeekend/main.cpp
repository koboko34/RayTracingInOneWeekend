#include "Vec3.h"
#include "Color.h"
#include "Ray.h"

#include <iostream>

double hit_sphere(const Point3& center, double radius, const Ray& r)
{
	Vec3 oc = center - r.origin();
	double a = r.direction().length_squared();
	double h = dot(r.direction(), oc);
	double c = oc.length_squared() - radius * radius;
	double discriminant = h * h - a * c;
	
	if (discriminant < 0)
	{
		return -1.0;
	}
	return (h - sqrt(discriminant)) / a;
}

Color ray_color(const Ray& r)
{
	Point3 center = Point3(0, 0, -1);
	auto t = hit_sphere(center, 0.5, r);
	if (t > 0.0)
	{
		Vec3 normal = unit_vector(r.at(t) - center);
		return 0.5 * Color(normal.x() + 1, normal.y() + 1, normal.z() + 1);
	}
	
	Vec3 unit_dir = unit_vector(r.direction());
	double a = 0.5 * (unit_dir.y() + 1.0);
	return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}

int main()
{
	// Image
	double aspect_ratio = 16.0 / 9.0;
	const int IMAGE_WIDTH = 400;
	int IMAGE_HEIGHT = int(IMAGE_WIDTH / aspect_ratio);
	IMAGE_HEIGHT = (IMAGE_HEIGHT < 1) ? 1 : IMAGE_HEIGHT;

	// Camera
	double focal_length = 1.0;
	Point3 camera_center = Point3(0, 0, 0);
	const double VIEWPORT_HEIGHT = 2.0;
	const double VIEWPORT_WIDTH = VIEWPORT_HEIGHT * (double(IMAGE_WIDTH) / IMAGE_HEIGHT);

	Vec3 viewport_u = Vec3(VIEWPORT_WIDTH, 0, 0);
	Vec3 viewport_v = Vec3(0, -VIEWPORT_HEIGHT, 0);

	Vec3 pixel_delta_u = viewport_u / IMAGE_WIDTH;
	Vec3 pixel_delta_v = viewport_v / IMAGE_HEIGHT;

	Point3 viewport_upper_left = camera_center - Vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
	Point3 start_pixel_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

	std::cout << "P3\n" << IMAGE_WIDTH << " " << IMAGE_HEIGHT << "\n255\n";

	for (size_t j = 0; j < IMAGE_HEIGHT; j++)
	{
		std::clog << "\rScanlines remaining: " << (IMAGE_HEIGHT - j) << " " << std::flush;

		for (size_t i = 0; i < IMAGE_WIDTH; i++)
		{
			Point3 pixel_center = start_pixel_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
			Vec3 dir = pixel_center - camera_center;
			Ray r(camera_center, dir);
			
			Color pixel_color = ray_color(r);
			write_color(std::cout, pixel_color);
		}
	}

	std::clog << "\rDone.                  \n";

	return 0;
}