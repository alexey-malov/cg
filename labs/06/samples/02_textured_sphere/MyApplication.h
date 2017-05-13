#pragma once
#include "glapplication.h"
#include "SkyBox.h"
#include "RotationController.h"
#include "OmniLight.h"
#include "AnimationController.h"
#include "Planet.h"


class CMyApplication : public CGLApplication
{
public:
	CMyApplication(const char * title, int width, int height);
	~CMyApplication(void);
protected:
	virtual void OnInit();
	virtual void OnDisplay();
	virtual void OnIdle();
	virtual void OnReshape(int width, int height);
	virtual void OnMouse(int button, int state, int x, int y);
	virtual void OnMotion(int x, int y);
	virtual void OnKeyboard(unsigned char key, int x, int y);
private:
	// ������ ������
	void DrawSkyBox()const;
	// ������ �����
	void DrawEarth()const;
	// ���� ������ �� ���������
	static const double FIELD_OF_VIEW;

	// ���������� �� ������� � ������� ���������� ���������
	static const double ZNEAR;
	static const double ZFAR;

	// ����� ������������ ������
	// (������������� ��� ������ ������� ������)
	GLenum m_polygonMode;

	// ����������� �������� � ��������
	CRotationController m_rotationController;
	CAnimationController m_animationController;

	// �������� �������� �����
	COmniLight m_light;

	// ����� � ���
	CPlanet m_earth;
	CSkyBox m_skyBox;
};
