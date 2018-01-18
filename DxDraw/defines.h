#pragma once
//common definitions for rendering and window creation

//api mode (DX11, DX10, DX9)
#define DX11

//window settings
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

//vsync applied to every nth frame
#define VSYNC_NFRAME 0

//rasterizer settings
#define MULTISAMPLE TRUE
#define ANTIALIAS FALSE
#define MULTISAMPLE_LEVEL 4
#define DEPTH_ENABLE 1

//rendering settings
//circle segments is the amount of sides that the circle is going to have when rendered
#define CIRCLE_SEGMENTS 50