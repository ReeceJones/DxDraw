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

			return dx_renderer.create_render_item(vertices, sizeof(vertices), 6);
		}

		dx_11_buffer create_triangle(float x1, float y1, float x2, float y2, float x3, float y3, float z, color color_info[3], dx_11_renderer dx_renderer)
		{
			dx_vertex vertices[3] = 
			{
				{ x1, y1, z, color_info[0].r, color_info[0].g, color_info[0].b, color_info[0].a },
				{ x2, y2, z, color_info[1].r, color_info[1].g, color_info[1].b, color_info[1].a },
				{ x3, y3, z, color_info[2].r, color_info[2].g, color_info[2].b, color_info[2].a },
			};

			return dx_renderer.create_render_item(vertices, sizeof(vertices), 3);
		}

		dx_11_buffer create_line(float x1, float y1, float x2, float y2, float z, float width, color color_info[2], dx_11_renderer dx_renderer)
		{
			dx_vertex vertices[6] =
			{
				{ x1, y1, z, color_info[0].r, color_info[0].g, color_info[0].b, color_info[0].a },
				{ x2, y2, z, color_info[1].r, color_info[1].g, color_info[1].b, color_info[1].a },
				{ x2, y2 + width, z, color_info[1].r, color_info[1].g, color_info[1].b, color_info[1].a },

				{ x2, y2 + width, z, color_info[1].r, color_info[1].g, color_info[1].b, color_info[1].a },
				{ x1, y1 + width, z, color_info[0].r, color_info[0].g, color_info[0].b, color_info[0].a },
				{ x1, y1, z, color_info[0].r, color_info[0].g, color_info[0].b, color_info[0].a }
			};

			return dx_renderer.create_render_item(vertices, sizeof(vertices), 6);
		}

	}
}