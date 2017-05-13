#pragma once
#include "glapplication.h"
#include "Texture.h"
#include "Shaders.h"
#include "GaussianBlurFilter.h"

class CMyApplication : public CGLApplication
{
public:
	CMyApplication(const char * title, int width, int height);
protected:
	virtual void OnInit();
	virtual void OnDisplay();
	virtual void OnReshape(int width, int height);
	virtual void OnMouse(int button, int state, int x, int y);
	virtual void OnSpecialKey(int key, int x, int y);
private:
	void LoadTextures();
	void LoadShaders();

	// ������ ��������
	CGaussianBlurFilter m_blurFilter;
	// ����, ��������������� � ������������� ���������� �������
	bool m_blur;
	// ���������
	GLfloat m_sigma;
	// ������ �������
	static const int BLUR_RADIUS;

	// ������� ����������� �����������
	unsigned m_textureWidth;
	unsigned m_textureHeight;

	// ��������
	CTexture2D m_texture;

	// ��������� ���������, ����������� �������� ������
	CProgram m_blurProgram;
};
