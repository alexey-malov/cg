// 01_initialization.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

void PrintShaderSupportInfo();

int main(int argc, char* argv[])
{
	// initialize glut
	glutInit(&argc, argv);

	// create window
	glutCreateWindow("Vertex shader initialization");

	// initialize glew library
	glewInit();

	PrintShaderSupportInfo();

	return 0;
}

void PrintShaderSupportInfo()
{
	// check if OpenGL implementation supports GLSL Shaders
	if (GLEW_ARB_shader_objects)
	{
		printf("GLSL shader objects are supported\n");
	}
	else
	{
		printf("GLSL shader objects are not supported\n");
	}
}