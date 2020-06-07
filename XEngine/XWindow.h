#pragma once
#include <windows.h>
class XWindow
{
public:
	XWindow();
	~XWindow();
	static XWindow* GetXwindow();
	bool initWindowApp(HINSTANCE hinstance, int show);
	int Run();
	static UINT16 Width;
	static UINT16 Height;
	HWND ghMainWnd = 0;
private:
	static XWindow* xwindow;
	
	
};

