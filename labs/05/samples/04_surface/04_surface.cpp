// 04_surface.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MyApplication.h"

CMyApplication g_app("3D surface - click to change polygon mode", 640, 480);

int _tmain(int argc, _TCHAR* argv[])
{
	g_app.MainLoop();
	return 0;
}

