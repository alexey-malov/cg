#pragma once
#include "glapplication.h"
#include "Texture.h"
#include "AnimationController.h"

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
	void DrawRectangle()const;
	// ���� ������ �� ���������
	static const double FIELD_OF_VIEW;

	// ���������� �� ������� � ������� ���������� ���������
	static const double ZNEAR;
	static const double ZFAR;
	static const float ROTATION_SPEED;

	CAnimationController m_animationController;
	CTexture2D m_car;
	CTexture2D m_smile;
	CTexture2D m_sun;
	CTexture2D m_medved;

	float m_rotationAngle;
};
