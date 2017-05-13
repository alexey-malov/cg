#pragma once
#include "glapplication.h"
#include "Shaders.h"
#include "Texture.h"

class CMyApplication :
	public CGLApplication
{
public:
	CMyApplication(const char * title, int width, int height);
	~CMyApplication(void);
protected:
	virtual void OnDisplay();
	virtual void OnInit();
	virtual void OnReshape(int width, int height);
private:
	void InitShaders();
	void InitTextures();
private:
	// ��������� �������
	CProgram m_program;
	CShader m_vertexShader;
	CShader m_fragmentShader;
	CShader m_geometryShader;
	// ������������ ���������� Size � ����������� �������
	GLint m_sizeLocation;
	// ������������ ���������� TextureMap � ����������� �������
	GLint m_textureMapLocation;

	// �������
	CTexture2D m_texture;
};
