#include "XWindow.h"
#include "XDirectT.h"
UINT16  XWindow::Width=1280;
UINT16  XWindow::Height=720;
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		{
			//MessageBox(0, L"left mouse down", 0, 0);
			return 0;
		}
	case WM_KEYDOWN:
		{
		if (wparam == VK_ESCAPE)
			{
				
			}
		return 0;
		}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

XWindow* XWindow::xwindow = nullptr;

XWindow::XWindow()
{
}


XWindow::~XWindow()
{

}

XWindow * XWindow::GetXwindow()
{
	if (xwindow == nullptr)
	{
		xwindow = new XWindow();
	}
	return xwindow;
}

bool XWindow::initWindowApp(HINSTANCE hinstance, int show)
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hinstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"BasicWndclass";
	
	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass File", 0, 0);
		return false;
	}

	ghMainWnd = CreateWindow(
		L"BasicWndclass",
		L"Win64Basic",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		Width,
		Height,
		0,
		0,
		hinstance,
		0
	);
	if (ghMainWnd == 0)
	{
		MessageBox(0, L"CreateWindow File", 0, 0);
	}
	ShowWindow(ghMainWnd, SW_SHOW);
	UpdateWindow(ghMainWnd);
	return true;
}

int XWindow::Run()
{
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		// If there are Window messages then process them.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// Otherwise, do animation/game stuff.
		else
		{
			XDirectT::Getdirectx()->UpdateTime();
			XDirectT::Getdirectx()->CalcFrame();
			XDirectT::Getdirectx()->Draw();
		}
	}

	return (int)msg.wParam;
}


