#pragma once
#include <ctime>
#include <Windows.h>

#include "..\helpers\helpers.h"

class fps_calculator
{
public:
	fps_calculator();
	~fps_calculator();
	
	void update_frame();

	unsigned int get_fps();

	void reset_timer();

private:
	unsigned int current_fps;
	unsigned int last_fps;
	clock_t recorded_time;
};