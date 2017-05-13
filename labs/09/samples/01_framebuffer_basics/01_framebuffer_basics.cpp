// 01_framebuffer_basics.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MyApplication.h"

CMyApplication app("FrameBuffer example", 800, 600);

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		glewInit();

		if (!GLEW_EXT_framebuffer_object)
		{
			throw std::runtime_error("GL_EXT_framebuffer_object extension is not available");
		}

		app.MainLoop();
	}
	catch (std::exception const& e)
	{
		std::cout << e.what();
		return 1;
	}
	return 0;
}

