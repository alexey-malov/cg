#pragma once

/*
����� "������������ �����" ��� ������������ ���� ���������
*/
class CFrame
{
public:
	CFrame(float size = 1, float arrowSize = 0.1);
	void Draw(void)const;
private:
	float m_size;
	float m_arrowSize;
};
