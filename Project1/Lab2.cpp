//#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
//#include <gdiplusmatrix.h>
#include "WorldWindow.h"
#include "resource.h"
#include <WindowsX.h>
#include "gdiplus.h"
#include <stdlib.h>
#include "gdipluspath.h"
#include <math.h>
#include<vector>


#pragma comment (lib, "GdiPlus.lib")
using namespace Gdiplus;
ULONG_PTR token;
HINSTANCE hinst;
HWND  FieldTask1, FieldTask2, htask1, Fram, AF;

#define M_20PI 62.8318530717958
#define M_EPSILON_E6   1e-6f

void MainFormCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnPaintTask1(HDC hdc);
void OnPaintTask2(HDC hdc);
void OnPaintLab3(HDC hdc);
void ClearField(HDC hdc);
void GIFCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
BOOL Accessory(int x, int y);
void OnPaintVector(HDC hdc, int x1, int y1, int x2, int y2);
inline float Sin(float angle);
inline float Cos(float angle);
inline bool IsZero(float x);
INT_PTR WINAPI Task1(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
INT_PTR WINAPI Task2(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
INT_PTR WINAPI ReadPoligon(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
inline void __fastcall SinCos(float angle, float *sin, float *cos);
class Viewport : public Gdiplus::Rect
{
public:
	inline Viewport(int left, int top, int right, int bottom)
		: Gdiplus::Rect(left, top, right - left, bottom - top) {}

	inline Viewport(const RECT &rect)
		: Gdiplus::Rect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top) {}
	inline operator RECT()const
	{
		RECT rect = { X, Y, X + Width, Y + Height };
		return rect;
	}
};

Point MnogoUG[5] = {
	Point(200,266),
	Point(243,176),
	Point(159,83),
	Point(75,136),
	Point(106,236)
};
Point Template[5] = {
	Point(0,3),
	Point(1,7),
	Point(5,8),
	Point(6,6),
	Point(3,0)
};

PointF TractorPoints[6] = {
	PointF(118,42),
	PointF(168,42),
	PointF(168,92),
	PointF(118,92),

	PointF(90,93),
	PointF(228,142),
};
PointF TractorPoints2[4] = {
	PointF(168,92),
	PointF(268,92),
	PointF(268,142),
	PointF(168,142)
};
PointF TractorPoints3[4] = {
	PointF(248,72),
	PointF(258,72),
	PointF(258,92),
	PointF(248,92)
};
PointF WheelRigth[6] = {
	PointF(230,120),
	PointF(290,120),
	PointF(290,180),
	PointF(230,180),

	PointF(230,150),
	PointF(290,150)
};
PointF WheelLeft[6] = {
	PointF(100,90),
	PointF(190,90),
	PointF(190,180),
	PointF(100,180),

	PointF(100,135),
	PointF(190,135)
};

PointF TractorPointsStart[6] = {
	PointF(118,42),
	PointF(168,42),
	PointF(168,92),
	PointF(118,92),

	PointF(90,93),
	PointF(228,142),
};
PointF TractorPoints2Start[4] = {
	PointF(168,92),
	PointF(268,92),
	PointF(268,142),
	PointF(168,142)
};
PointF TractorPoints3Start[4] = {
	PointF(248,72),
	PointF(258,72),
	PointF(258,92),
	PointF(248,92)
};
PointF WheelRigthStart[6] = {
	PointF(230,120),
	PointF(290,120),
	PointF(290,180),
	PointF(230,180),

	PointF(230,150),
	PointF(290,150)
};
PointF WheelLeftStart[6] = {
	PointF(100,90),
	PointF(190,90),
	PointF(190,180),
	PointF(100,180),

	PointF(100,135),
	PointF(190,135)
};

PointF TractorPointsStart2[6] = {
	PointF(518,42),
	PointF(568,42),
	PointF(568,92),
	PointF(518,92),

	PointF(490,93),
	PointF(628,142),
};
PointF TractorPoints2Start2[4] = {
	PointF(568,92),
	PointF(668,92),
	PointF(668,142),
	PointF(568,142)
};
PointF TractorPoints3Start2[4] = {
	PointF(648,72),
	PointF(658,72),
	PointF(658,92),
	PointF(648,92)
};
PointF WheelRigthStart2[6] = {
	PointF(630,120),
	PointF(690,120),
	PointF(690,180),
	PointF(630,180),

	PointF(630,150),
	PointF(690,150)
};
PointF WheelLeftStart2[6] = {
	PointF(500,90),
	PointF(590,90),
	PointF(590,180),
	PointF(500,180),

	PointF(500,150),
	PointF(590,150)
};


int PoligonSize = 0, currentPoint = 0;
PointF pointsF[2] , Cursor[1];
std::vector<PointF> AllPoints;
std::vector<Gdiplus::PointF> FunctionPoints;
WorldWindow w(-20, 20, 20, -20);
Viewport vp(20, 20, 500, 500);
Image *image;
UINT frameCount, frameIndex;
void PaintGiF(HDC hdc);
void PaintTractor(HDC hdc);
void Tweening(HDC hdc);
void SetStartPosition();
void Display(HDC hdc);
HDC globalHDC;
inline void ViewportToWorld(const WorldWindow &w, const Viewport &vp, Gdiplus::PointF *points, unsigned int count)
{
	float A = (w.Right - w.Left) / (float)vp.Width;
	float B = (w.Bottom - w.Top) / (float)vp.Height;
	float C = w.Left - A * vp.X;
	float D = w.Top - B * vp.Y;
	for (unsigned int i = 0; i < count; ++i) {
		points[i].X = A * points[i].X + C;
		points[i].Y = B * points[i].Y + D;
	}
}
inline void WorldToViewport(const WorldWindow &w, const Viewport &vp, Gdiplus::PointF *points, unsigned int count)
{
	float A = vp.Width / (w.Right - w.Left);
	float B = vp.Height / (w.Bottom - w.Top);
	float C = vp.X - A * w.Left;
	float D = vp.Y - B * w.Top;

	for (unsigned int i = 0; i < count; ++i)
	{
		points[i].X = A * points[i].X + C;
		points[i].Y = B * points[i].Y + D;
	}
}
PointF Tween(const PointF &A, const PointF &B, float t) {
	return PointF(A.X*(1.f - t) + B.X*t, A.Y*(1.f - t) + B.Y*t);
}
void Tween(const PointF *A, const PointF *B, PointF *P, int count, float t) {
	for (int i = 0; i < count; ++i)
		P[i] = Tween(A[i], B[i], t);
}

INT_PTR WINAPI DialogProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_CLOSE:
	{
		PostQuitMessage(WM_QUIT);
	}
	break;
	case WM_COMMAND: {
		HANDLE_WM_COMMAND(hWnd, wParam, lParam, MainFormCommand);
	}
	break;
	}
	return 0;
}
INT_PTR WINAPI AnimationTask(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_TIMER: {
		// увеличиваем индекс кадра (циклически)
		//frameIndex = (frameIndex + 1) % frameCount;
		//// делаем кадр активным
		//image->SelectActiveFrame(&FrameDimensionTime, frameIndex);

		//keyFrameIndex = (keyFrameIndex + 1) % keyFrameCount;
		//AffinesFrameIndex = (AffinesFrameIndex + 1) % frameCount;

		//frameIndex2 = (frameIndex2 + 1) % frameCount2;
	//	MessageBox(htask1, TEXT("Точка находится на границе."), TEXT("Сообщение"), MB_OK);
	//	Gdiplus::Matrix mx;
	//	mx.Translate(5.f, 0.f);
	////	for (int i = 0; i < 70; i++)
	//	{
	//		mx.TransformPoints(TractorPoints, 6);
	//		mx.TransformPoints(TractorPoints2, 4);
	//		mx.TransformPoints(TractorPoints3, 4);//Смещение по х
	//		mx.TransformPoints(WheelLeft, 4);
	//		mx.TransformPoints(WheelRigth, 4);
	//		InvalidateRect(hWnd, NULL, FALSE);
	//	//	PaintTractor(hdc);
	//	//	Sleep(5);
	//	}

		
		
		//UpdateWindow(hWnd);
	}
	break;
	case WM_ERASEBKGND:return 1;
		break;
	case WM_INITDIALOG:
	{
		SetStartPosition();
		htask1 = hWnd;
	//	SetTimer(hWnd, 1, 0, NULL);
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		globalHDC = BeginPaint(hWnd, &ps);
		HDC hdcMem = CreateCompatibleDC(globalHDC);
		DeleteDC(hdcMem);
		ClearField(globalHDC);
		OnPaintLab3(globalHDC);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_CLOSE:
	{
		EndDialog(hWnd, 0);
		DialogBoxParamW(hinst, MAKEINTRESOURCEW(IDD_DIALOG1), NULL, DialogProc, NULL);
	}
	break;
	case WM_COMMAND: {
		HANDLE_WM_COMMAND(hWnd, wParam, lParam, MainFormCommand);
	}
	break;
	}
	return 0;
}
INT_PTR WINAPI AnimationGIF(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_TIMER: {
		frameIndex = (frameIndex + 1) % frameCount;
		image->SelectActiveFrame(&FrameDimensionTime, frameIndex);
		InvalidateRect(hWnd, NULL, FALSE);
	}
	break;
	case WM_ERASEBKGND:return 1;
		break;
	case WM_INITDIALOG:
	{
		htask1 = GetDlgItem(hWnd, IDC_GIFBOX);
		Fram = GetDlgItem(hWnd, IDC_STATICFram);
		char str[20];
		
    	image = Image::FromFile(L"ra.gif");
		if (NULL == image) return -1;
		frameIndex = 0;
		frameCount = image->GetFrameCount(&FrameDimensionTime);

		_itoa_s(frameCount, str, 10);
		SetWindowText(Fram, str);
		SetTimer(hWnd, 1, 40, NULL);	
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(htask1, &ps);
		PaintGiF(hdc);
		EndPaint(htask1, &ps);
	}
	break;
	case WM_CLOSE:
	{
		EndDialog(hWnd, 0);
		DialogBoxParamW(hinst, MAKEINTRESOURCEW(IDD_DIALOG1), NULL, DialogProc, NULL);
	}
	break;
	case WM_DESTROY: {
		if (NULL != image) delete image;
	}
	break;
	case WM_COMMAND: {
		HANDLE_WM_COMMAND(hWnd, wParam, lParam, GIFCommand);
	}
	break;
	}
	return 0;
}
void GIFCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
	//case IDC_GIFBTN: //Переход в окно с ГИФ
	//{
	//	EndDialog(hwnd, 0);
	//	DialogBoxParamW(hinst, MAKEINTRESOURCEW(IDD_DIALOG6), NULL, AnimationGIF, NULL);
	//}
	//break;
	//case IDC_ANIMATION:
	//{
	//	EndDialog(hwnd, 0);
	//	DialogBoxParamW(hinst, MAKEINTRESOURCEW(IDD_DIALOG5), NULL, AnimationTask, NULL);
	//}
	//break;
	//case IDOK: //Добавить точку в полигон
	//{
	//	TCHAR XNumber[10], YNumber[10];
	//	GetWindowText(GetDlgItem(hwnd, IDC_EDIT5), XNumber, 10);
	//	GetWindowText(GetDlgItem(hwnd, IDC_EDIT6), YNumber, 10);
	//	AllPoints.push_back(Gdiplus::PointF(atoi(XNumber), atoi(YNumber)));
	//	EndDialog(hwnd, 0);
	//}
	//break;	
	}
}
void MainFormCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
	case IDC_GIFBTN: //Переход в окно с ГИФ
	{
		EndDialog(hwnd, 0);
		DialogBoxParamW(hinst, MAKEINTRESOURCEW(IDD_DIALOG6), NULL, AnimationGIF, NULL);
	}
	break;
	case IDC_ANIMATION:
	{
		EndDialog(hwnd, 0);
		DialogBoxParamW(hinst, MAKEINTRESOURCEW(IDD_DIALOG5), NULL, AnimationTask, NULL);
	}
	break;
	case IDOK: //Добавить точку в полигон
	{
		TCHAR XNumber[10], YNumber[10];
		GetWindowText(GetDlgItem(hwnd, IDC_EDIT5), XNumber, 10);
		GetWindowText(GetDlgItem(hwnd, IDC_EDIT6), YNumber, 10);
		AllPoints.push_back(Gdiplus::PointF(atoi(XNumber), atoi(YNumber)));
		EndDialog(hwnd, 0);
	}
	break;
	case IDC_BUTTON4: //Задать полигон
	{
		TCHAR PoligonSizeString[10];
		GetWindowText(GetDlgItem(hwnd, IDC_EDIT4), PoligonSizeString, 10);
		PoligonSize = atoi(PoligonSizeString);
		if (PoligonSize > 2)
		{
			AllPoints.clear();
			if (PoligonSize == 797)
			{
				PoligonSize = 5;
				for (int i = 0; i < PoligonSize; i++)
				{
					AllPoints.push_back(Gdiplus::PointF(Template[i].X, Template[i].Y));
				}
			}
			else
				for (int i = 0; i < PoligonSize; i++)
				{
					DialogBoxParamW(hinst, MAKEINTRESOURCEW(IDD_DIALOG4), NULL, ReadPoligon, NULL);
				}

		}
		else MessageBox(htask1, TEXT("Не верное кол-во сторон!"), TEXT("Окно"), 0);
	}
	break;
	case IDC_BUTTON1: //Задание 1
	{
		EndDialog(hwnd, 0);
		DialogBoxParamW(hinst, MAKEINTRESOURCEW(IDD_DIALOG2), NULL, Task1, NULL);
	}
	break;
	case IDC_BUTTON2: //Задание 2
	{
		EndDialog(hwnd, 0);
		DialogBoxParamW(hinst, MAKEINTRESOURCEW(IDD_DIALOG3), NULL, Task2, NULL);
	}
	break;
	case IDC_BUTTON3: //Проверка задание 1
	{
		if (PoligonSize != 0)
		{
			htask1 = hwnd;
			TCHAR Xsting[10], Ystring[10];
			GetWindowText(GetDlgItem(hwnd, IDC_EDIT3), Xsting, 10);
			GetWindowText(GetDlgItem(hwnd, IDC_EDIT2), Ystring, 10);

			Cursor[0].X = atof(Xsting); Cursor[0].Y = atof(Ystring);
			//int X = atoi(Xsting), Y = atoi(Ystring);

			HDC hdc = GetDC(FieldTask1);
			ClearField(hdc);
			OnPaintTask1(hdc);
			ReleaseDC(FieldTask1, hdc);

			if (Accessory(Cursor[0].X, Cursor[0].Y))  MessageBox(hwnd, TEXT("Точка принадлежит многоугольнику."), TEXT("Сообщение"), MB_OK);
			else MessageBox(hwnd, TEXT("Точка не принадлежит многоугольнику."), TEXT("Сообщение"), MB_OK);
		}
		else MessageBox(htask1, TEXT("Введите кол-во сторон!"), TEXT("Окно"), 0);
	}
	break;
	}
}
void PaintTractor(HDC hdc)
{
	Graphics g(hdc);
	g.SetSmoothingMode(SmoothingModeHighQuality);
	SolidBrush Material(Color::Green);
	SolidBrush Elips(Color::Black);
	Pen Contur(Color(255, 0, 0, 0), 5.f);
	Pen Contur2(Color::Red, 5.f);
	Rect rect;
	g.GetVisibleClipBounds(&rect);
	Bitmap backBuffer(rect.Width, rect.Height, &g);
	Graphics temp(&backBuffer);
	temp.Clear(Color::White);

	Point lines[2];
	lines[0].X = 0; lines[0].Y = 370;
	lines[1].X = 800; lines[1].Y = 370;
	temp.DrawLine(&Contur, lines[0], lines[1]);
	lines[0].X = 0; lines[0].Y = 550;			//Отрисовка дороги и скалы.
	lines[1].X = 800; lines[1].Y = 550;
	temp.DrawLine(&Contur, lines[0], lines[1]);

	Rect d;
	d.X = 0; d.Y = 175; d.Width = 500; d.Height = 200;
	temp.DrawRectangle(&Contur, d);

	temp.FillPolygon(&Material, TractorPoints, 4);
	temp.DrawPolygon(&Contur, TractorPoints, 4);

	temp.FillPolygon(&Material, TractorPoints2, 4);
	temp.DrawPolygon(&Contur, TractorPoints2, 4);

	temp.FillPolygon(&Material, TractorPoints3, 4);
	temp.DrawPolygon(&Contur, TractorPoints3, 4);
	//<-------------------->
	Rect WHL; 

	WHL.Width = sqrt(pow((WheelLeft[5].X - WheelLeft[4].X),2) + pow((WheelLeft[5].Y - WheelLeft[4].Y), 2));
	WHL.X = WheelLeft[4].X;
	WHL.Y = WheelLeft[0].Y;
	temp.FillEllipse(&Elips, WHL.X, WHL.Y, WHL.Width, WHL.Width); //левое колесо

	WHL.Width = sqrt(pow((WheelRigth[5].X - WheelRigth[4].X), 2) + pow((WheelRigth[5].Y - WheelRigth[4].Y), 2));
	WHL.X = WheelRigth[4].X;
	WHL.Y = WheelRigth[0].Y;
	temp.FillEllipse(&Elips, WHL.X, WHL.Y, WHL.Width, WHL.Width); //правое колесо

	g.DrawImage(&backBuffer, rect);
}
void OnPaintLab3(HDC hdc)
{
	Graphics g(hdc);
	g.SetSmoothingMode(SmoothingModeHighQuality);
	Gdiplus::Matrix mx;

	PaintTractor(hdc);

	mx.Translate(5.f, 0.f);
	for (int i = 0; i < 80; i++) 
	{	
		mx.TransformPoints(TractorPoints, 6);
		mx.TransformPoints(TractorPoints2, 4);
		mx.TransformPoints(TractorPoints3, 4);//Смещение по х
		mx.TransformPoints(WheelLeft, 6);
		mx.TransformPoints(WheelRigth, 6);
		PaintTractor(hdc);
	//	Sleep(4);
	}
	
	//mx.Translate(25.f, 0.f);
	mx.RotateAt(45, PointF(TractorPoints[0].X, TractorPoints[0].Y));
	mx.TransformPoints(TractorPoints, 6);
	mx.TransformPoints(TractorPoints2, 4);
	mx.TransformPoints(TractorPoints3, 4);//Наклон
	mx.TransformPoints(WheelLeft, 6);
	mx.TransformPoints(WheelRigth, 6);
	PaintTractor(hdc);

	mx.Reset();
	mx.Translate(25.f, 25.f);
	mx.Scale(0.96, 0.96);
	for (int i = 0; i < 25; i++)
	{
		mx.TransformPoints(TractorPoints, 6);
		mx.TransformPoints(TractorPoints2, 4);
		mx.TransformPoints(TractorPoints3, 4);//Падение.
		mx.TransformPoints(WheelLeft, 6);
		mx.TransformPoints(WheelRigth, 6);
		PaintTractor(hdc);
		//Sleep(10);
		if (i == 23) 
		{ 
			mx.Translate(-150.f, -300.f); 
			mx.Shear(0.5, 0.5); 
		}
	}
	Tweening(hdc);
}
void ClearField(HDC hdc)
{
	Graphics g(hdc);
	g.Clear(Color::White);
}
void PaintGiF(HDC hdc) {
	Graphics g(hdc);
	Rect rect;
	g.GetVisibleClipBounds(&rect);
	Bitmap backBuffer(rect.Width, rect.Height, &g);
	Graphics temp(&backBuffer);
	temp.Clear(Color::White);
	temp.DrawImage(image, 0, 0);
	g.DrawImage(&backBuffer, rect);
}
void Tweening(HDC hdc) {
	Graphics g(hdc);
	g.SetSmoothingMode(SmoothingModeHighQuality);
	Rect rect;
	g.GetVisibleClipBounds(&rect);
	Bitmap backBuffer(rect.Width, rect.Height, &g);
	Graphics temp(&backBuffer);
	SolidBrush Material(Color::Green);
	SolidBrush Elips(Color::Black);
	Pen Contur(Color(255, 0, 0, 0), 5.f);
	float t = 0;

	Point lines[4];
	lines[0].X = 0; lines[0].Y = 370;
	lines[1].X = 800; lines[1].Y = 370;
	lines[2].X = 0; lines[2].Y = 550;
	lines[3].X = 800; lines[3].Y = 550;

	Rect d;
	d.X = 0; d.Y = 175; d.Width = 500; d.Height = 200;

	PointF frames1[6];
	PointF frames2[4];
	PointF frames3[4];
	PointF frames4[6];
	PointF frames5[6];

	for (int i = 0; i <= 5; i++) 
	{
		Sleep(100);
		Tween(TractorPoints, TractorPointsStart2, frames1, 6, t);
		Tween(TractorPoints2, TractorPoints2Start2, frames2, 4, t);
		Tween(TractorPoints3, TractorPoints3Start2, frames3, 4, t);
		Tween(WheelLeft, WheelLeftStart2, frames4, 6, t);
		Tween(WheelRigth, WheelRigthStart2, frames5, 6, t);
		temp.Clear(Color::White);

		temp.DrawLine(&Contur, lines[0], lines[1]);
		temp.DrawLine(&Contur, lines[2], lines[3]);
		temp.DrawRectangle(&Contur, d);

		temp.FillPolygon(&Material, frames1, 4);
		temp.DrawPolygon(&Contur, frames1, 4);

		temp.FillPolygon(&Material, frames2, 4);
		temp.DrawPolygon(&Contur, frames2, 4);

		temp.FillPolygon(&Material, frames3, 4);
		temp.DrawPolygon(&Contur, frames3, 4);


		Rect WHL; 


		WHL.Width = sqrt(pow((frames4[5].X - frames4[4].X), 2) + pow((frames4[5].Y - frames4[4].Y), 2));
		WHL.X = frames4[4].X;
		WHL.Y = frames4[0].Y;
		temp.FillEllipse(&Elips, WHL.X, WHL.Y, WHL.Width, WHL.Width); //левое колесо


		WHL.Width = sqrt(pow((frames5[5].X - frames5[4].X), 2) + pow((frames5[5].Y - frames5[4].Y), 2));
		WHL.X = frames5[4].X;
		WHL.Y = frames5[0].Y;
		temp.FillEllipse(&Elips, WHL.X, WHL.Y, WHL.Width, WHL.Width); //правое колесо

		g.DrawImage(&backBuffer, rect);
		t += 0.2;
	}
	t = 0;
	for (int i = 0; i <= 5; i++)
	{
		Tween(TractorPointsStart2, TractorPointsStart, frames1, 6, t);
		Tween(TractorPoints2Start2, TractorPoints2Start, frames2, 4, t);
		Tween(TractorPoints3Start2, TractorPoints3Start, frames3, 4, t);
		Tween(WheelLeftStart2, WheelLeftStart, frames4, 6, t);
		Tween(WheelRigthStart2, WheelRigthStart, frames5, 6, t);
		temp.Clear(Color::White);

		temp.DrawLine(&Contur, lines[0], lines[1]);
		temp.DrawLine(&Contur, lines[2], lines[3]);
		temp.DrawRectangle(&Contur, d);

		temp.FillPolygon(&Material, frames1, 4);
		temp.DrawPolygon(&Contur, frames1, 4);

		temp.FillPolygon(&Material, frames2, 4);
		temp.DrawPolygon(&Contur, frames2, 4);

		temp.FillPolygon(&Material, frames3, 4);
		temp.DrawPolygon(&Contur, frames3, 4);


		Rect WHL; 
		
		WHL.Width = sqrt(pow((frames4[5].X - frames4[4].X), 2) + pow((frames4[5].Y - frames4[4].Y), 2));
		WHL.X = frames4[4].X;
		WHL.Y = frames4[0].Y;
		temp.FillEllipse(&Elips, WHL.X, WHL.Y, WHL.Width, WHL.Width); //левое колесо


		WHL.Width = sqrt(pow((frames5[5].X - frames5[4].X), 2) + pow((frames5[5].Y - frames5[4].Y), 2));
		WHL.X = frames5[4].X;
		WHL.Y = frames5[0].Y;
		temp.FillEllipse(&Elips, WHL.X, WHL.Y, WHL.Width, WHL.Width); //правое колесо

		g.DrawImage(&backBuffer, rect);
		t += 0.2;
		Sleep(100);
	}
}
void SetStartPosition() {
	for (int i = 0; i < 6; i++)
	{
		TractorPoints[i].X = TractorPointsStart[i].X;
		TractorPoints[i].Y = TractorPointsStart[i].Y;

		WheelRigth[i].X = WheelRigthStart[i].X;
		WheelRigth[i].Y = WheelRigthStart[i].Y;

		WheelLeft[i].X = WheelLeftStart[i].X;
		WheelLeft[i].Y = WheelLeftStart[i].Y;
	}
	for (int i = 0; i < 4; i++)
	{
		TractorPoints2[i].X = TractorPoints2Start[i].X;
		TractorPoints2[i].Y = TractorPoints2Start[i].Y;

		TractorPoints3[i].X = TractorPoints3Start[i].X;
		TractorPoints3[i].Y = TractorPoints3Start[i].Y;	
	}
}
void Display(HDC hdc)
{
	Graphics g(hdc);
	g.SetSmoothingMode(SmoothingModeHighQuality);
	Matrix mx;

	PaintTractor(hdc);

	mx.Translate(5.f, 0.f);

	mx.TransformPoints(TractorPoints, 6);
	mx.TransformPoints(TractorPoints2, 4);
	mx.TransformPoints(TractorPoints3, 4);//Смещение по х
	mx.TransformPoints(WheelLeft, 4);
	mx.TransformPoints(WheelRigth, 4);
	PaintTractor(hdc);
	

	//temp.TranslateTransform(width*(0.6 + (float)frameIndex / frameCount) - 1000, 0);

	//
	//mx.RotateAt((float)10 * frameIndex, PointF(195, height - 105));
	//temp.MultiplyTransform(&mx);
	//DrawWheel(temp, 0);

	//temp.ResetTransform();
	//temp.TranslateTransform(width*(0.6 + (float)frameIndex / frameCount) - 1000, 0);
	//mx.Reset();
	//mx.RotateAt((float)10 * frameIndex, PointF(345, height - 105));
	//temp.MultiplyTransform(&mx);
	//DrawWheel(temp, 150);

	//temp.ResetTransform();
	//temp.TranslateTransform(width*(0.6 + (float)frameIndex / frameCount) - 1000, 0);
	//mx.Reset();
	//mx.RotateAt((float)10 * frameIndex, PointF(585, height - 105));
	//temp.MultiplyTransform(&mx);
	//DrawWheel(temp, 390);

	//temp.ResetTransform();
	//temp.TranslateTransform(width*(0.6 + (float)frameIndex / frameCount) - 550, 10);
	//temp.ScaleTransform((float)1 - (frameIndex + 0.1)*0.02, (float)1 - (frameIndex + 0.1) * 0.01);
	//temp.RotateTransform(15 + frameIndex);
	//DrawGasCloud(temp);

	//temp.ResetTransform();
	//temp.TranslateTransform(width*(0.6 + (float)frameIndex / frameCount) - 550, 0);
	//temp.ScaleTransform((float)1 - (frameIndex + 0.1)*0.02, (float)1 - (frameIndex + 0.1) * 0.02);
	//temp.RotateTransform(25 + frameIndex);
	//DrawGasCloud(temp);
	//g.DrawImage(&backBuffer, rect);
}

bool LineClip(const WorldWindow &w, PointF &pt1, PointF &pt2)
{
	float tin = 0, tout = 1, thit = 0;

	float p1 = -(pt2.X - pt1.X);
	float p2 = -p1;
	float p3 = -(pt2.Y - pt1.Y);
	float p4 = -p3;

	float q1 = pt1.X - w.Left;
	float q2 = w.Right - pt1.X;
	float q3 = pt1.Y - w.Bottom;
	float q4 = w.Top - pt1.Y;

	if ((IsZero(p1) && q1 < 0) || (IsZero(p3) && q3 < 0) || (IsZero(p2) && q2 < 0) || (IsZero(p4) && q4 < 0)) return false;

	thit = q1 / p1;
	if (p1 < 0)
	{
		if (thit > tin) tin = thit;
	}
	else if (thit < tout) tout = thit;

	thit = q2 / p2;
	if (p2 < 0)
	{
		if (thit > tin) tin = thit;
	}
	else if (thit < tout) tout = thit;

	thit = q3 / p3;
	if (p3 < 0)
	{
		if (thit > tin) tin = thit;
	}
	else if (thit < tout) tout = thit;

	thit = q4 / p4;
	if (p4 < 0)
	{
		if (thit > tin) tin = thit;
	}
	else if (thit < tout) tout = thit;


	if (tin > tout) return false;

	pointsF[1].X = pt1.X + p2 * tin;
	pointsF[1].Y = pt1.Y + p4 * tin; // Вычисляем новые точки

	pointsF[0].X = pt1.X + p2 * tout;
	pointsF[0].Y = pt1.Y + p4 * tout;

	return true;
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE NotUsed, LPSTR pCmdLine, int fShow)
{

	hinst = hInstance;
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartupOutput gdiplusStartupOutput;

	Status stret = GdiplusStartup(&token, &gdiplusStartupInput, NULL);

	if (stret == Ok)
	{
		DialogBoxParamW(hInstance, MAKEINTRESOURCEW(IDD_DIALOG1), NULL, DialogProc, NULL);
		GdiplusShutdown(token);
	}
	return EXIT_SUCCESS;
}
INT_PTR WINAPI Task1(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_INITDIALOG:
	{
		FieldTask1 = GetDlgItem(hWnd, IDC_STATIC1);
		htask1 = GetDlgItem(hWnd, IDD_DIALOG2);
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(FieldTask1, &ps);
		HDC hdcMem = CreateCompatibleDC(hdc);
		DeleteDC(hdcMem);
		ClearField(hdc);
		EndPaint(FieldTask1, &ps);
	}
	break;
	case WM_CLOSE:
	{
		EndDialog(hWnd, 0);
		DialogBoxParamW(hinst, MAKEINTRESOURCEW(IDD_DIALOG1), NULL, DialogProc, NULL);
	}
	break;
	case WM_COMMAND: {
		HANDLE_WM_COMMAND(hWnd, wParam, lParam, MainFormCommand);
	}
					 break;
	}
	return 0;
}
INT_PTR WINAPI ReadPoligon(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_CLOSE:
	{
		EndDialog(hWnd, 0);
	}
	break;
	case WM_COMMAND: {
		HANDLE_WM_COMMAND(hWnd, wParam, lParam, MainFormCommand);
	}
					 break;
	}
	return 0;
}
INT_PTR WINAPI Task2(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_INITDIALOG:
	{
		FieldTask1 = GetDlgItem(hWnd, IDC_STATIC2);
		htask1 = GetDlgItem(hWnd, IDD_DIALOG3);
	}
	break;
	case WM_PAINT:
	{
		BITMAP bm;
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(FieldTask1, &ps);
		HDC hdcMem = CreateCompatibleDC(hdc);
		DeleteDC(hdcMem);
		ClearField(hdc);
		OnPaintTask2(hdc);
		EndPaint(FieldTask1, &ps);
	}
	break;
	case WM_CLOSE:
	{
		EndDialog(hWnd, 0);
		DialogBoxParamW(hinst, MAKEINTRESOURCEW(IDD_DIALOG1), NULL, DialogProc, NULL);
	}
	break;
	case WM_COMMAND: {
		HANDLE_WM_COMMAND(hWnd, wParam, lParam, MainFormCommand);
	}
					 break;
	}
	return 0;
}
void FillMassiv()
{
	FunctionPoints.clear();
	FunctionPoints.reserve(500);
	float dx = (M_20PI) / (float)(499);
	float fsin = 0, fcos = 0, fsin2 = 0, fcos2 = 0;
	for (size_t i = 0; i < 500; ++i)
	{
		SinCos(i*dx, &fsin, &fcos);
		SinCos(1.1 * i*dx, &fsin2, &fcos2);
		FunctionPoints.emplace_back((11 * fcos + 10 * fcos2), (11 * fsin - 10 * fsin2));
	}
}
void OnPaintTask1(HDC hdc)
{
	Graphics g(hdc);
	g.SetSmoothingMode(SmoothingModeHighQuality);
	Pen Contur(Color(255, 0, 0, 0), 1.f);
	SolidBrush Material(Color::Red);

	Pen VPcolor(Color::Red, 1.f);
	g.DrawRectangle(&VPcolor, vp.X, vp.Y, vp.Width, vp.Height);

	PointF *Mypoligon = new PointF[PoligonSize];
	for (size_t i = 0; i < PoligonSize; i++)
	{
		Mypoligon[i] = AllPoints[i];
	}

	WorldToViewport(w, vp, Cursor, 1);
	g.FillEllipse(&Material, (double)Cursor[0].X, (double)Cursor[0].Y, 3, 3);
	WorldToViewport(w, vp, Mypoligon, PoligonSize);
	g.DrawPolygon(&Contur, Mypoligon, PoligonSize);
	delete[] Mypoligon;
}
void OnPaintTask2(HDC hdc)
{
	Graphics g(hdc);
	g.SetSmoothingMode(SmoothingModeHighQuality);
	Pen Contur(Color(255, 0, 0, 0), 1.f);
	Pen VPcolor(Color::Red, 1.f);
	WorldWindow w(-25, 25, 25, -25);
	Viewport vp(100, 20, 500, 420);

	g.DrawRectangle(&VPcolor, vp.X, vp.Y, vp.Width, vp.Height);
	FillMassiv();
	for (size_t i = 1; i < FunctionPoints.size(); ++i)
	{
		pointsF[0].X = FunctionPoints[i - 1].X;
		pointsF[0].Y = FunctionPoints[i - 1].Y;
		pointsF[1].X = FunctionPoints[i].X;
		pointsF[1].Y = FunctionPoints[i].Y;
		bool fIsDraw = LineClip(w, pointsF[0], pointsF[1]);
		if (fIsDraw)
		{
			WorldToViewport(w, vp, pointsF, 2);
			g.DrawLines(&Contur, pointsF, 2);
		}
	}
}
BOOL Accessory(int x, int y)
{
	int index1 = PoligonSize-1, minus = 0, plus = 0;
	float Scalar = 0;
	PointF T1, T2, Tcenter,VectorA,VectorP;
	PointF *Mypoligon = new PointF[PoligonSize];

	for (size_t i = 0; i < PoligonSize; i++)
	{
		Mypoligon[i] = AllPoints[i];
	}
	ViewportToWorld(w, vp, Cursor, 1);
	//WorldToViewport(w, vp, Mypoligon, PoligonSize);
	for (size_t i = 0; i < PoligonSize; i++)
	{
		T1.X = Mypoligon[index1].X; T1.Y = Mypoligon[index1].Y; //Координаты первой точки
		T2.X = Mypoligon[i].X; T2.Y = Mypoligon[i].Y; //Координаты второй точки
		Tcenter.X = (float)(T1.X + T2.X) / 2;
		Tcenter.Y = (float)(T1.Y + T2.Y) / 2; //Координаты центра прямой

	/*	HDC hdc = GetDC(FieldTask1);
		OnPaintVector(hdc, Tcenter.X, Tcenter.Y, (Tcenter.X + (T2.Y - T1.Y)), (Tcenter.Y - (T2.X - T1.X)));
		ReleaseDC(FieldTask1, hdc);*/

		VectorA.X = (Tcenter.X) - (Tcenter.X + (T2.Y - T1.Y));
		VectorA.Y = (Tcenter.Y) - (Tcenter.Y - (T2.X - T1.X));//Вектор прямой

		VectorP.X = Cursor->X - Tcenter.X;
		VectorP.Y = Cursor->Y - Tcenter.Y;//Вектор точки

		Scalar = (VectorA.X * VectorP.X) + (VectorA.Y * VectorP.Y);//скалярное произведение
		if (IsZero(Scalar)) 
		{ 
			MessageBox(htask1, TEXT("Точка находится на границе."), TEXT("Сообщение"), MB_OK);
			return TRUE; 
		}
		if (Scalar > 0) plus++; 
		else minus++;

		if (index1 == PoligonSize-1) index1 = 0; else index1++;
	}
	delete[] Mypoligon;
	if (plus == PoligonSize || minus == PoligonSize)
		return TRUE;
	else 
		return FALSE;
}
void OnPaintVector(HDC hdc, int x1, int y1, int x2, int y2)
{
	Graphics g(hdc);
	g.SetSmoothingMode(SmoothingModeHighQuality);
	g.DrawLine(&Pen(Color::Red, 1.f), x1, y1, x2, y2);
}
inline float Sin(float angle)
{
	__asm
	{ 
	fld   angle
	fsin
	}
} 
inline float Cos(float angle)
{
	__asm
	{
		fld   angle
		fcos
	}
}
inline bool IsZero(float x) {
	return (abs(x) < M_EPSILON_E6);
}
inline void __fastcall SinCos(float angle, float*sin, float*cos) 
{
	__asm {
		fld     angle
		fsincos
		fstp    dword ptr[edx]
		fstp    dword ptr[ecx]
	}
}