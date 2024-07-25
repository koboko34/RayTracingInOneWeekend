#pragma once

#ifndef INTERVAL_H
#define INTERVAL_H

#include "RTWeekend.h"

struct Interval
{
	double min, max;

	Interval() : min(+INF), max(-INF) {}
	Interval(double min, double max) : min(min), max(max) {}
	Interval(const Interval& a, const Interval& b)
	{
		min = a.min <= b.min ? a.min : b.min;
		max = a.max <= b.max ? a.max : b.max;
	}

	double size() const { return max - min; }
	bool contains(double x) const { return min <= x && x <= max; }
	bool surrrounds(double x) const { return min < x && x < max; }

	double clamp(double x) const
	{
		if (x < min)
			return min;
		if (x > max)
			return max;
		return x;
	}

	Interval expand(double delta) const
	{
		double padding = delta / 2;
		return Interval(min - padding, max + padding);
	}

	static const Interval empty, universe;
};

const Interval Interval::empty		= Interval(INF, -INF);
const Interval Interval::universe	= Interval(-INF, INF);

#endif