#pragma once

class CSphere
{
public:
	CSphere(float radius, unsigned slices, unsigned stacks);
	void Draw()const;
	~CSphere(void);
private:
	void DrawSphere()const;

	// ������������� ����������� ������, ������������ ��������� �����
	mutable GLuint m_displayList;
	float m_radius;		// ������
	unsigned m_slices;	// ���������� ����������
	unsigned m_stacks;	// ���������� ����������
};
