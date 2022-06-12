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
#include "gdipluspath.h"
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
Image *image;
void OnPaint(HDC hdc)
{
	Graphics g(hdc);
	g.SetSmoothingMode(SmoothingModeHighQuality);
	g.Clear(Color::White);

	GraphicsPath path;

	path.AddEllipse(210, 360, 180, 180);
	path.AddEllipse(650, 430, 120, 120);
	PathGradientBrush pthGrBrush(&path);

	pthGrBrush.SetCenterColor(Color::Black);
	Color color = Color::Yellow;
	int n = 1;
	pthGrBrush.SetSurroundColors(&color, &n);//Кисть градиента контура

	HatchBrush hatchBrush(HatchStyleLargeConfetti, Color::Blue, Color(0, 0, 0, 0)); //Штриховая кисть
	
	SolidBrush solidBrush2(Color::LightGray);
	SolidBrush solidBrush(Color::Black);
	SolidBrush Material(Color::Green);
	SolidBrush Material2(Color::Red);

	Pen Contur(Color(255, 0, 0, 0), 5.f);
	
	Pen Contur2(Color::Black, 10.f);
	float comp[6] = {
		0.0f, 0.2f,
		0.3f, 0.7f,
		0.8f, 1.0f };
	Contur2.SetCompoundArray(comp, 6); //Составное перо

	Pen Contur3(Color(255, 158, 44, 0), 5.f);

	Point points[3] = {
		Point(530,120),
		Point(580,250),
		Point(525,300) 
	};
	Point points2[3] = {	
		Point(525,300),
		Point(525,250),
		Point(420,290)
	};
	Point points3[3] = {
		Point(420,290),
		Point(410,250),
		Point(505,280)
	};
	Point points4[4] = {
		Point(310,270),
		Point(310,80),
		Point(278,72),
		Point(250,270)
	};
	Point points5[7] = {
		Point(490,470),
		Point(480,360),
		Point(410,270),
		Point(250,270),
		Point(200,270),
		Point(100,380),
		Point(448,480)
	};


	g.FillPolygon(&Material, points4, 4);

	points4[0].X = 525; points4[1].X = 525; points4[2].X = 278; points4[3].X = 278;
	points4[0].Y = 120; points4[1].Y = 90; points4[2].Y = 72; points4[3].Y = 120;

	g.FillPolygon(&Material, points4, 4);

	points4[0].X = 770; points4[1].X = 750; points4[2].X = 580; points4[3].X = 570;
	points4[0].Y = 400; points4[1].Y = 280; points4[2].Y = 250; points4[3].Y = 400;

	g.FillPolygon(&Material, points4, 4);

	points4[0].X = 580; points4[1].X = 580; points4[2].X = 500; points4[3].X = 480;
	points4[0].Y = 420; points4[1].Y = 250; points4[2].Y = 330; points4[3].Y = 400;

	g.FillPolygon(&Material, points4, 4);

	points4[0].X = 580; points4[1].X = 580; points4[2].X = 430; points4[3].X = 480;
	points4[0].Y = 400; points4[1].Y = 300; points4[2].Y = 290; points4[3].Y = 400;

	g.FillPolygon(&Material, points4, 4);


	Region rgn2;
	Rect ClipRect(140, 290, 320, 610 - 290);
	GraphicsPath path2;
	path2.AddEllipse(ClipRect);
	Region Rgn1(&path2);
	g.SetClip(&Rgn1, CombineModeExclude);
	g.FillPolygon(&Material, points5, 7);	
	g.SetClip(&rgn2);

	points5[0].X = 545; points5[1].X = 545; points5[2].X = 650; points5[3].X = 650; points5[4].X = 485; points5[5].X = 485; points5[6].X = 485;
	points5[0].Y = 520; points5[1].Y = 480; points5[2].Y = 480; points5[3].Y = 400; points5[4].Y = 400; points5[5].Y = 520; points5[6].Y = 520;

	g.FillPolygon(&Material, points5, 7);

	g.FillPolygon(&hatchBrush, points, 3);
	g.FillPolygon(&hatchBrush, points2, 3);
	g.FillPolygon(&hatchBrush, points3, 3);

	g.FillEllipse(&solidBrush, 150, 300, 300, 300);
	g.FillEllipse(&solidBrush2, 210, 360, 180, 180); //левое колесо///

	g.FillEllipse(&solidBrush, 600, 380, 220, 220);
	g.FillEllipse(&solidBrush2, 650, 430, 120, 120); //правое колесо

    g.FillRectangle(&hatchBrush, 310, 120, 220, 150); //Зкраска окна

	g.FillPath(&pthGrBrush, &path);
	g.FillEllipse(&solidBrush, 700, 480, 20, 20);
	g.FillEllipse(&solidBrush, 275, 430, 50, 50);

	g.DrawLine(&Contur, 448, 480, 490, 470); 
	g.DrawLine(&Contur, 488, 468, 487, 522);
	g.DrawLine(&Contur, 485, 520, 545, 520); //между колесами
	g.DrawLine(&Contur, 543, 522, 545, 480);
	g.DrawLine(&Contur, 543, 482, 600, 482);

	g.DrawLine(&Contur2, 488, 470, 480, 360);
	g.DrawLine(&Contur2, 480, 362, 410, 270);
	g.DrawLine(&Contur2, 412, 270, 200, 270);// окнонтовка колеса
	g.DrawLine(&Contur2, 200, 270, 100, 380);
	g.DrawLine(&Contur2, 100, 380, 160, 400);

	
	g.DrawLine(&Contur2, 250, 270, 280, 70);
	g.DrawLine(&Contur2, 278, 72, 520, 90); //кабина
	g.DrawLine(&Contur2, 520, 90, 580, 250);
	g.DrawLine(&Contur, 578, 250, 750, 280);
	g.DrawLine(&Contur, 750, 280, 770, 400);

	g.DrawLine(&Contur, 580, 250, 570, 400);
	g.DrawLine(&Contur, 570, 400, 645, 400);

	g.DrawLine(&Contur, 574, 300, 752, 300);
	g.DrawLine(&Contur, 573, 320, 755, 320);

	g.DrawLine(&Contur, 580, 250, 528, 302);// оконтовка верх. части кабины
	g.DrawLine(&Contur, 530, 300, 425, 290);

	g.DrawLine(&Contur, 570, 260, 550, 432);//дверца
	g.DrawLine(&Contur, 550, 430, 485, 425);

	g.DrawLine(&Contur, 310, 270, 310, 120);//1
	g.DrawLine(&Contur, 308, 120, 530, 120);
	g.DrawLine(&Contur, 320, 270, 320, 120);


	g.DrawLine(&Contur, 380, 270, 410, 120);
	g.DrawLine(&Contur, 390, 270, 420, 120);

	g.FillRectangle(&Material, 700, 217, 20, 55);
	g.DrawLine(&Contur, 720, 277, 720, 217); //
	g.DrawLine(&Contur, 700, 270, 700, 217);// Труба
	g.FillRectangle(&Material,692, 180, 35, 35);
	
	g.DrawRectangle(&Contur, 692, 180, 35, 35); //

//	Image image(L"1.png");
	g.DrawImage(image, 750, 200);

	Font font(L"Arial", 11.f, FontStyleRegular);
	SolidBrush blackBrush(Color::Black);
	StringFormat sf;
	RectF rect(1050.f, 620.f, 140.f, 30.f);
	const wchar_t text[] = L"Рисунок 1. Трактор";
	g.DrawString(text, -1, &font, rect, &sf, &blackBrush);


}
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR lpszCmdLine, int nCmdShow)
{
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartupOutput gdiplusStartupOutput;

	Status stret = GdiplusStartup(&token, &gdiplusStartupInput, NULL);

	if (stret == Ok)
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
	image = Image ::FromFile(L"1.png");
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
		delete image;
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