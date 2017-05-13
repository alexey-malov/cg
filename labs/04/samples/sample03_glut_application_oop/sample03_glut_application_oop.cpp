// sample03_glut_application_oop.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MyApplication.h"

// Создаем экземпляр нашего приложения
CMyApplication app("test");

int _tmain(int argc, _TCHAR* argv[])
{
	// и запускаем его
	app.MainLoop();
	
	return 0;
}

