#include "RTWeekend.h"

#include "Hittable.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Camera.h"

int main()
{
	HittableList world;
	world.add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5));
	world.add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100));

	Camera camera;
	camera.aspect_ratio = 16.0 / 9.0;
	camera.image_width = 400;
	camera.samples_per_pixel = 50;
	camera.max_depth = 50;
	camera.render(world);

	return 0;
}