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

	//interval in milliseconds
	void enable_fps_logging(unsigned int interval = 1000);

	void disable_fps_logging();

private:
	unsigned int current_fps;
	unsigned int last_fps;
	clock_t fps_recorded_time;
	bool fps_log;
	unsigned int log_interval;
	clock_t log_recorded_time;
};