#pragma once
#include "glapplication.h"
#include "OmniLight.h"
#include "RotationController.h"
#include "Material.h"


class CMyApplication : public CGLApplication
{
public:
	CMyApplication(const char * title, int width, int height);
	~CMyApplication(void);
protected:
	virtual void OnInit();
	virtual void OnDisplay();
	virtual void OnReshape(int width, int height);
	virtual void OnMotion(int x, int y);
	virtual void OnMouse(int button, int state, int x, int y);
	virtual void OnKeyboard(unsigned char key, int x, int y);
private:
	// ���� ������ �� ���������
	static const double FIELD_OF_VIEW;

	// ���������� �� ������� � ������� ���������� ���������
	static const double ZNEAR;
	static const double ZFAR;
	
	COmniLight m_light;
	CMaterial m_teapotMaterial;
	CRotationController m_rotationController;
	bool m_fogEnabled;
};
