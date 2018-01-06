#pragma once
#include <cstdio>
#include <Windows.h>

#define FOREGROUND_BRIGHT_RED FOREGROUND_RED | FOREGROUND_INTENSITY
#define FOREGROUND_BRIGHT_GREEN FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define FOREGROUND_BRIGHT_BLUE FOREGROUND_BLUE | FOREGROUND_INTENSITY
#define FOREGROUND_WHITE FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
#define FOREGROUND_BRIGHT_WHITE FOREGROUND_WHITE | FOREGROUND_INTENSITY

namespace helpers
{
	//print a string then continue to the next line
	void str_println(const char* str, ...);

	//different types of errors
	enum error : int
	{
		ERR_INFO = 0,
		ERR_WARNING = 1,
		ERR_ERROR = 2,
		ERR_CRITICAL = 3
	};

	//change the output color of the console
	void change_color(WORD w_color);
	
	//print an error
	void err_print(const char* str_err, error err, ...);

	//create a window to output to
	BOOL create_window(HINSTANCE hInst, int nCmd, LPCSTR str_classname, LPCSTR str_windowname, int width, int height, HWND *hWnd_out, WNDPROC wnd_proc);

	void last_error();
}