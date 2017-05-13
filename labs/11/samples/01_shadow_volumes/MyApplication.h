#pragma once
#include "glapplication.h"
#include "Cube.h"
#include "RotationController.h"
#include "IRotationControllerListener.h"
#include "OmniLight.h"
#include "Material.h"
#include "AnimationController.h"

class CMyApplication 
	: public CGLApplication
	, private IRotationControllerListener
{
public:
	CMyApplication(const char * title, int width, int height);
private:
	virtual void OnIdle();
	virtual void OnInit();
	virtual void OnDisplay();
	virtual void OnReshape(int width, int height);
	virtual void OnRotationControllerUpdate();
private:
	// ������ �������� ����� � ���� �����
	void DrawLightSource()const;
	// ������ ����������, ���� ������������ �����
	void DrawScene(bool enableLighting)const;
	// ������ ������� ����� ����
	void DrawShadowVolume()const;
	// ������ ����� �������� ������ ����
	void DrawShadowVolumeEdges()const;
private:
	CCube m_cube;

	// ��������� ������ (ZFAR = +�������������)
	static const double FOV;
	static const double ZNEAR;

	// ����������� ����������� ��������� ������
	static const float EXTRUSION_FACTOR;

	// ����������� �������� � ��������
	CRotationController m_rotationController;
	CAnimationController m_animationController;

	// �������� �������� ����� � ��� ��������� � ������������
	COmniLight m_light;
	CVector3f m_lightPosition;

	// �������� �������� �����
	CMaterial m_material;

	// ���� �������� ��������� ����� (0..2pi)
	float m_lightAnimationPhase;

	// ���� �������� (���� ��������) �����
	float m_torusAnimationPhase;
};
