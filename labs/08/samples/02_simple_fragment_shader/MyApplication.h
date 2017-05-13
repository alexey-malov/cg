#pragma once
#include "glapplication.h"

#include "shaders.h"

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
	void InitShaders();

	CProgram m_program;
};
