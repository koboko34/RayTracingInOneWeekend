#pragma once

#ifndef CAMERA_H
#define CAMERA_H

class Camera
{
public:
	double aspect_ratio = 1.0;
	int image_width = 100;
	int samples_per_pixel = 10;
	int max_depth = 10;

	void render(const Hittable& world)
	{
		initialise();

		std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

		for (int j = 0; j < image_height; j++)
		{
			std::clog << "\rScanlines remaining: " << (image_height - j) << " " << std::flush;

			for (int i = 0; i < image_width; i++)
			{
				Color pixel_color(0, 0, 0);
				for (int sample = 0; sample < samples_per_pixel; sample++)
				{
					Ray r = get_ray(i, j);
					pixel_color += ray_color(r, max_depth, world);
				}

				write_color(std::cout, pixel_samples_scale * pixel_color);
			}
		}

		std::clog << "\rDone.                  \n";
	}

private:
	
	int image_height;
	double pixel_samples_scale;
	Vec3 pixel_delta_u;
	Vec3 pixel_delta_v;
	Point3 camera_center;
	Point3 start_pixel_loc;

	void initialise()
	{
		image_height = int(image_width / aspect_ratio);
		image_height = (image_height < 1) ? 1 : image_height;

		pixel_samples_scale = 1.0 / samples_per_pixel;
		
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

	Ray get_ray(int i, int j) const
	{
		// construct a ray going from the camera onto a randomly sampled point around a pixel at i, j
		Vec3 offset = sample_square();
		Vec3 pixel_sample = start_pixel_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

		Point3 ray_origin = camera_center;
		Vec3 ray_direction = pixel_sample - ray_origin;

		return Ray(ray_origin, ray_direction);
	}

	Vec3 sample_square() const
	{
		// returns a vector to a random point inside the unit square
		return Vec3(random_double() - 0.5, random_double() - 0.5, 0);
	}

	Color ray_color(const Ray& r, int depth, const Hittable& world) const
	{
		if (depth <= 0)
			return Color(0, 0, 0);
		
		HitRecord rec;
		if (world.hit(r, Interval(0.001, INF), rec))
		{
			Ray scattered;
			Color attenuation;
			if (rec.mat->scatter(r, rec, attenuation, scattered))
			{
				return attenuation * ray_color(scattered, depth - 1, world);
			}
			
			return Color(0, 0, 0);
		}

		Vec3 unit_dir = unit_vector(r.direction());
		double a = 0.5 * (unit_dir.y() + 1.0);
		return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
	}
};

#endif
