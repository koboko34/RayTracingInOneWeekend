#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include <thread>
#include <future>
#include <mutex>

class Camera
{
public:
	double aspect_ratio = 1.0;
	int image_width = 100;
	int samples_per_pixel = 10;
	int max_depth = 10;

	double vfov = 90.0;
	Point3 lookfrom = Point3(0, 0, 0);
	Point3 lookat = Point3(0, 0, -1);
	Vec3 vup = Vec3(0, 1, 0);		// camera relative up direction

	double defocus_angle = 0.0;		// variation angle of rays through each pixel
	double focus_dist = 10.0;		// distance from camera lookfrom point to plane of perfect focus

	Camera(const Hittable& world) : world(world) {}

	void render()
	{
		initialise();

		// -1 == not yet handled by any thread, any number other than -1 signifies which thread is handling the line
		lines_rendered.resize(image_height, -1);
		lines_as_string.resize(image_height);

		std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

		const int numOfThreads = std::thread::hardware_concurrency();
		std::vector<std::future<void>> futures;

		for (int i = 0; i < numOfThreads; ++i)
		{
			futures.push_back(std::async(std::launch::async, &Camera::render_next_line, this, i));
		}

		for (auto& future : futures)
		{
			future.get();
		}

		for (const std::string& s : lines_as_string)
		{
			std::cout << s;
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
	Vec3 u, v, w;
	Vec3 defocus_disk_u;
	Vec3 defocus_disk_v;
	const Hittable& world;

	int lines_left;
	std::vector<int> lines_rendered;
	std::vector<std::string> lines_as_string;
	std::mutex mtx;

	void initialise()
	{		
		image_height = int(image_width / aspect_ratio);
		image_height = (image_height < 1) ? 1 : image_height;
		lines_left = image_height;

		pixel_samples_scale = 1.0 / samples_per_pixel;
		
		camera_center = lookfrom;
		
		double theta = degrees_to_radians(vfov);
		double h = tan(theta / 2.0);
		const double VIEWPORT_HEIGHT = 2.0 * h * focus_dist;
		const double VIEWPORT_WIDTH = VIEWPORT_HEIGHT * (double(image_width) / image_height);

		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		Vec3 viewport_u = VIEWPORT_WIDTH * u;
		Vec3 viewport_v = VIEWPORT_HEIGHT * -v;

		pixel_delta_u = viewport_u / image_width;
		pixel_delta_v = viewport_v / image_height;

		Point3 viewport_upper_left = camera_center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
		start_pixel_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

		// calculate the camera defocus disk basis vectors
		double defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle / 2));
		defocus_disk_u = u * defocus_radius;
		defocus_disk_v = v * defocus_radius;
	}

	Ray get_ray(int i, int j) const
	{
		// construct a ray going from the defocus disk onto a randomly sampled point around a pixel at i, j
		Vec3 offset = sample_square();
		Vec3 pixel_sample = start_pixel_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

		Point3 ray_origin = defocus_angle <= 0 ? camera_center : defocus_disk_sample();
		Vec3 ray_direction = pixel_sample - ray_origin;

		return Ray(ray_origin, ray_direction);
	}

	Vec3 sample_square() const
	{
		// returns a vector to a random point inside the unit square
		return Vec3(random_double() - 0.5, random_double() - 0.5, 0);
	}

	Point3 defocus_disk_sample() const
	{
		// returns a random point on the camera defocus disk
		Vec3 p = random_in_unit_disk();
		return camera_center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
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

	void render_next_line(int id)
	{		
		std::unique_lock<std::mutex> lock(mtx);
		
		bool found_empty_line = false;
		for (int j = 0; j < image_height; j++)
		{
			if (lines_rendered[j] == -1)
			{
				lines_rendered[j] = id;
				std::clog << "\rScanlines remaining: " << std::to_string(lines_left) << " " << std::flush;
				lines_left--;

				lock.unlock();
				found_empty_line = true;

				// render the line
				for (int i = 0; i < image_width; i++)
				{
					Color pixel_color(0, 0, 0);
					for (int sample = 0; sample < samples_per_pixel; sample++)
					{
						Ray r = get_ray(i, j);
						pixel_color += ray_color(r, max_depth, world);
					}

					lines_as_string[j] += write_color(std::cout, pixel_samples_scale * pixel_color);
				}
				break;
			}
		}

		if (lock.owns_lock())
		{
			lock.unlock();
		}

		if (found_empty_line)
		{
			render_next_line(id);
		}
	}
};

#endif
