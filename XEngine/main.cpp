#include "XWindow.h"
int WINAPI WinMain(HINSTANCE hinstance,HINSTANCE prehinstance,PSTR pCmdLine,int CmdShow)
{
	XWindow::GetXwindow()->initWindowApp(hinstance,0);
	XWindow::GetXwindow()->Run();

}