#pragma once
#include "glapplication.h"
#include "Texture.h"
#include "Matrix4.h"
#include "RotationController.h"
#include "AnimationController.h"
#include "IRotationControllerListener.h"


class CMyApplication 
	: public CGLApplication
	, private IRotationControllerListener
{
public:
	CMyApplication(const char * title, int width, int height);
	~CMyApplication();
private:
	// ����������� ������� �� ����������
	virtual void OnInit();
	virtual void OnDisplay();
	virtual void OnReshape(int width, int height);
	virtual void OnIdle();

	// ������������� ���������������� ������ �����
	void InitFrameBuffer();

	// ������ ������ �� ��������, ��������� � �������� 
	// ������ ����� ���������������� ������ �����
	void DrawObjectToTexture()const;

	// �������� ����������� �� ���������� ���������
	// ����������� ��������
	void OnRotationControllerUpdate();

	// ��������� �������� ����� � ������� �������������
	static void SetupViewportAndProjectionMatrix(
		int width, int height, 
		double fov, double znear, double zfar);

	// ��������� ������
	static const double FIELD_OF_VIEW;;
	static const double ZNEAR;
	static const double ZFAR;

	// �������� �������� ������� ������ ���� X, Y, Z
	static const double X_ROTATION_SPEED;
	static const double Y_ROTATION_SPEED;
	static const double Z_ROTATION_SPEED;

	// ������ � ������ ���������������� ������ �����
	static const GLuint FRAME_BUFFER_WIDTH;
	static const GLuint FRAME_BUFFER_HEIGHT;

	// ��� ���������������� ������ �����
	GLuint m_frameBuffer;
	// ��� ���������������� ������ �������
	GLuint m_depthBuffer;

	// ��������, ����������� ���� ������ ����� �� ��������������� 
	// ������ �����
	CTexture2D m_colorBufferTexture;

	// ������ � ������ ����
	int m_windowWidth;
	int m_windowHeight;

	// ������� �������������-����, ������������ ��� ������ ������� 
	// �� ��������������� ����� �����
	CMatrix4d m_modelViewFrameBuffer;

	CRotationController m_rotationController;
	CAnimationController m_animationController;
};
