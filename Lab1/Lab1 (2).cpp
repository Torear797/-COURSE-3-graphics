#include <Windows.h>
#include <WindowsX.h>
#include <CommCtrl.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h> 
#include <random>  
#include <process.h>
#include "psapi.h"
#include "strsafe.h"
#include "gdiplus.h"
#pragma comment (lib, "GdiPlus.lib")
MSG msg;
HWND hwnd = NULL;
BOOL bRet;
HWND hDlg = NULL;
HWND hFindDlg = NULL;
HACCEL hAccel = NULL;
#define IDR_MENU1  101
#define IDR_ACCELERATOR1  2020

ULONG_PTR token;


LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL PreTranslateMessage(LPMSG lpMsg);
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
using namespace Gdiplus;
//void WINAPI GdiplusDebugProc(DebugEventLevel level, CHAR*message)
//{
//switch (level)
//{
//case DebugEventLevelFatal:
//MessageBoxA(NULL, message, "Критическая ошибка", MB_OK | MB_ICONERROR | MB_TASKMODAL); 
//break; 
//case DebugEventLevelWarning:MessageBoxA(NULL, message, "Предупреждение", MB_OK | MB_ICONWARNING | MB_TASKMODAL); 
//break; 
//} 
//} 
void OnPaint(HDC hdc)
{
	Graphics g(hdc);
	g.SetSmoothingMode(SmoothingModeHighQuality);
	g.Clear(Color::White);

	

	GraphicsPath path;
//	path.AddEllipse(rect);
	//path.AddEllipse(Tractor);

	Pen Contur(Color(255, 0, 0, 0), 5.f);
	Rect rect(50, 300, 280, 280);
	g.DrawEllipse(&Contur, rect);

	rect.X = 115;
	rect.Y = 365;
	rect.Width = 150;
	rect.Height = 150;
	g.DrawEllipse(&Contur, rect);

	//Pen pen2(Color(255, 0, 128, 0), 20.f);
//	HatchBrush hatchBrush(HatchStyleCross, Color::Red, Color(100, 255, 0, 0));
	//PathGradientBrush pthGrBrush(&path);
	//pthGrBrush.SetCenterColor(Color::Aqua);
	//Color color = Color::Blue;
	//int n = 1;
	//pthGrBrush.SetSurroundColors(&color, &n);
	//g.FillPath(&pthGrBrush, &path);
	//g.FillRectangle(&hatchBrush, 30, 10, 50, 120);
	
	//g.FillEllipse(&pthGrBrush, Tractor);
	//g.DrawLine(&pen1, 150, 10, 150, 130);
	//g.DrawLine(&pen2, 180, 10, 180, 130);
}
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR lpszCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = WindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wcex.lpszClassName = TEXT("Windowslass");;
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (0 == RegisterClassEx(&wcex)) // регистрирует класс
	{
		MessageBox(NULL, TEXT("Не удалось зарегестрировать класс!"), TEXT("Ошибка"), MB_ICONEXCLAMATION | MB_OK);
		return -1;
	}

	LoadLibrary(TEXT("ComCtl32.dll"));

	hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Windowslass"), TEXT("GDI+ LAB1"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (NULL == hwnd)
	{
		MessageBox(NULL, TEXT("Не удалось создать окно!"), TEXT("Ошибка"), MB_ICONEXCLAMATION | MB_OK);
		return -1;
	}
	ShowWindow(hwnd, nCmdShow);

	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartupOutput gdiplusStartupOutput;

	//gdiplusStartupInput.DebugEventCallback = GdiplusDebugProc;
	//gdiplusStartupInput.SuppressBackgroundThread = TRUE;

	Status stret = GdiplusStartup(&token,&gdiplusStartupInput,NULL);
	if (stret == Ok) 
	{
		for (;;)
		{
			while (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
			{
				//OnIdle(hwnd);
			} // while
			bRet = GetMessage(&msg, NULL, 0, 0);

			if (bRet == -1)
			{
			}
			else if (FALSE == bRet)
			{
				break;
			}
			else if (!PreTranslateMessage(&msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		/*gdiplusStartupOutput.NotificationUnhook(token);*/
		GdiplusShutdown(token);
	}
	return (int)msg.wParam;
}

BOOL PreTranslateMessage(LPMSG lpMsg)
{
	BOOL bRet = TRUE;

	if (!TranslateAccelerator(hwnd, hAccel, lpMsg))
	{
		bRet = IsDialogMessage(hDlg, lpMsg);

		if (FALSE == bRet)
			bRet = IsDialogMessage(hFindDlg, lpMsg);
	}

	return bRet;
}

BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{

	return TRUE;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{
		HANDLE_MSG(hwnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hwnd, WM_COMMAND, OnCommand);
	case WM_CLOSE:
	{
		DestroyWindow(hwnd);
	}
	break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		OnPaint(hdc);
		EndPaint(hwnd, &ps);
	}
	break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{

	switch (id)
	{



	}
}

