#pragma once
#include "glapplication.h"
#include "Cube.h"

class CMyApplication : public CGLApplication
{
public:
	CMyApplication(const char * title, int width, int height);
	~CMyApplication(void);
protected:
	virtual void OnInit();
	virtual void OnDisplay();
	virtual void OnReshape(int width, int height);
private:
	CCube m_cube;

	// Угол обзора по вертикали 
	static const double FIELD_OF_VIEW;

	static const double Z_NEAR;
	static const double Z_FAR;

	// Размер стороны куба
	static const double CUBE_SIZE;
};
