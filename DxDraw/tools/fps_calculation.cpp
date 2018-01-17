#include "fps_calculation.h"

fps_calculator::fps_calculator()
{
	this->current_fps = 0;
	this->last_fps = 0;
	this->recorded_time = NULL;
}

fps_calculator::~fps_calculator()
{
	this->current_fps = 0;
	this->last_fps = 0;
	this->recorded_time = NULL;
}

void fps_calculator::update_frame()
{
	//increment frame counter
	this->current_fps += 1;
	
	//check to see if the difference in time is greater than 1 second
	if (clock() - this->recorded_time > 1000)
	{
		//1 second has passed so we update the previous frame fps and reset time
		this->last_fps = this->current_fps;
		this->current_fps = 0;
		this->recorded_time = clock();
	}
}

unsigned int fps_calculator::get_fps()
{
	return this->last_fps;
}

void fps_calculator::reset_timer()
{
	this->recorded_time = clock();
}