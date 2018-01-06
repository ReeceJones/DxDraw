#include "wrapper.h"

namespace wrapper
{
	namespace D3D11
	{
		dx_11_buffer create_square(float x, float y, float width, float height, float z,  color color_info[4], dx_11_renderer dx_renderer)
		{
			dx_vertex hex_verts[6] =
			{
				{ -0.5f, 0.5f, z, color_info[0].r, color_info[0].g, color_info[0].b, color_info[0].a },
				{ 0.5f, -0.5f, z, color_info[2].r, color_info[2].g, color_info[2].b, color_info[2].a },
				{ -0.5f, -0.5f, z, color_info[3].r, color_info[3].g, color_info[3].b, color_info[3].a },
				{ -0.5f, 0.5f, z, color_info[0].r, color_info[0].g, color_info[0].b, color_info[0].a },
				{ 0.5f, 0.5f, z, color_info[1].r, color_info[1].g, color_info[1].b, color_info[1].a },
				{ 0.5f, -0.5f, z, color_info[2].r, color_info[2].g, color_info[2].b, color_info[2].a }
			};

			return dx_renderer.create_render_item(hex_verts, sizeof(hex_verts), 6);
		}
	}
}