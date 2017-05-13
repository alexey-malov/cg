#include "StdAfx.h"
#include "MyApplication.h"

// ���������� �� ������� ��������� ��������� ������������� ������
const double CMyApplication::ZNEAR = 0.5;
// ���������� �� ������ ��������� ��������� ������������� ������
const double CMyApplication::ZFAR = 10;
// ���� ������ �� ���������
const double CMyApplication::FIELD_OF_VIEW = 60;

CMyApplication::CMyApplication(const char * title, int width, int height)
:CGLApplication(title, width, height)
,m_carTexture(0)
{
}

CMyApplication::~CMyApplication(void)
{
	// ������� ���������� ������
	if (m_carTexture)
	{
		glDeleteTextures(1, &m_carTexture);
	}
}

void CMyApplication::OnInit()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(1, 1, 1, 1);

	// ��������� �������� �� �����
	m_carTexture = LoadTexture(L"car.jpg");
}

void CMyApplication::OnDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0, -1, 2, 0, 0, 0, 0, 1, 0);

	// �������� ����� ��������� ��������
	glEnable(GL_TEXTURE_2D);
	// � ������ �������� ���������� ������
	glBindTexture(GL_TEXTURE_2D, m_carTexture);
	/*
	������ �������������, �������� ��� ������ �� ��� ������ ���������� ����������
	     Y
	     |
	 0---|---3
	 |   |   |
	 |   |   |
	-----+------X
	 |   |   |
	 |   |   |
	 1---|---2
	     |
	*/
	glBegin(GL_QUADS);
	{
		// ������� ����� ����
		glTexCoord2f(0, 0);
		glVertex3f(-1, 1, 0);

		// ������ ����� ����
		glTexCoord2f(0, 1);
		glVertex3f(-1, -1, 0);

		// ������ ������ ����
		glTexCoord2f(1, 1);
		glVertex3f(1, -1, 0);

		// ������� ������ ����
		glTexCoord2f(1, 0);
		glVertex3f(1, 1, 0);
	}	
	glEnd();
}

GLuint CMyApplication::LoadTexture(std::wstring const& fileName)
{
	// ��������� ����������� ��� ������ GDI+
	Gdiplus::Bitmap bmp(fileName.c_str());

	if (bmp.GetLastStatus() != Gdiplus::Ok)
	{
		throw std::runtime_error("Error loading texture file");
	}

	// ���������� ������ �������� ��� ������������� 
	// � ������� Bitmap::Lock � �������� gluBuild2DMipmaps
	Gdiplus::PixelFormat pixelFormat = 0;
	GLint textureFormat = 0;
	GLint colorComponents = 0;
	GLint internalFormat = 0;

	// ���� �� � ����������� �����-�����?
	if ((bmp.GetPixelFormat() & PixelFormatAlpha) != 0)
	{
		colorComponents = 4;
		pixelFormat = PixelFormat32bppARGB;
		textureFormat = GL_BGRA_EXT;
		internalFormat = GL_RGBA;
	}
	else
	{
		colorComponents = 3;
		pixelFormat = PixelFormat24bppRGB;
		textureFormat = GL_BGR_EXT;
		internalFormat = GL_RGB;
	}

	// ������� � ��������� ������������� ������� �����������, 
	// ������ � ������ ������� �� ����� ��������
	Gdiplus::Rect lockRect(0, 0, bmp.GetWidth(), bmp.GetHeight());

	// �������� ������ ������ ��� ������ � ������ ���������� �����������
	Gdiplus::BitmapData bitmapData;
	bmp.LockBits(&lockRect, Gdiplus::ImageLockModeRead, 
		pixelFormat, &bitmapData);

	GLuint textureName = 0;
	// ���������� ���� ���������� ������������� ����������� �������
	glGenTextures(1, &textureName);

	// ������ �������� ���������� ������ � ������ ���������������
	// (� ��� ��� ���� �� ������� ������� �����������)
	glBindTexture(GL_TEXTURE_2D, textureName);

	// ������ ��������� ���-������� ��� ������������ �����������
	// � ����������� �� � ���������� ����������� �������
	// ������ ���������� ����������� ��� 0 ������ �����������
	glTexImage2D(
		GL_TEXTURE_2D,
		0,		// ������� �����������
		internalFormat,
		bitmapData.Width,
		bitmapData.Height,
		0,		// ������ ����� �������� (0 - ��� �����, 1 - ����� � 1 �������)
		textureFormat,
		GL_UNSIGNED_BYTE,
		bitmapData.Scan0
		);
	// ������ ��������� ���������� �������� ��� ���������� � ��� ����������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// ��������� ������ � ������� ������
	bmp.UnlockBits(&bitmapData);

	// ���������� ������������� ���������� �����������
	return textureName;
}

void CMyApplication::OnReshape(int width, int height)
{
	glViewport(0, 0, width, height);

	// ��������� ����������� ������ ���������� ������� ����
	double aspect = double(width) / double(height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FIELD_OF_VIEW, aspect, ZNEAR, ZFAR);
	glMatrixMode(GL_MODELVIEW);
}
