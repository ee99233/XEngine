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
private:
	HWND ghMainWnd = 0;
	
	static XWindow* xwindow;
	
	
};

