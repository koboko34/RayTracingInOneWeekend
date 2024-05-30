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
			double r = double(i) / (WIDTH - 1);
			double g = double(j) / (HEIGHT - 1);
			double b = 0.0;

			int ir = int(255.999 * r);
			int ig = int(255.999 * g);
			int ib = int(255.999 * b);

			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}

	std::clog << "\rDone.                  \n";

	return 0;
}