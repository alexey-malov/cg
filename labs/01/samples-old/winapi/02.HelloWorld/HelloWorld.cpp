// HelloWorld.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

int APIENTRY WinMain(HINSTANCE /*hInstance*/,
                     HINSTANCE /*hPrevInstance*/,
                     LPSTR     /*lpCmdLine*/,
                     int       /*nCmdShow*/)
{
	MessageBox(NULL, "Hello World", "Hi", MB_OK | MB_ICONINFORMATION);
	return 0;
}



