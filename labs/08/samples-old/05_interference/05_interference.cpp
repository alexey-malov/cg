// 04_simple_fragment_shader.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "utils.h"

GLhandleARB g_program;
GLuint spectrumTexture;
DWORD g_lastTick = GetTickCount();

void InitApplication()
{
	glewInit();
	
	// check if shaders
	if (!GLEW_ARB_shader_objects)
	{
		throw std::runtime_error("GL_ARB_shader_objects OpenGL extension is not supported.");
	}
	
	g_program = glCreateProgramObjectARB();
	
	GLhandleARB vertexShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	
	LoadShaderSource(vertexShader, "interference.fsh");
	
	CompileShader(vertexShader);
	
	glAttachObjectARB(g_program, vertexShader);
	
	LinkProgram(g_program);
	
	// we have just linked the program and now can delete vertex shader since it is not needed anymore
	glDeleteObjectARB(vertexShader);

	// load spectrum texture
	FILE * pTextureFile = fopen("spectrum.raw", "rb");
	if (!pTextureFile)
	{
		throw std::runtime_error("cannot find \"spectrum.raw\" texture file");
	}
	fseek(pTextureFile, 0, SEEK_END);
	std::vector<RGBTRIPLE> buffer(ftell(pTextureFile));
	fseek(pTextureFile, 0, SEEK_SET);

	fread(&buffer[0], 1, buffer.size(), pTextureFile);
	fclose(pTextureFile);

	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	gluBuild1DMipmaps(GL_TEXTURE_1D, 3, buffer.size() / 3, GL_BGR_EXT, GL_UNSIGNED_BYTE, &buffer[0]);
}

void DestroyApplication()
{
	if (g_program)
	{
		glDeleteObjectARB(g_program);
	}
}

void Idle()
{
	glutPostRedisplay();
}

void Draw()
{
	DWORD currentTick = GetTickCount();
	float time = (currentTick - g_lastTick) / 1000.0f;
	g_lastTick = currentTick;

	static float phase = 0;
	phase += time * 0.3;
	if (phase >= 1)
	{
		phase -= 1;
	}

	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgramObjectARB(g_program);
	glEnable(GL_TEXTURE_1D);

	GLint spectrumUniform = glGetUniformLocationARB(g_program, "spectrum");
	// �������� � ��� ��������� � ������� ���������� ����� � ������������ � ������� ��� ������ spectrum (������)
	glUniform1iARB(spectrumUniform, 0);

	GLint phaseUniform = glGetUniformLocationARB(g_program, "phase");
	glUniform1fARB(phaseUniform, phase);

	glBegin(GL_QUADS);
	{
		glTexCoord2f(-1, -1);
		glVertex2f(-0.8, -0.8);

		glTexCoord2f(1, -1);
		glVertex2f(0.8, -0.8);
		
		glTexCoord2f(1, 1);
		glVertex2f(0.8, 0.8);
		
		glTexCoord2f(-1, 1);
		glVertex2f(-0.8, 0.8);
		
	}
	glEnd();
	glUseProgramObjectARB(NULL);
	glutSwapBuffers();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("Rainbow");
	glutDisplayFunc(Draw);
	glutIdleFunc(Idle);
	
	try
	{
		InitApplication();
		glutMainLoop();
	}
	catch (std::exception & e)
	{
		printf("Error: %s\n", e.what());
	}
	
	DestroyApplication();
	return 0;
}


