#include "helpers\helpers.h"
#include "DirectX11\Dx11Renderer.h"
#include "defines.h"
#include "wrapper\wrapper.h"

dx_11_renderer dx11;

dx_vertex tri_verts[] =
{
	/*x, y, z, r, g, b, a*/
	{-1, 0, 1.f, 1, 0, 0, 1.f },
	{ -0.75f, 1, 1.f, 0, 1, 0, 1.f },
	{ -0.5f, 0, 1.f, 0, 0, 1, 1.f },
};

/*use in mulitples of 3; d3d11 renders using triangles, ie: square made of 2 triangles*/
//TODO: point rendering
dx_vertex hex_verts[6] =
{
	{ -0.5f, 0.5f, 1.f, 0.f, 0.f },
	{ 0.5f, -0.5f, 0.f, 0.f, 1.f },
	{ -0.5f, -0.5f, 0.f, 0.f, 0.f },
	{ -0.5f, 0.5f, 1.f, 0.f, 0.f },
	{ 0.5f, 0.5f, 0.f, 1.f, 0.f },
	{ 0.5f, -0.5f, 0.f, 0.f, 1.f }
};

wrapper::color color_info[4] =
{
	{ 1.f, 0.f, 0.f, 1.f },
	{ 0.f, 1.f, 0.f, 1.f },
	{ 0.f, 0.f, 1.f, 1.f },
	{ 0.f, 0.f, 0.f, 1.f }
};

VOID draw()
{
	//create the individual buffers

	dx_11_buffer dx_sq_buf = wrapper::D3D11::create_square(-0.5f, -0.5f, 1.f, 1.f, 1.f, color_info, dx11);//dx11.create_render_item(hex_verts, sizeof(hex_verts), 6);
	dx_11_buffer dx_tr_buf = dx11.create_render_item(tri_verts, sizeof(tri_verts), 3);
	
	//draw each individual buffer
	dx11.draw_render_item(dx_tr_buf);
	dx11.draw_render_item(dx_sq_buf);
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
	if (helpers::create_window(hInst, SW_SHOWNORMAL /*nCmd: https://msdn.microsoft.com/en-us/library/windows/desktop/ms633559(v=vs.85).aspx */, "some classname", "some windowname", WINDOW_WIDTH, WINDOW_HEIGHT, &out_hwnd, WndProc))
	{
		MSG msg; //declare the message
		ZeroMemory(&msg, sizeof(MSG)); //initialize the msg
		//initialize our direct3d helper
		if (dx11.initialize(out_hwnd, draw))
		{
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
			}
		}
	}
	system("pause");

	return EXIT_SUCCESS;
}