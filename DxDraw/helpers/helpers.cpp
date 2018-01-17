#include "helpers.h"
#include "..\defines.h"

namespace helpers
{
	void str_println(const char* str, ...)
	{
		//just something large so that we dont accidentally overflow the buffer
		char buf[0xffff];
		va_list vl;
		va_start(vl, str);
		vsprintf_s(buf, str, vl);
		va_end(vl);
		char buf_ret[0xffff];
		sprintf_s(buf_ret, "%s\n", buf);
		printf(buf_ret);
	}

	void change_color(WORD w_color)
	{
		//get a handle to the console
		HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
		//set the color
		SetConsoleTextAttribute(hCon, w_color);
		//i guess i shouldn't closehandle to hCon
		//CloseHandle(hCon);
	}

	void err_print(error err, const char* str_err, ...)
	{
		//array of warning strings
		const char* str_errs[4] = { "INFO", "WARNING", "ERROR", "CRITICAL ERROR" };
		const WORD w_errs[4] = { FOREGROUND_BRIGHT_GREEN, FOREGROUND_RED | FOREGROUND_GREEN, FOREGROUND_BRIGHT_RED, FOREGROUND_BRIGHT_RED | FOREGROUND_BRIGHT_BLUE };
		//change print color

		change_color(FOREGROUND_WHITE);
		printf("[");
		change_color(w_errs[err]);
		printf("%s", str_errs[err]);
		//change back print color
		change_color(FOREGROUND_WHITE);

		char buf[0xffff];
		va_list vl;
		va_start(vl, str_err);
		vsprintf_s(buf, str_err, vl);
		va_end(vl);

		printf("] %s\n", buf);
	}

	BOOL create_window(HINSTANCE hInst, int nCmd, LPCSTR str_classname, LPCSTR str_windowname, int width, int height, HWND *hWnd_out, WNDPROC wnd_proc)
	{
		//initialize hwnd and wndclassex
		HWND hWnd;
		WNDCLASSEX wcex;
		ZeroMemory(&wcex, sizeof(WNDCLASSEX));
		wcex.cbSize = sizeof(wcex);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = wnd_proc;
		wcex.hInstance = hInst;
		wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wcex.lpszClassName = str_classname;
		wcex.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

		//register the wndclass
		if (!RegisterClassEx(&wcex))
			return FALSE;

		//create the window
		hWnd = CreateWindowEx(NULL, str_classname, str_windowname, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, (GetSystemMetrics(SM_CXSCREEN) - width) / 2,
			(GetSystemMetrics(SM_CYSCREEN) - height) / 2, width, height, NULL, NULL, hInst, NULL);

		ShowWindow(hWnd, nCmd);
		*hWnd_out = hWnd;
		return TRUE;
	}

	void last_error()
	{
		printf("Last Error: ");
		change_color(FOREGROUND_RED);
		printf("0x%X\n", GetLastError());
	}
}