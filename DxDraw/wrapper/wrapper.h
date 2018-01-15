#pragma once
#include "..\defines.h"
#include "..\helpers\helpers.h"

#include "..\DirectX11\Dx11Renderer.h"

#include <vector>

enum CIRCLE_MODE : int
{
	CIRCLE_SOLID = 0, 
	CIRCLE_LINE,
	CIRCLE_WEIRD
};

//TODODODODODODOODODODOD: BEFORE CONTINUING CONVERT DECIMALS TO COORDINATES SO WE DONT HAVE THESE TRASH WONKY SHAPES
namespace wrapper
{
	struct color
	{
		float r, g, b, a;
	};
	namespace D3D11
	{
		/*
		color_info:
			-index 0: top left
			-index 1: top right
			-index 2: bottom right
			-index 3: bottom left
		*/
		extern dx_11_buffer create_square(float x, float y, float width, float height, float z, color color_info[4], dx_11_renderer dx_renderer);

		/*
		color_info:
		-index 0: point 1
		-index 1: point 2
		-index 2: point 3
		*/
		extern dx_11_buffer create_triangle(float x1, float y1, float x2, float y2, float x3, float y3, float z, color color_info[3], dx_11_renderer dx_renderer);

		/*
		color_info:
		-index 0: point 1
		-index 1: point 2
		*/
		extern dx_11_buffer create_line(float x1, float y1, float x2, float y2, float z, color color_info[2], dx_11_renderer dx_renderer);

		/*
		color_info:
		-index 0: point 1
		-index 1: point 2
		*/
		extern dx_11_buffer create_circle(float x1, float y1, float z, float radius, CIRCLE_MODE circle_mode, color color_info[2], dx_11_renderer dx_renderer);
	}
}