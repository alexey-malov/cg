#pragma once

#include "IApplicationListener.h"
#include "Matrix4.h"
#include "IEventDispatcher.h"

class IRotationControllerListener;

class CRotationController 
	: public CApplicationListener
	, public CEventDispatcherImpl<
		IEventDispatcher<IRotationControllerListener>, 
		IRotationControllerListener>
{
public:
	CRotationController(int windowWidth, int windowHeight);

	// ��������� � ��������� ������� �������������-����,
	// ������� �������� ���������� ��������
	void SetModelViewMatrix(CMatrix4d const& matrix);
	CMatrix4d const& GetModelViewMatrix()const;


private:
	// ����������� ������� � ������� ������ ���� � ����������� 
	// �������, � ����� ������� �� ��������� �������� ����
	virtual void OnReshape(int windowWidth, int windowHeight);
	virtual void OnMouse(int button, int state, int x, int y);
	virtual void OnMotion(int x, int y);

	// �������� ������ �� �������� ����
	void RotateCamera(GLfloat rotateX, GLfloat rotateY);

	bool m_leftButtonPressed;
	int m_mouseX;
	int m_mouseY;
	int m_windowWidth;
	int m_windowHeight;

	// ������� �������������-����, ����������� ������������ ��������
	CMatrix4d m_modelViewMatrix;
};
