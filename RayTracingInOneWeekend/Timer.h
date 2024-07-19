#pragma once

#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer
{
public:
	Timer(const std::string timer_name) : timer_name(timer_name), start(std::chrono::high_resolution_clock::now()) {}
	~Timer()
	{
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> duration = end - start;
		float duration_in_mins = duration.count() / 60.f;

		std::clog << std::endl << timer_name << " took " << duration_in_mins << " minutes." << std::endl;
	}

private:
	const std::string timer_name;
	std::chrono::steady_clock::time_point start;
};

#endif
