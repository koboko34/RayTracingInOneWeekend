#include "Vec3.h"
#include "Color.h"

#include <iostream>

int main()
{
	const int WIDTH = 256;
	const int HEIGHT = 256;

	std::cout << "P3\n" << WIDTH << " " << HEIGHT << "\n255\n";

	for (size_t j = 0; j < HEIGHT; j++)
	{
		std::clog << "\rScanlines remaining: " << (HEIGHT - j) << " " << std::flush;

		for (size_t i = 0; i < WIDTH; i++)
		{
			Color pixel_color(double(i) / (WIDTH - 1), double(j) / (HEIGHT - 1), 0.0);
			write_color(std::cout, pixel_color);
		}
	}

	std::clog << "\rDone.                  \n";

	return 0;
}