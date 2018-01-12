#pragma once
#include <Windows.h>
#include <d3d11.h>

//for calling begin and end frame by wrapper
typedef VOID(__stdcall* FRAMEPROC)(void);

struct dx_vertex
{
	//add z, will require some messing around with vertex and pixel shader
	float x, y, z, r, g, b, a;
};

struct dx_11_buffer
{
	ID3D11Buffer* ptr_buffer;
	size_t points;
};

//
class dx_11_renderer
{
public:
	//constructor
	dx_11_renderer();

	//initialize the renderer class
	BOOL initialize(HWND hWnd, FRAMEPROC fn_draw);

	//called in main to output to application
	VOID draw_loop();

	//get our d3d11device
	ID3D11Device* get_ptr_d3d11device();

	//get our d3d11devicecontext
	ID3D11DeviceContext* get_ptr_d3d11devicecontext();

	//create an item to be rendered, in order to render it call draw_render_item
	dx_11_buffer create_render_item(dx_vertex* verts, size_t vert_size, size_t num_points);

	//draw a shape
	VOID draw_render_item(dx_11_buffer dx_buffer);

	//clear rendering buffer in the case that we do not want to render an item
	VOID clear_render_buffer(dx_11_buffer dx_buffer);

private:
	FRAMEPROC draw_frame;

	//for dx11 specific functions
	//begin rendering a frame
	//begin frame
	VOID begin_frame();
	//end frame
	VOID end_frame();

	//dx11 interfaces
	ID3D11Device* ptr_d3d11device;
	IDXGISwapChain* ptr_swapchain;
	ID3D11DeviceContext* ptr_d3d11devicecontext;
	ID3D11RenderTargetView* ptr_rendertarget;
	D3D11_TEXTURE2D_DESC ptr_textdesc;

	//buffer items
	//can later be repurposed to store multiple buffers to reduce abstractions with wrapper and increase speed a tiny amount
	//ID3D11Buffer* ptr_buffer;
	//size_t points;

	//for rendering the items
	ID3D11VertexShader* ptr_vertexshader;
	ID3D11PixelShader* ptr_pixelshader;
	ID3D11InputLayout* ptr_inputlayout;
	ID3D11RasterizerState* ptr_rasterizerstate;
	ID3D11BlendState* ptr_blendstate;
	ID3D11DepthStencilState* ptr_depthstencilstate;
};