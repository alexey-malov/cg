// 01_3d_axes.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MyApplication.h"

CMyApplication g_application("My first 3d application", 640, 480);

int _tmain(int /*argc*/, _TCHAR* /*argv*/[])
{
	g_application.MainLoop();
	return 0;
}

