#pragma once

#ifndef CAMERA_H
#define CAMERA_H

class Camera
{
public:
	double aspect_ratio = 1.0;
	int image_width = 100;

	void render(const Hittable& world)
	{
		initialise();

		std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

		for (size_t j = 0; j < image_height; j++)
		{
			std::clog << "\rScanlines remaining: " << (image_height - j) << " " << std::flush;

			for (size_t i = 0; i < image_width; i++)
			{
				Point3 pixel_center = start_pixel_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
				Vec3 dir = pixel_center - camera_center;
				Ray r(camera_center, dir);

				Color pixel_color = ray_color(r, world);
				write_color(std::cout, pixel_color);
			}
		}

		std::clog << "\rDone.                  \n";
	}

private:
	
	int image_height;
	Vec3 pixel_delta_u;
	Vec3 pixel_delta_v;
	Point3 camera_center;
	Point3 start_pixel_loc;

	void initialise()
	{
		image_height = int(image_width / aspect_ratio);
		image_height = (image_height < 1) ? 1 : image_height;
		
		camera_center = Point3(0, 0, 0);
		
		double focal_length = 1.0;
		const double VIEWPORT_HEIGHT = 2.0;
		const double VIEWPORT_WIDTH = VIEWPORT_HEIGHT * (double(image_width) / image_height);

		Vec3 viewport_u = Vec3(VIEWPORT_WIDTH, 0, 0);
		Vec3 viewport_v = Vec3(0, -VIEWPORT_HEIGHT, 0);

		pixel_delta_u = viewport_u / image_width;
		pixel_delta_v = viewport_v / image_height;

		Point3 viewport_upper_left = camera_center - Vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
		start_pixel_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
	}

	Color ray_color(const Ray& r, const Hittable& world) const
	{
		HitRecord rec;
		if (world.hit(r, Interval(0, INF), rec))
		{
			return 0.5 * (rec.normal + Color(1, 1, 1));
		}

		Vec3 unit_dir = unit_vector(r.direction());
		double a = 0.5 * (unit_dir.y() + 1.0);
		return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
	}
};

#endif
