#pragma once

/*
����� "������������ �����" ��� ������������ ���� ���������
*/
class CFrame
{
public:
	// ������������� ������������� ������
	// size - ����� �������,
	CFrame(float size = 1, float endSize = 0.1);
	// ������ ������������ ���
	void Draw(void)const;
private:
	float m_size;
	float m_endSize;
};
