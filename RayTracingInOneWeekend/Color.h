#pragma once

#ifndef COLOR_H
#define COLOR_H

#include <string>

#include "Interval.h"

using Color = Vec3;

inline double linear_to_gamma(double linear_component)
{
	if (linear_component > 0)
		return sqrt(linear_component);

	return 0;
}

std::string write_color(std::ostream& out, const Color& pixel_color)
{
	double r = pixel_color.x();
	double g = pixel_color.y();
	double b = pixel_color.z();

	// apply linear to gamma transform for gamma 2
	r = linear_to_gamma(r);
	g = linear_to_gamma(g);
	b = linear_to_gamma(b);

	// translate from 0 to 1 into 0 to 255
	static const Interval intensity(0.0, 0.999);
	int rbyte = int(256 * intensity.clamp(r));
	int gbyte = int(256 * intensity.clamp(g));
	int bbyte = int(256 * intensity.clamp(b));

	std::string out_color = std::to_string(rbyte) + " " + std::to_string(gbyte) + " " + std::to_string(bbyte) + "\n";
	return out_color;
}

#endif