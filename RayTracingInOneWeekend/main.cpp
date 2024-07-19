#include "RTWeekend.h"

#include "Hittable.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Material.h"
#include "Camera.h"

int main()
{
	HittableList world;

	double R = cos(PI / 4);

	auto material_ground = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
	auto material_center = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
	auto material_left = std::make_shared<Dielectric>(1.50);
	auto material_bubble = std::make_shared<Dielectric>(1.00 / 1.50);
	auto material_right = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0);

	world.add(std::make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, material_ground));
	world.add(std::make_shared<Sphere>(Point3(0.0, 0.0, -1.2), 0.5, material_center));
	world.add(std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, material_left));
	world.add(std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.4, material_bubble));
	world.add(std::make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_right));

	Camera camera;
	camera.aspect_ratio = 16.0 / 9.0;
	camera.image_width = 400;
	camera.samples_per_pixel = 50;
	camera.max_depth = 50;

	camera.vfov = 20;
	camera.lookfrom = Point3(-2, 2, 1);
	camera.lookat = Point3(0, 0, -1);
	camera.vup = Vec3(0, 1, 0);

	camera.render(world);

	return 0;
}