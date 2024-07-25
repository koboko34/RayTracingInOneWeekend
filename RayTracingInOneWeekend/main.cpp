#include "RTWeekend.h"

#include "Hittable.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Material.h"
#include "Camera.h"
#include "Timer.h"

int main()
{
	Timer timer("Render");

	HittableList world;

	auto material_ground = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
	world.add(std::make_shared<Sphere>(Point3(0, -1000.0, 0), 1000.0, material_ground));

	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			double choose_mat = random_double();
			Point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if ((center - Point3(4.0, 0.2, 0.0)).length() > 0.9)
			{
				std::shared_ptr<Material> sphere_mat;

				if (choose_mat < 0.6)
				{
					// diffuse
					Color albedo = Color::random() * Color::random();
					sphere_mat = std::make_shared<Lambertian>(albedo);
					world.add(std::make_shared<Sphere>(center, 0.2, sphere_mat));
				}
				else if (choose_mat < 0.9)
				{
					// metal
					Color albedo = Color::random(0.5, 1.0);
					double fuzz = random_double(0, 0.5);
					sphere_mat = std::make_shared<Metal>(albedo, fuzz);
					world.add(std::make_shared<Sphere>(center, 0.2, sphere_mat));
				}
				else
				{
					// glass
					sphere_mat = std::make_shared<Dielectric>(1.5);
					world.add(std::make_shared<Sphere>(center, 0.2, sphere_mat));
				}
			}
		}
	}

	auto material_one = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
	world.add(std::make_shared<Sphere>(Point3(-4.0, 1.0, 0), 1.0, material_one));

	auto material_two = std::make_shared<Dielectric>(1.5);
	world.add(std::make_shared<Sphere>(Point3(0, 1.0, 0), 1.0, material_two));

	auto material_three = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0);
	world.add(std::make_shared<Sphere>(Point3(4.0, 1.0, 0), 1.0, material_three));
	

	Camera camera(world);
	camera.aspect_ratio = 16.0 / 9.0;
	camera.image_width = 1920;
	camera.samples_per_pixel = 500;
	camera.max_depth = 50;

	camera.vfov = 20;
	camera.lookfrom = Point3(13.0 , 2.0, 3.0);
	camera.lookat = Point3(0, 0, 0);
	camera.vup = Vec3(0, 1, 0);

	camera.defocus_angle = 0.6;
	camera.focus_dist = 10.0;

	camera.render();

	return 0;
}