#pragma once
#include "glapplication.h"
#include "SincSurface.h"
#include "Frame.h"

class CMyApplication :
	public CGLApplication
{
public:
	CMyApplication(const char *title, int width, int height);
	~CMyApplication(void);
protected:
	// Перегружаем необходимые виртуальные методы родительского класса
	virtual void OnInit();
	virtual void OnIdle();
	virtual void OnDisplay(void);
	virtual void OnReshape(int width, int height);
	virtual void OnMouse(int button, int state, int x, int y);

private:
	void SetupLightsAndMaterials();
	void SetupCamera();

	CSincSurface m_surface;
	CFrame	m_frame;

	float m_rotationAngle;

	GLenum m_polygonMode;

	DWORD m_lastTick;

	static const float ROTATION_SPEED;
	static const double FOV;
	static const double ZNEAR;
	static const double ZFAR;
};
