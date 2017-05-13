#pragma once
#include "glapplication.h"

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
	// ������� ������ ������ ������ �������� �� �������� ����
	static void RotateCamera(GLfloat rotateX, GLfloat rotateY);
	// ���������� ���������������� ������� �������������-����
	static void NormalizeModelViewMatrix(void);

	// ����, ����������������� � ��������� ����� ������ ����
	bool m_leftButtonPressed;

	// ������ ���������� ������� ����
	int	m_mouseX;
	int m_mouseY;

	// ������� ����
	int m_windowWidth;
	int m_windowHeight;

	// ������ ������� ������� (�� ��������� � �����������)
	static const double FRUSTUM_SIZE;

	// ���������� �� ������ �� ������ ������������ ���
	static const double DISTANCE_TO_ORIGIN;
};
