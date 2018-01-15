#include "wrapper.h"

namespace wrapper
{
	namespace D3D11
	{
		dx_11_buffer create_square(float x, float y, float width, float height, float z,  color color_info[4], dx_11_renderer dx_renderer)
		{
			dx_vertex vertices[6] =
			{
				{ x, y + height, z, color_info[0].r, color_info[0].g, color_info[0].b, color_info[0].a },
				{ x + width, y, z, color_info[2].r, color_info[2].g, color_info[2].b, color_info[2].a },
				{ x, y, z, color_info[3].r, color_info[3].g, color_info[3].b, color_info[3].a },
				{ x, y + height, z, color_info[0].r, color_info[0].g, color_info[0].b, color_info[0].a },
				{ x + width, y + height, z, color_info[1].r, color_info[1].g, color_info[1].b, color_info[1].a },
				{ x + width, y, z, color_info[2].r, color_info[2].g, color_info[2].b, color_info[2].a }
			};

			return dx_renderer.create_render_item(vertices, sizeof(vertices), 6, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}

		dx_11_buffer create_triangle(float x1, float y1, float x2, float y2, float x3, float y3, float z, color color_info[3], dx_11_renderer dx_renderer)
		{
			dx_vertex vertices[3] = 
			{
				{ x1, y1, z, color_info[0].r, color_info[0].g, color_info[0].b, color_info[0].a },
				{ x2, y2, z, color_info[1].r, color_info[1].g, color_info[1].b, color_info[1].a },
				{ x3, y3, z, color_info[2].r, color_info[2].g, color_info[2].b, color_info[2].a },
			};

			return dx_renderer.create_render_item(vertices, sizeof(vertices), 3, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}

		dx_11_buffer create_line(float x1, float y1, float x2, float y2, float z, color color_info[2], dx_11_renderer dx_renderer)
		{
			dx_vertex vertices[2] =
			{
				{ x1, y1, z, color_info[0].r, color_info[0].g, color_info[0].b, color_info[0].a },
				{ x2, y2, z, color_info[1].r, color_info[1].g, color_info[1].b, color_info[1].a }
			};

			return dx_renderer.create_render_item(vertices, sizeof(vertices), 2, D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		}

		dx_11_buffer create_circle(float x1, float y1, float z, float radius, CIRCLE_MODE circle_mode, color color_info[2], dx_11_renderer dx_renderer)
		{
			//amount of points we need to render
			const float points = CIRCLE_SEGMENTS * 3;

			//vector containing points
			dx_vertex circle[CIRCLE_SEGMENTS * 3];

			//pi  value that we will use
			const float pi = 3.14159265358979323;

			for (int i = 0; i < points; i++)
				circle[i] = { x1 - radius * cos(pi * ((i - 1) / (CIRCLE_SEGMENTS / 2.0f))), y1 - radius * sin(pi * ((i - 1) / (CIRCLE_SEGMENTS / 2.0f))), z, color_info[0].r, color_info[0].g, color_info[0].b, color_info[0].a };//this->CreateVertex(point.x - Radius * cos(flPi * ((i - 1) / (Segments / 2.0f))), point.y - Radius * sin(flPi * ((i - 1) / (Segments / 2.0f))), point.z, 1.f, color, 0.f, 0.f);
			
			//not "circle != CIRCLE_LINE" for future compatability
			if (circle_mode == CIRCLE_SOLID || circle_mode == CIRCLE_WEIRD)
				for (int i = 0; i < points; i += 3)
					circle[i] = { x1, y1, z, color_info[1].r, color_info[1].g, color_info[1].b, color_info[1].a };

			return dx_renderer.create_render_item(circle, sizeof(circle), points, ((circle_mode == CIRCLE_SOLID) ? D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP : D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP)/*D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP*//*D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP*/);
		}
	}
}