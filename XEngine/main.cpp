#pragma  once
#include "XWindow.h"
#include "XDirectT.h"
int WINAPI WinMain(HINSTANCE hinstance,HINSTANCE prehinstance,PSTR pCmdLine,int CmdShow)
{
	XWindow::GetXwindow()->initWindowApp(hinstance,0);
	 XDirectT::Getdirectx()->InitD3d();
	 
	
	XWindow::GetXwindow()->Run();
	
	

}