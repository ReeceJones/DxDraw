#include "Dx11Renderer.h"
#include <d3d11.h>
#include "..\helpers\helpers.h"
#include "..\defines.h"
#include <d3dcompiler.h>

dx_11_renderer::dx_11_renderer()
{
	//do nothing
}

dx_11_renderer::~dx_11_renderer()
{
	ptr_d3d11device->Release();
	ptr_swapchain->Release();
	ptr_d3d11devicecontext->Release();
	ptr_rendertarget->Release();
	ptr_vertexshader->Release();
	ptr_pixelshader->Release();
	ptr_inputlayout->Release();
	ptr_rasterizerstate->Release();
	ptr_blendstate->Release();
	ptr_depthstencilstate->Release();
}

BOOL dx_11_renderer::initialize(HWND hWnd, FRAMEPROC fn_Draw)
{
	//create swap chain
	DXGI_SWAP_CHAIN_DESC dxgi_swap_chain_desc = { 0 };
	dxgi_swap_chain_desc.BufferCount = 1;
	dxgi_swap_chain_desc.Windowed = TRUE;
	dxgi_swap_chain_desc.OutputWindow = hWnd;
	dxgi_swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgi_swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgi_swap_chain_desc.SampleDesc.Count = MULTISAMPLE_LEVEL;
	dxgi_swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	HRESULT hRes = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, NULL, nullptr,
		NULL, D3D11_SDK_VERSION, &dxgi_swap_chain_desc, &this->ptr_swapchain, &this->ptr_d3d11device,
		nullptr, &this->ptr_d3d11devicecontext);
	//check if we successfully created the device and swap chain
	if (FAILED(hRes))
	{
		helpers::err_print(helpers::ERR_ERROR, "Could not create D3D11Device and DXGI_SWAP_CHAIN_DESC!");
		return FALSE;
	}

	helpers::err_print(helpers::ERR_INFO, "Successfully created D3D11Device and DXGI_SWAP_CHAIN_DESC!");
	
	//create the render target
	ID3D11Texture2D* textbuf;
	this->ptr_swapchain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), (VOID**)&textbuf);
	this->ptr_d3d11device->CreateRenderTargetView(textbuf, nullptr, &this->ptr_rendertarget);
	
	//make sure that we are not releasing a null ptr, can cause exception
	
	if (textbuf != nullptr)
	{
		textbuf->GetDesc(&this->ptr_textdesc);
		textbuf->Release();
	}
	else
	{
		helpers::err_print(helpers::ERR_ERROR, "null texture buffer!");
		return FALSE;
	}

	helpers::err_print(helpers::ERR_INFO, "Successfully creatde Render Target!");

	//use this to make it a bit easier to control framez
	this->draw_frame = fn_Draw;


	//compile shaders at runtime

	//we store our shader in a c string
	//this is the vertex shader
	const char vertex_shader[] =
	"struct Input"
	"{"
	"	float3 position : POSITION;"
	"	float4 color : COLOR;"
	"};"
	"struct Output"
	"{"
	"	float4 position : SV_POSITION;"
	"	float4 color : COLOR;"
	"};"
	"Output main(Input input)"
	"{"
	"	Output output;"
	"	output.position = float4(((input.position.x - (%f / 2)) / (%f / 2)), -((input.position.y - (%f / 2)) / (%f / 2)), input.position.z, 1);"
	"	output.color = input.color;"
	"	return output;"
	"}";
	/*
	now, because d3d11 is obnoxious we have to convert some coordinates
	to convert a coordinate to a point(?) (uses left-hand coordinate system)
		-Because the coordinate is in relation to the top left we:
			>dimension = height or width
			>subtract half of the dimension, then divide that by half the dimension
			>for the y coordinate you have to take the opposite of the value so that it is the correct dimension
	*/
	//WE CAN FINALLY USE COORDINATES. WE DID IT
	//downside though: if resolution changes we have to recompile shaders or else it breaks...TODO: work
	char vertex_buf[0xffff];
	sprintf_s(vertex_buf, vertex_shader, (FLOAT)WINDOW_WIDTH, (FLOAT)WINDOW_WIDTH, (FLOAT)WINDOW_HEIGHT, (FLOAT)WINDOW_HEIGHT);

	//initialize some blobs
	ID3D10Blob* ptr_blob;
	ID3D10Blob* ptr_error;

	//compile the vertex shader from memory
	if (FAILED(D3DCompile((DWORD*)vertex_buf, sizeof(vertex_buf), "vertex_shader", NULL, NULL, "main", "vs_5_0", NULL, NULL, &ptr_blob, &ptr_error)))
	{
		//what went wrong?
		helpers::err_print(helpers::ERR_ERROR, "Could not compile vertex shader in memory!");
		helpers::last_error();
		return FALSE;
	}
	//todo done :D https://msdn.microsoft.com/en-us/library/windows/desktop/ff476262(v=vs.85).aspx
	//create the vertex shader
	if (FAILED(this->ptr_d3d11device->CreateVertexShader(/*vsData.data()*/ptr_blob->GetBufferPointer(), /*vsData.size()*/ptr_blob->GetBufferSize(), nullptr, &this->ptr_vertexshader)))
	{
		helpers::err_print(helpers::ERR_CRITICAL, "Could not create vertex shader!");
		ptr_blob->Release();
		return FALSE;
	}

	//create input layout while we are working on our shaders
	D3D11_INPUT_ELEMENT_DESC input_layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	if (FAILED(this->ptr_d3d11device->CreateInputLayout(input_layout, 2, /*vertex data*/ptr_blob->GetBufferPointer(), ptr_blob->GetBufferSize(), &this->ptr_inputlayout)))
	{
		helpers::err_print(helpers::ERR_ERROR, "Could not create input layout!");
		return FALSE;
	}

	helpers::err_print(helpers::ERR_INFO, "Successfully created input layout!");
	//clean up our blob for reuse

	//make sure that we don't release a nullptr, will cause exception
	if (ptr_blob != nullptr)
		ptr_blob->Release();

	if (ptr_error != nullptr)
		ptr_blob->Release();

	//make sure that we have completely cleaned up
	ZeroMemory(&ptr_blob, sizeof(ID3D10Blob));
	ZeroMemory(&ptr_error, sizeof(ID3D10Blob));

	//our pixel shader
	const char pixel_shader[] = 
		"struct Input"
		"{"
		"	float4 position : SV_POSITION;"
		"	float4 color : COLOR;"
		"};"
		"float4 main(Input input) : SV_TARGET"
		"{"
		"	return float4(input.color.r, input.color.g, input.color.b, input.color.a);"
		"}";
	//compile the pixel shader
	if (FAILED(D3DCompile((DWORD*)pixel_shader, sizeof(pixel_shader), "pixel_shader", NULL, NULL, "main", "ps_5_0", NULL, NULL, &ptr_blob, &ptr_error)))
	{
		helpers::err_print(helpers::ERR_ERROR, "Could not compile pixel shader in memory!");
		ptr_error->Release();
		helpers::last_error();
		return FALSE;
	}
	
	//create the pixel shader for use
	if (FAILED(this->ptr_d3d11device->CreatePixelShader(/*pixel data*//*psData.data()*/ptr_blob->GetBufferPointer(), /*data size*//*psData.size()*/ptr_blob->GetBufferSize(), nullptr, &this->ptr_pixelshader)))
	{
		helpers::err_print(helpers::ERR_CRITICAL, "Could not create pixel shader!");
		ptr_blob->Release();
		return FALSE;
	}

	//clean up
	if (ptr_blob != nullptr)
		ptr_blob->Release();

	if (ptr_error != nullptr)
		ptr_blob->Release();

	//make sure that we have completely cleaned up
	ZeroMemory(&ptr_blob, sizeof(ID3D10Blob));
	ZeroMemory(&ptr_error, sizeof(ID3D10Blob));

	helpers::err_print(helpers::ERR_INFO, "Successfully created shaders!");

	//create the input layout, used for communication with the shaders


	//create the rasterizer
	CD3D11_RASTERIZER_DESC rasterizer_desc = CD3D11_RASTERIZER_DESC(D3D11_FILL_SOLID, D3D11_CULL_NONE, 0, 0, 0, 0, 0, 0, MULTISAMPLE, ANTIALIAS);

	if (FAILED(this->ptr_d3d11device->CreateRasterizerState(&rasterizer_desc, &this->ptr_rasterizerstate)))
	{
		helpers::err_print(helpers::ERR_ERROR, "Could not create rasterizer state!");
		return FALSE;
	}

	helpers::err_print(helpers::ERR_INFO, "Successfully created raterizer state!");

	//create the blender
	CD3D11_BLEND_DESC blend_desc = CD3D11_BLEND_DESC(CD3D11_DEFAULT());

	if (FAILED(this->ptr_d3d11device->CreateBlendState(&blend_desc, &this->ptr_blendstate)))
	{
		helpers::err_print(helpers::ERR_ERROR, "Could not create blend state!");
		return FALSE;
	}

	helpers::err_print(helpers::ERR_INFO, "Successfully created blend state!");

	//create the depthstencil, can be used for better detail on certain things
	D3D11_DEPTH_STENCIL_DESC depthstencil_desc;
	depthstencil_desc.DepthEnable = DEPTH_ENABLE;
	depthstencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthstencil_desc.DepthFunc = D3D11_COMPARISON_LESS;
	depthstencil_desc.StencilEnable = FALSE;
	depthstencil_desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depthstencil_desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	depthstencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthstencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthstencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthstencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthstencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthstencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthstencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthstencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	if (FAILED(this->ptr_d3d11device->CreateDepthStencilState(&depthstencil_desc, &this->ptr_depthstencilstate)))
	{
		helpers::err_print(helpers::ERR_ERROR, "Could not create depth stencil state!");
		return FALSE;
	}

	helpers::err_print(helpers::ERR_INFO, "Successfully created depth stencil state!");

	D3D11_TEXTURE2D_DESC texture2d_desc;
	ZeroMemory(&texture2d_desc, sizeof(texture2d_desc));
	texture2d_desc.Width = this->ptr_textdesc.Width;
	texture2d_desc.Height = this->ptr_textdesc.Height;
	texture2d_desc.MipLevels = 1;
	texture2d_desc.ArraySize = 1;
	texture2d_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texture2d_desc.SampleDesc.Count = MULTISAMPLE_LEVEL;
	texture2d_desc.SampleDesc.Quality = 0;
	texture2d_desc.Usage = D3D11_USAGE_DEFAULT;
	texture2d_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texture2d_desc.CPUAccessFlags = 0;
	texture2d_desc.MiscFlags = 0;

	if (FAILED(this->ptr_d3d11device->CreateTexture2D(&texture2d_desc, NULL, &this->ptr_texture2d)))
	{
		helpers::err_print(helpers::ERR_ERROR, "Could not create texture2d desc!");
		return FALSE;
	}

	helpers::err_print(helpers::ERR_INFO, "Successfully created texture2d desc!");

	D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc;
	ZeroMemory(&depth_stencil_view_desc, sizeof(depth_stencil_view_desc));
	depth_stencil_view_desc.Format = texture2d_desc.Format;
	depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	depth_stencil_view_desc.Texture2D.MipSlice = 0;
	

	if (FAILED(this->ptr_d3d11device->CreateDepthStencilView(this->ptr_texture2d, &depth_stencil_view_desc, &this->ptr_depthstencilview)))
	{
			helpers::err_print(helpers::ERR_ERROR, "Could not create texture2d desc!");
			return FALSE;
	}

	//it worked!!!
	helpers::err_print(helpers::ERR_INFO, "Successfully initialized class: dx_11_renderer!");
	return TRUE;
}

VOID dx_11_renderer::draw_loop()
{
	this->begin_frame();
	this->draw_frame();
	this->end_frame();
}

VOID dx_11_renderer::begin_frame()
{
	//set render target
	this->ptr_d3d11devicecontext->OMSetRenderTargets(1, &this->ptr_rendertarget, this->ptr_depthstencilview);
	//set viewport
	D3D11_VIEWPORT d3d11_viewport;
	d3d11_viewport.MinDepth = 0.f;
	d3d11_viewport.MaxDepth = 1.f;
	d3d11_viewport.TopLeftX = 0.f;
	d3d11_viewport.TopLeftY = 0.f;
	d3d11_viewport.Width = (FLOAT)this->ptr_textdesc.Width;
	d3d11_viewport.Height = (FLOAT)this->ptr_textdesc.Height;

	this->ptr_d3d11devicecontext->RSSetViewports(1, &d3d11_viewport);

	//clear screen with the backbuffer, and color it
	const float rgba[4] = { 0.2f, 0.2f, 0.2f, 1.f };
	this->ptr_d3d11devicecontext->ClearRenderTargetView(this->ptr_rendertarget, rgba);
	this->ptr_d3d11devicecontext->ClearDepthStencilView(this->ptr_depthstencilview, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);


	/*MOVED FROM PER-SHAPE DRAW*/
	//set render states to make sure that shapes are rendered correctly
	this->ptr_d3d11devicecontext->RSSetState(this->ptr_rasterizerstate);
	this->ptr_d3d11devicecontext->OMSetBlendState(this->ptr_blendstate, NULL, 0xffffffff);
	this->ptr_d3d11devicecontext->OMSetDepthStencilState(this->ptr_depthstencilstate, 1);

	this->ptr_d3d11devicecontext->IASetInputLayout(this->ptr_inputlayout);
	this->ptr_d3d11devicecontext->VSSetShader(this->ptr_vertexshader, nullptr, 0);
	this->ptr_d3d11devicecontext->PSSetShader(this->ptr_pixelshader, nullptr, 0);
}

VOID dx_11_renderer::end_frame()
{
	//present the frame
	//use flags if you are using vsync or you are doing something like variable refresh rate displays
	this->ptr_swapchain->Present(VSYNC_NFRAME /*vsync every nth frame*/, 0 /*flags: https://msdn.microsoft.com/en-us/library/windows/desktop/bb509554(v=vs.85).aspx */);
}

ID3D11Device* dx_11_renderer::get_ptr_d3d11device()
{
	return this->ptr_d3d11device;
}

ID3D11DeviceContext* dx_11_renderer::get_ptr_d3d11devicecontext()
{
	return this->ptr_d3d11devicecontext;
}

dx_11_buffer dx_11_renderer::create_render_item(dx_vertex* verts, size_t vert_size, size_t num_points, D3D11_PRIMITIVE_TOPOLOGY topology)
{
	//create a vertex buffer with the vertices given to us
	CD3D11_BUFFER_DESC vertex_buffer_desc = CD3D11_BUFFER_DESC((UINT)vert_size, D3D11_BIND_VERTEX_BUFFER);
	D3D11_SUBRESOURCE_DATA vert_data = { 0 };
	vert_data.pSysMem = verts;

	//create the buffer
	dx_11_buffer dx_buffer;
	this->ptr_d3d11device->CreateBuffer(&vertex_buffer_desc, &vert_data, &dx_buffer.ptr_buffer);

	//used for rendering the item
	dx_buffer.points = num_points;
	dx_buffer.topology_type = topology;

	return dx_buffer;
}

VOID dx_11_renderer::draw_render_item(dx_11_buffer dx_buffer)
{
	//set the vertex buffer so we can draw it
	UINT stride = sizeof(dx_vertex);
	UINT offset = 0;
	this->ptr_d3d11devicecontext->IASetVertexBuffers(0, 1, &dx_buffer.ptr_buffer, &stride, &offset);
	
	//type of topology so that we know how to draw shape
	this->ptr_d3d11devicecontext->IASetPrimitiveTopology(dx_buffer.topology_type);

	//draw the buffer
	this->ptr_d3d11devicecontext->Draw((UINT)dx_buffer.points, 0);
	//clear our buffer, for the next item
}

VOID dx_11_renderer::clear_render_buffer(dx_11_buffer dx_buffer)
{
	//release the buffer
	dx_buffer.ptr_buffer->Release();
	//clear memory of the buffer
	ZeroMemory(&dx_buffer.ptr_buffer, sizeof(ID3D11Buffer));
}