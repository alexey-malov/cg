// 02_cube_maps.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "utils.h"

using namespace Gdiplus;

void SetUpProjectionMatrixAndViewPort(int w, int h, double fov, double zNear, double zFar);
GLuint LoadCubemapTextureFromFile(const wchar_t * negXFileName, const wchar_t * posXFileName, const wchar_t * negYFileName, const wchar_t * posYFileName, const wchar_t * negZFileName, const wchar_t * posZFileName);
void InitApplication();
void CheckExtensions();
void DrawScene();
void DestroyApplication();
void OnSize(int x, int y);



GLuint g_cubeMapTexture = 0;
ULONG_PTR g_gdiPlusToken = 0;

GLhandleARB g_programObject = 0;
GLhandleARB g_vertexShader = 0;
GLhandleARB g_fragmentShader = 0;
GLint g_environmentVariableLocation = 0;

void OnIdle();

int main(int argc, char* argv[])
{
	GdiplusStartupInput gdiplusInput;
	GdiplusStartupOutput gdiplusOutput;
	GdiplusStartup(&g_gdiPlusToken, &gdiplusInput, &gdiplusOutput);

	atexit(&DestroyApplication);
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Render to texture");
	glutDisplayFunc(DrawScene);
	glutReshapeFunc(&OnSize);
	glutIdleFunc(&OnIdle);
	
	glewInit();
	
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

double g_rotationAngleX = 0;
double g_rotationAngleY = 0;
double g_rotationAngleZ = 0;

void DrawScene()
{
	glClearColor(1, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	glLoadIdentity();
	gluLookAt(
		0, 0, 5,
		0, 0, 0,
		0, 1, 0
		);

	glUseProgramObjectARB(g_programObject);

	glActiveTexture(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_CUBE_MAP_EXT);
	assert(glGetError() == GL_NO_ERROR);

	glBindTexture(GL_TEXTURE_CUBE_MAP_EXT, g_cubeMapTexture);
	assert(glGetError() == GL_NO_ERROR);

	glUniform1iARB(g_environmentVariableLocation, 0);
	assert(glGetError() == GL_NO_ERROR);

	glRotated(g_rotationAngleX, 1, 0, 0);
	glRotated(g_rotationAngleY, 0, 1, 0);
	glRotated(g_rotationAngleZ, 0, 0, 1);

	//glutSolidTeapot(1.0);
	glutSolidTorus(0.5, 1.2, 20, 40);

	glUseProgramObjectARB(NULL);

	glutSwapBuffers();
}

void IncrementAngle(double & angle, double offset)
{
	angle = fmod(angle + offset, 360);
}

void OnIdle()
{
	static DWORD lastTickCount = GetTickCount();

	DWORD currentTick = GetTickCount();
	double time = (currentTick - lastTickCount) * 0.001;
	lastTickCount = currentTick;

	g_rotationAngleX = fmod(g_rotationAngleX + time * 33, 360);
	g_rotationAngleY = fmod(g_rotationAngleY + time * 47, 360);
	g_rotationAngleZ = fmod(g_rotationAngleZ + time * 67, 360);

	glutPostRedisplay();
}


void CheckExtensions()
{
	// проверяем поддержку кубических текстур
	if (!GLEW_EXT_texture_cube_map)
	{
		throw std::runtime_error("Cube maps are not supported");
	}

	if (!GLEW_ARB_shader_objects)
	{
		throw std::runtime_error("Shader objects are not supported");
	}

	if (!GLEW_SGIS_generate_mipmap)
	{
		throw std::runtime_error("Automatic mipmap generation is not supported");
	}
}

void LoadTextures()
{
	g_cubeMapTexture = LoadCubemapTextureFromFile(
		L"brightday2_negative_x.png",
		L"brightday2_positive_x.png",
		L"brightday2_negative_y.png",
		L"brightday2_positive_y.png",
		L"brightday2_negative_z.png",
		L"brightday2_positive_z.png"
		);
}

void SetUpProjectionMatrixAndViewPort(int w, int h, double fov, double zNear, double zFar)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, (double)w / h, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);
}

void LoadTextureImageFromFile(const wchar_t * fileName, GLuint textureTarget, GLuint level = 0)
{
	Bitmap bmp(fileName);
	
	// failed to load a bitmap?
	if (bmp.GetLastStatus() != Ok)
	{
		// failed to load bitmap
		throw std::runtime_error("Failed to load texture");
	}

	int w = bmp.GetWidth();
	int h = bmp.GetHeight();

	Rect rc(0, 0, bmp.GetWidth(), bmp.GetHeight());	
	BitmapData lockedData;
	
	if (bmp.LockBits(&rc, ImageLockModeRead, PixelFormat32bppARGB, &lockedData) != Ok)
	{
		// failed to lock bits
		throw std::runtime_error("Failed to lock bitmap bits");
	}

	glTexImage2D(textureTarget, level, GL_RGBA, bmp.GetWidth(), bmp.GetHeight(), 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, lockedData.Scan0);

	assert(glGetError() == GL_NO_ERROR);

	bmp.UnlockBits(&lockedData);	
}

GLuint LoadCubemapTextureFromFile(
	const wchar_t * negXFileName, 
	const wchar_t * posXFileName, 
	const wchar_t * negYFileName, 
	const wchar_t * posYFileName, 
	const wchar_t * negZFileName, 
	const wchar_t * posZFileName)
{
	
	GLuint textureName = 0;
	glGenTextures(1, &textureName);
	if (textureName == 0)
	{
		// failed to generate texture name
		throw std::runtime_error("Failed to generate texture name");
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP_EXT, textureName);
	assert(glGetError() == GL_NO_ERROR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	assert(glGetError() == GL_NO_ERROR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	assert(glGetError() == GL_NO_ERROR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_EXT);
	assert(glGetError() == GL_NO_ERROR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_EXT);
	assert(glGetError() == GL_NO_ERROR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_WRAP_R_EXT, GL_CLAMP_TO_EDGE_EXT);
	assert(glGetError() == GL_NO_ERROR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
	assert(glGetError() == GL_NO_ERROR);

	LoadTextureImageFromFile(negXFileName, GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT);
	LoadTextureImageFromFile(posXFileName, GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT);
	LoadTextureImageFromFile(negYFileName, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT);
	LoadTextureImageFromFile(posYFileName, GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT);
	LoadTextureImageFromFile(negZFileName, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT);
	LoadTextureImageFromFile(posZFileName, GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT);

	return textureName;
}

void LoadShaders()
{
	g_programObject = glCreateProgramObjectARB();
	if (!g_programObject)
	{
		throw std::runtime_error("Failed to create program object");
	}

	g_vertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	if (!g_vertexShader)
	{
		throw std::runtime_error("Failed to create vertex shader");
	}

	g_fragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	if (!g_fragmentShader)
	{
		throw std::runtime_error("Failed to create fragment shader");
	}

	LoadShaderSource(g_vertexShader, "vertex.txt");
	assert(glGetError() == GL_NO_ERROR);

	CompileShader(g_vertexShader);
	assert(glGetError() == GL_NO_ERROR);

	LoadShaderSource(g_fragmentShader, "fragment.txt");
	assert(glGetError() == GL_NO_ERROR);

	CompileShader(g_fragmentShader);
	assert(glGetError() == GL_NO_ERROR);

	glAttachObjectARB(g_programObject, g_vertexShader);
	assert(glGetError() == GL_NO_ERROR);

	glAttachObjectARB(g_programObject, g_fragmentShader);
	assert(glGetError() == GL_NO_ERROR);

	LinkProgram(g_programObject);
	assert(glGetError() == GL_NO_ERROR);

	g_environmentVariableLocation = glGetUniformLocationARB(g_programObject, "EnvironmentTexture");
	assert(glGetError() == GL_NO_ERROR);
}


void InitApplication()
{
	CheckExtensions();
	LoadShaders();
	LoadTextures();
}

void DestroyApplication()
{
	glDeleteTextures(1, &g_cubeMapTexture);
	GdiplusShutdown(g_gdiPlusToken);
	glDeleteObjectARB(g_programObject);
	glDeleteObjectARB(g_vertexShader);
	glDeleteObjectARB(g_fragmentShader);
}

void OnSize(int w, int h)
{
	SetUpProjectionMatrixAndViewPort(w, h, 60, 0.1, 10);
}