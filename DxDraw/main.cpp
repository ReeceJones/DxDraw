
#define WIN32_LEAN_AND_MEAN

#include "helpers\helpers.h"
#include "DirectX11\Dx11Renderer.h"
#include "defines.h"
#include "wrapper\wrapper.h"
#include "tools\fps_calculation.h"

dx_11_renderer dx11;
fps_calculator* fps = new fps_calculator();

//dx_vertex tri_verts[] =
//{
//	/*x, y, z, r, g, b, a*/
//	{-1, 0, 1.f, 1, 0, 0, 1.f },
//	{ -0.75f, 1, 1.f, 0, 1, 0, 1.f },
//	{ -0.5f, 0, 1.f, 0, 0, 1, 1.f },
//};
//
///*use in mulitples of 3; d3d11 renders using triangles, ie: square made of 2 triangles*/
////TODO: point rendering
//dx_vertex hex_verts[6] =
//{
//	{ -0.5f, 0.5f, 1.f, 0.f, 0.f },
//	{ 0.5f, -0.5f, 0.f, 0.f, 1.f },
//	{ -0.5f, -0.5f, 0.f, 0.f, 0.f },
//	{ -0.5f, 0.5f, 1.f, 0.f, 0.f },
//	{ 0.5f, 0.5f, 0.f, 1.f, 0.f },
//	{ 0.5f, -0.5f, 0.f, 0.f, 1.f }
//};

wrapper::color color_info_sq[4] =
{
	{ 1.f, 0.f, 0.f, 1.f },
	{ 0.f, 1.f, 0.f, 1.f },
	{ 0.f, 0.f, 1.f, 1.f },
	{ 0.f, 0.f, 0.f, 1.f }
};

wrapper::color color_info_tri[3] =
{
	{ 1.f, 0.f, 0.f, 1.f },
	{ 0.f, 1.f, 0.f, 1.f },
	{ 0.f, 0.f, 1.f, 1.f }
};

wrapper::color color_info_duo[2] = 
{
	{1.f, 0.f, 0.f, 1.f},
	{0.f, 1.f, 0.f, 1.f}
};

VOID draw()
{
	//create the individual buffers
	dx_11_buffer dx_sq_buf = wrapper::D3D11::create_square(500.f, 500.f, 100.f, 100.f, 1.f, color_info_sq, dx11);//dx11.create_render_item(hex_verts, sizeof(hex_verts), 6);
	dx_11_buffer dx_tri_buf = wrapper::D3D11::create_triangle(0.f, 200.f, 300.f, 0.f, 600.f, 200.f, 1.f, color_info_tri, dx11); //dx11.create_render_item(tri_verts, sizeof(tri_verts), 3);
	dx_11_buffer dx_ln_buf = wrapper::D3D11::create_line(100.f, 700.f, 1000.f, 700.f, 1.f, color_info_duo, dx11);
	dx_11_buffer dx_crc_buf = wrapper::D3D11::create_circle(700.f, 500.f, 1.f, 50.f, CIRCLE_SOLID, color_info_duo, dx11);
	
	//draw each individual buffer
	dx11.draw_render_item(dx_tri_buf);
	dx11.draw_render_item(dx_sq_buf);
	dx11.draw_render_item(dx_ln_buf);
	dx11.draw_render_item(dx_crc_buf);

	//clear the render buffers of the created items, do this so we don't have a memory leak
	dx11.clear_render_buffer(dx_sq_buf);
	dx11.clear_render_buffer(dx_tri_buf);
	dx11.clear_render_buffer(dx_ln_buf);
	dx11.clear_render_buffer(dx_crc_buf);
}

//wndproc for stuffz
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) //window proc function
{
	switch (message)
	{
	case WM_DESTROY: //if the message sent is a message to destroy the window
		DestroyWindow(hWnd); //destroy the window
		PostQuitMessage(NULL); //stops the program
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

//entrypoint
//pretty much just winmain without being winmain, same control structure for the most part
int main()
{
	//helpers::err_print("This is info", helpers::ERR_INFO);
	//helpers::err_print("This is a warning", helpers::ERR_WARNING);
	//helpers::err_print("This is an error", helpers::ERR_ERROR);
	//helpers::err_print("This is a critical error", helpers::ERR_CRITICAL);

	HINSTANCE hInst = GetModuleHandle(NULL);
	HWND out_hwnd;
	if (helpers::create_window(hInst, SW_SHOWNORMAL /*nCmd's: https://msdn.microsoft.com/en-us/library/windows/desktop/ms633559(v=vs.85).aspx */, "some classname", "some windowname", WINDOW_WIDTH, WINDOW_HEIGHT, &out_hwnd, WndProc))
	{
		MSG msg; //declare the message
		ZeroMemory(&msg, sizeof(MSG)); //initialize the msg
		//initialize our direct3d helper
		if (dx11.initialize(out_hwnd, draw))
		{
			fps->enable_fps_logging();
			fps->reset_timer();
			while (msg.message != WM_QUIT) //make sure that the application is not quit yets
			{
				while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) //get the message
				{
					TranslateMessage(&msg); //catch the messages
					DispatchMessage(&msg);
				}
				//draw here
				//begin and end frame
				dx11.draw_loop();
				fps->update_frame();
				//printf("frames: %i\n", fps.get_fps());
			}
		}
	}

	//having to press a key is annoying and too much work. I didn't type this project, I used my brain and it just appeared out of nowhere
	#ifdef _DEBUG 
		system("pause");
	#endif
	return EXIT_SUCCESS;
}