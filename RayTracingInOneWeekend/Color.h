#pragma once

#ifndef COLOR_H
#define COLOR_H

#include "Interval.h"

using Color = Vec3;

void write_color(std::ostream& out, const Color& pixel_color)
{
	double r = pixel_color.x();
	double g = pixel_color.y();
	double b = pixel_color.z();

	// translate from 0 to 1 into 0 to 255
	static const Interval intensity(0.0, 0.999);
	int rbyte = int(256 * intensity.clamp(r));
	int gbyte = int(256 * intensity.clamp(g));
	int bbyte = int(256 * intensity.clamp(b));

	out << rbyte << " " << gbyte << " " << bbyte << "\n";
}

#endif