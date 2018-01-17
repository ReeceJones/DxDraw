#include "fps_calculation.h"

fps_calculator::fps_calculator()
{
	this->current_fps = 0;
	this->last_fps = 0;
	this->fps_recorded_time = NULL;
	this->log_recorded_time = NULL;
	this->fps_log = false;
}

fps_calculator::~fps_calculator()
{
	this->current_fps = 0;
	this->last_fps = 0;
	this->fps_recorded_time = NULL;
	this->log_recorded_time = NULL;
	this->fps_log = false;
}

void fps_calculator::update_frame()
{
	//increment frame counter
	this->current_fps += 1;

	//check to see if the difference in time is greater than 1 second
	if (clock() - this->fps_recorded_time > 1000)
	{
		//1 second has passed so we update the previous frame fps and reset time
		this->last_fps = this->current_fps;
		this->current_fps = 0;
		this->fps_recorded_time = clock();
	}

	if (this->fps_log && (clock() - this->log_recorded_time) > this->log_interval)
	{
		helpers::err_print(helpers::ERR_INFO, "FPS: %i", this->get_fps());
		this->log_recorded_time = clock();
	}
}

unsigned int fps_calculator::get_fps()
{
	return this->last_fps;
}

void fps_calculator::reset_timer()
{
	this->fps_recorded_time = clock();
}

void fps_calculator::enable_fps_logging(unsigned int interval)
{
	this->fps_log = true;
	this->log_interval = interval;
}

void fps_calculator::disable_fps_logging()
{
	this->fps_log = false;
}