// 02_simple_vertex_shader.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Application.h"
#include "MyApplication.h"

int main(int argc, char* argv[])
{

	try
	{
		CMyApplication app(&argc, argv, 640, 480, "Simple vertex shader");
		app.Init();
		app.Run();
	}
	catch (std::exception & e)
	{
		printf("%s\n", e.what());
	}

	return 0;
}

