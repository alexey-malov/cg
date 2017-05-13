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
private:
	// ���� ������ �� ���������
	static const double FIELD_OF_VIEW;

	// ���������� �� ������� � ������� ���������� ���������
	static const double ZNEAR;
	static const double ZFAR;
};
