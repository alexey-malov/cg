#pragma once
#include "glapplication.h"

#include "shaders.h"
#include "AnimationController.h"
#include "MorphShape.h"


class CMyApplication : public CGLApplication
{
public:
	CMyApplication(const char * title, int width, int height);
	~CMyApplication(void);
protected:
	virtual void OnInit();
	virtual void OnDisplay();
	virtual void OnReshape(int width, int height);
	virtual void OnIdle();
private:
	void InitShaders();

	// ���������� ������ � ������
	static const int NUMBER_OF_VERTICES;
	// ������������ �������
	static const float SHAPE_MAX_RADIUS;
	// ����������� ������
	static const float SHAPE_MIN_RADIUS;
	// ������ ������ �������� ��������
	static const float ANIMATION_PERIOD;

	CAnimationController m_animationController;

	// ���������������� ������
	CMorphShape m_morphShape;

	// ����������� ������
	CProgram m_program;

	// ������� ���� ��������
	float m_phase;
};
