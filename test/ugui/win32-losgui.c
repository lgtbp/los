/*----------------------------------------------------------------------------/
 /  los -  system module  R0.1
 /-----------------------------------------------------------------------------/
 /
 / Copyright (c) 2014-2017 LP电子,All Rights Reserved.
 /
 / 未经授权，禁止商用。
 /----------------------------------------------------------------------------*/

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <stdint.h>
#include <process.h>
#include "los.h"
#include "lcdDriver.h"

lgui_t *lgui, lgui_copy;

// Global variables
uint8_t flg = 0;
uint16_t win_width = 260;
uint16_t win_height = 340;
COLORREF *lcd_buf;
// The main window class name.
static TCHAR szWindowClass[] = _T("win32app");

static TCHAR szTitle[] = _T("Win los");
extern void win_time_init();

//buf_path当前路径
//len代码空间
uint8_t *read_file_bin(char *buf_path, uint32_t *len)
{
	FILE *pF;
	uint8_t *buf;
	pF = fopen(buf_path, "rb");
	if (NULL == pF)
	{
		fprintf(stderr, "open bin ERR= %s\r\n", buf_path);
		len[0] = 0;
		return NULL;
	}
	fseek(pF, 0, SEEK_END);
	len[0] = ftell(pF);
	buf = (uint8_t *)malloc(len[0]);
	rewind(pF);
	fread(buf, 1, len[0], pF);
	fclose(pF);
	return buf;
}
extern int los_sprintf_low(char *buf, const char *fmt, uint8_t va_num);
extern void lcd_draw_point(uint16_t x, uint16_t y, uint32_t c);
void los_function(int f)
{
	uint8_t buf[256];
	if (f == 1) //printf
	{
		los_sprintf_low(buf, los_get_argp(0), 1);
		printf("%s", buf);
		return;
	}
	else if (f == 2) //get test
	{
		printf("printf [%d]\r\n", *los_get_arg(0));
		los_set_return(*los_get_arg(0) * 2);
		return;
	}
	else if (f == 3) //sellp
	{
		//printf("sleep [%d]\r\n", *(uint32_t *)los_get_arg(0));
		Sleep(*(uint32_t *)los_get_arg(0));
		return;
	}
	else if (f == 4) //xxxxx
	{
		los_set_return(lgui->point_x);
		return;
	}
	else if (f == 5) //yyyy
	{
		los_set_return(lgui->point_y);
		return;
	}
	else if (f == 6) //yyyy
	{
		lcd_draw_point(*(uint16_t *)los_get_arg(0), *(uint16_t *)los_get_arg(1), *(uint16_t *)los_get_arg(2));
		return;
	}
	else if (f == 7)
	{
		lcd_draw_line(*(uint16_t *)los_get_arg(0), *(uint16_t *)los_get_arg(1), *(uint16_t *)los_get_arg(2), *(uint16_t *)los_get_arg(3), *(uint16_t *)los_get_arg(4));
	}
	else if (f == 8)
	{
		lcd_draw_frect(*(uint16_t *)los_get_arg(0), *(uint16_t *)los_get_arg(1), *(uint16_t *)los_get_arg(2), *(uint16_t *)los_get_arg(3), *(uint16_t *)los_get_arg(4));
	}
	// printf("function call %d,%d,%d,%d\r\n", *(uint32_t *)los_get_arg(0), *(uint16_t *)los_get_arg(1), *(uint16_t *)los_get_arg(2), *(uint16_t *)los_get_arg(3));
}

uint32_t los_app_frist(uint8_t *pname)
{
	int res;
	uint32_t len;
	uint8_t *los;
	char path[128];

	GetCurrentDirectory(100, path);
	len = strlen(path);
	strcpy(&path[len], "\\test.los");

	los = read_file_bin(path, &len);
	if (los == 0)
		return 2;
	printf("path=%s,%d\r\n", path, len);
	los_set_function(los_function);
	res = los_app_first((uint32_t *)los, 1);
	printf("res:%d\n", res);
	return 0;
}
extern int comm_arg_deal(char *com, uint8_t ***buf);
char pname[128] = {0};
char papp[128] = {0};
DWORD WINAPI ThreadFunc(LPVOID);
DWORD WINAPI ThreadFunc(LPVOID p)
{
	int res = 0;
	char data[128];
	printf("los run\n");
	res = los_app_frist(pname);
	printf("los res=%x\n", res);
	sprintf(data, "run ret %x\n", res);
	MessageBox(NULL, data, L"Error", MB_OK);
	exit(1);
}
int CommandLineToArgvA(char *v, char *pNumArgs)
{
	uint8_t buf[128];
	int len = 0, ff = 0, flg = 0, i = 0;
	len = strlen(v);
	memset(buf, 0, 128);
	strcpy(buf, v);
	if (v[0] == '"')
	{
		i = 1;
		ff = 1;
	}
	for (; i < len; ++i)
	{
		if (ff) //是 “”
		{
			if (v[i] == '"')
			{
				memcpy(buf, &v[1], i - 1);
				break;
			}
		}
		else //这是空格
		{
			if (v[i] == ' ')
			{
				memcpy(buf, v, i);
				buf[i] = 0;
				break;
			}
		}
	}
	//get_pc_los_path(buf);
	uint8_t **bufs, num = 0;
	//num = comm_arg_deal(pNumArgs, &bufs);
	for (i = 0; i < num; ++i)
	{
		if (i == 0)
			strcpy(pname, bufs[0]);
		else if (i == 1)
			strcpy(papp, bufs[1]);
		free(bufs[i]);
	}

	return 0;
}

HANDLE Thread1;
extern uint8_t los_app_tick(void);
uint8_t irq_num = 0;
uint8_t los_app_tick(void)
{
	SuspendThread(Thread1);
	los_irq(irq_num++);
	ResumeThread(Thread1);
	return 0;
}
void func_tick(void *v)
{
	Sleep(10);
	while (1)
	{
		//los_app_tick();
		Sleep(1000);
	}
	exit(1);
}
extern void win_time_tick();
void win_time_tick()
{
}

void func_time(void *v)
{
	Sleep(10);
	while (1)
	{
		win_time_tick();
		Sleep(1000);
	}
	exit(1);
}
extern int ft_test(void);
// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HWND mhWnd;
void window_set_los_title(char *name)
{
	SetWindowText(mhWnd, name);
}
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
					 LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex;

	HINSTANCE hInst;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, _T("Call to RegisterClassEx failed!"),
				   _T("Win32 Guided Tour"),
				   NULL);

		return 1;
	}
	LPWSTR *szArgList;
	int argCount;

	szArgList = CommandLineToArgvW(GetCommandLine(), &argCount);
	if (szArgList == NULL)
	{
		MessageBox(NULL, L"Unable to parse command line", L"Error", MB_OK);
		return 10;
	}

	CommandLineToArgvA(szArgList[0], lpCmdLine); //get cmd
	hInst = hInstance;							 // Store instance handle in our global variable

	mhWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, //600,100,
		win_width, win_height,
		NULL,
		NULL,
		hInstance,
		NULL);
	if (!mhWnd)
	{
		MessageBox(NULL, _T("Call to CreateWindow failed!"),
				   _T("Win32 Guided Tour"),
				   NULL);

		return 1;
	}
	LocalFree(szArgList);
	//lgui=los_get_io();
	lgui = &lgui_copy;

	int scrWidth, scrHeight;
	RECT rect;
	lcd_buf = malloc(win_width * win_height * sizeof(COLORREF));
	if (lcd_buf == 0)
		return 1;
	memset(lcd_buf, 240, win_width * win_height * sizeof(COLORREF)); //clear

	scrWidth = GetSystemMetrics(SM_CXSCREEN);
	scrHeight = GetSystemMetrics(SM_CYSCREEN);
	GetWindowRect(mhWnd, &rect);
	SetWindowPos(mhWnd, HWND_TOPMOST, (scrWidth - rect.right) / 2, (scrHeight - rect.bottom) / 2, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);

	Thread1 = CreateThread(NULL, 0, ThreadFunc, NULL, 0, NULL);
	//_beginthread(fun1, 0, NULL);
	SetTimer(mhWnd, 1, 3, NULL);
	Sleep(1000);
	SetTimer(mhWnd, 2, 1000, NULL);
	_beginthread(func_time, 0, NULL);
	_beginthread(func_tick, 0, NULL);
	ShowWindow(mhWnd, nCmdShow);
	UpdateWindow(mhWnd);
	MSG msg; // Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}
uint16_t recx, recy, recw, rech;
void Draw(HDC hdc)
{
	int x, y;
	for (y = 0; y < win_height; ++y)
		for (x = 0; x < win_width; ++x)
		{
			SetPixel(hdc, x, y, lcd_buf[x + win_width * y]);
		}
	recx = win_width / 2 - 20;
	recy = win_height - 75;
	recw = recx + 40;
	rech = recy + 40;

	RoundRect(hdc, recx, recy, recw, rech, 5, 5);
	RoundRect(hdc, recx + 3, recy + 3, recw - 3, rech - 3, 5, 5);
}

int CaptureAnImage(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc_old = BeginPaint(hwnd, &ps);
	HDC hdc = CreateCompatibleDC(hdc_old);
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);
	HBITMAP hBmp = CreateCompatibleBitmap(hdc_old, clientRect.right,
										  clientRect.bottom);

	SelectObject(hdc, hBmp);
	SelectObject(hdc, GetSysColorBrush(COLOR_3DFACE));
	Rectangle(hdc, -1, -1, clientRect.right + 2, clientRect.bottom + 2);

	Draw(hdc);

	BitBlt(hdc_old, 0, 0, clientRect.right, clientRect.bottom, hdc, 0, 0,
		   SRCCOPY);
	DeleteObject(hBmp);
	DeleteDC(hdc);
	ReleaseDC(hwnd, hdc);
	EndPaint(hwnd, &ps);
	return 0;
}

uint32_t re_time = 0;
int8_t point_up = 0, sync = 0;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int nums = 0;
	switch (message)
	{
	case WM_CLOSE:
		printf((const char *)"app exit!\n");
		exit(1);
		break;
	case WM_TIMER:

		switch (wParam)
		{

		case 1: //ui refresh 50ms
				//RedrawWindow(hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE);
			// if (flg == 2)
			// {
			// 	re_time = 0;
			// 	RedrawWindow(hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE);
			// 	flg = 0;
			// }
			// else
			if (flg)
			{
				re_time++;
				if (re_time > 3) //10*10
				{
					re_time = 0;
					RedrawWindow(hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE);
					flg = 0;
				}
			}
			break;
		case 2:
			//los_app_tick();
			break;
		}
		break;
	case WM_CREATE:
	{
		break;
	}
	case WM_LBUTTONDOWN:
		lgui->point_x = LOWORD(lParam); //x
		lgui->point_y = HIWORD(lParam); //y
		lgui->point_flgs = 0;

		SuspendThread(Thread1);
		//Sleep(50);
		los_irq(0);
		ResumeThread(Thread1);
		nums = 0;
		while (lgui->point_flgs != -1)
		{
			Sleep(5);
			nums++;
			if (nums > 20)
				break;
		}
		if (lgui->point_x > recx && lgui->point_x < recw && lgui->point_y > recy && lgui->point_y < rech)
		{
			//los_app_min();
		}
		break;
	case WM_LBUTTONUP:
		Sleep(5);
		lgui->point_flgs = 1;
		SuspendThread(Thread1);
		//Sleep(50);
		los_irq(1);
		ResumeThread(Thread1);
		break;
	case WM_PAINT:
		CaptureAnImage(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:

		switch (wParam)
		{
		case VK_UP:
			lgui->key_key = VK_UP;
			break;
		case VK_DOWN:
			lgui->key_key = VK_DOWN;
			break;
		case VK_RETURN:
			lgui->key_key = VK_RETURN;
			break;
		}
		break;
	case WM_ERASEBKGND:
		return TRUE;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}
