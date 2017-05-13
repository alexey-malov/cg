#include "StdAfx.h"
#include "RotationController.h"
#include "Vector3.h"
#include "IApplication.h"
#include "IRotationControllerListener.h"

CRotationController::CRotationController(int windowWidth, int windowHeight)
:m_leftButtonPressed(false)
,m_windowWidth(windowWidth)
,m_windowHeight(windowHeight)
{
}

void CRotationController::SetModelViewMatrix(CMatrix4d const& matrix)
{
	m_modelViewMatrix = matrix;
}

CMatrix4d const& CRotationController::GetModelViewMatrix()const
{
	return m_modelViewMatrix;
}

void CRotationController::OnReshape(int windowWidth, int windowHeight)
{
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;
}

void CRotationController::OnMouse(int button, int state, int x, int y)
{
	// ������� �� ����� ������ ����
	if (button == GLUT_LEFT_BUTTON)
	{
		// ��������� ��������� ����� ������ ����
		m_leftButtonPressed = (state == GLUT_DOWN);
		// ��������� ���������� ����
		m_mouseX = x;
		m_mouseY = y;
	}
}

void CRotationController::OnMotion(int x, int y)
{
	// ���� ������ ����� ������ ����
	if (m_leftButtonPressed)
	{
		// ��������� �������� ������� ����
		int dx = x - m_mouseX;
		int dy = y - m_mouseY;

		// ��������� ���� �������� ������ ���� Y � X ��� ������� ���������
		// �� �������� ���� �� ���� X � Y
		GLfloat rotateX = GLfloat(dy) * 180 / m_windowHeight;
		GLfloat rotateY = GLfloat(dx) * 180 / m_windowWidth;

		RotateCamera(rotateX, rotateY);

		// ��������� ������� ���������� ����
		m_mouseX = x;
		m_mouseY = y;
	}
}

// ������� ������ ������ ������ �������� �� �������� ����
void CRotationController::RotateCamera(GLfloat rotateX, GLfloat rotateY)
{
	// ������������ ������ ���� x � y ������
	m_modelViewMatrix.Rotate(rotateX, m_modelViewMatrix[0], m_modelViewMatrix[4], m_modelViewMatrix[8]);
	m_modelViewMatrix.Rotate(rotateY, m_modelViewMatrix[1], m_modelViewMatrix[5], m_modelViewMatrix[9]);

	// � ���� ��������� ������ ����� ��������� �����������, �������,
	// ������������ ����� ������ �������� ��������
	// ��� �� ����������� ����� ������ ����������� ������� �������������-����
	// �������� �� ����������������
	m_modelViewMatrix.Normalize();

	// ������� �� ���������� ������� �������������-����
	class CRotationEvent
	{
	public:
		void operator()(IRotationControllerListener * pListener)const
		{
			pListener->OnRotationControllerUpdate();
		}
	};

	// �������������� �������
	DispatchEvent(CRotationEvent());
}
