#pragma once
#include "glapplication.h"

#include "shaders.h"
#include "RotationController.h"
#include "Sphere.h"
#include "texture.h"

class CMyApplication : public CGLApplication
{
public:
	CMyApplication(const char * title, int width, int height);
	~CMyApplication(void);
protected:
	virtual void OnInit();
	virtual void OnDisplay();
	virtual void OnReshape(int width, int height);
	virtual void OnMouse(int button, int state, int x, int y);
	virtual void OnMotion(int x, int y);
private:
	void InitShaders();
	void LoadTextures();

	CRotationController m_rotationController;

	// ���� ������ �� ���������
	static const double FIELD_OF_VIEW;
	static const double ZNEAR;
	static const double ZFAR;

	CTexture2D m_sphereTexture;
	CTexture2D m_cloudsTexture;
	CSphere m_sphere;

	// ����������� ������
	CProgram m_program;
	// ������������ uniform-���������� mainTexture
	GLint m_mainTextureSamplerLocation;
	// ������������ uniform-���������� cloudsTexture
	GLint m_cloudsTextureSamplerLocation;
};
