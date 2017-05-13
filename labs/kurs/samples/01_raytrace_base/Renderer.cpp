#include "stdafx.h"
#include "Renderer.h"
#include "FrameBuffer.h"

using namespace boost::interprocess::ipcdetail;
using boost::mutex;

CRenderer::CRenderer(void)
:m_rendering(0)			// ����������� ���������: "�� ������"
,m_stopping(0)			// �� ���������������
,m_totalChunks(0)		// ����� ���������� �������������� ������ �����������
,m_renderedChunks(0)	// ���������� ������������ ������ �����������
{
}

CRenderer::~CRenderer(void)
{
	// ������������� ������ �������� ������, ���� �� ��� �� ����������
	Stop();
}

// ����������� �� � ������ ������ ���������� ����������� � ������ �����?
bool CRenderer::IsRendering()const
{
	// ��������� ���������������� ������� �������� ���������� m_rendering
	return atomic_read32(const_cast<volatile boost::uint32_t*>(&m_rendering)) != 0;
}

// ���������� �� ����, ���������� � ������������� ���������� ������
bool CRenderer::IsStopping()const
{
	// ��������� ���������������� ������� �������� ���������� m_stopping
	return atomic_read32(const_cast<volatile boost::uint32_t *>(&m_stopping)) != 0;
}

bool CRenderer::SetRendering(bool rendering)
{
	/*
	��������� (� ������ ���������� �������) ���������� ���������� 
	������������������ ��������:
		wasRendering = m_rendering != 0;
		if (m_rendering == !rendering)
		{
			m_rendering = rendering;
		}
	*/
	bool wasRendering = atomic_cas32(&m_rendering, rendering, !rendering) != 0;

	// ��������, ��������� �� ��������� �������� �����
	return wasRendering != rendering;
}

bool CRenderer::SetStopping(bool stopping)
{
	/*
	��������� (� ������ ���������� �������) ���������� ���������� 
	������������������ ��������:
		wasStopping = wasStopping != 0;
		if (m_stopping == !stopping)
		{
			m_stopping = stopping;
		}
	*/
	bool wasStopping = atomic_cas32(&m_stopping, stopping, !stopping) != 0;

	// ��������, ��������� �� ��������� �������� �����
	return wasStopping != stopping;
}

bool CRenderer::GetProgress(unsigned & renderedChunks, unsigned & totalChunks)const
{
	// ����������� ������� �� ����� ������ ������� ������
	mutex::scoped_lock lock(m_mutex);

	// �������� ���������������� ������� �������� ����������
	// m_renderedChunks � m_totalChunks
	renderedChunks = atomic_read32(const_cast<volatile boost::uint32_t*>(&m_renderedChunks));
	totalChunks = atomic_read32(const_cast<volatile boost::uint32_t*>(&m_totalChunks));;

	// ��������, ��� �� ����� ����������� ���� ����������
	return (totalChunks > 0) && (renderedChunks == totalChunks);
}

/*
��������� �������� ������ �� ���������� ����������� � ������ �����
*/
void CRenderer::RenderFrame(CFrameBuffer & frameBuffer)
{
	// ���������� ������ � ������ ������ �����, ����� ������ ��� �� ��������
	// ������ ������ CFrameBuffer
	const int width = frameBuffer.GetWidth();
	const int height = frameBuffer.GetHeight();

	/*
	������ ����� ���������� ������ ����������� 
	��� ������ ����������� ����� ���������� 1 ������ ������ �����
	*/
	atomic_write32(&m_totalChunks, height);

	// ��������� ��� ������ ������ �����
	// ��� ���������� ��������� OpenMP �������� ����� �� ������� �����������
	// ����� ����������� � ������������ �������
#ifdef _OPENMP
	#pragma omp parallel for schedule(dynamic)
#endif
	for (int y = 0; y < height; ++y)
	{
		// ����� y-� ������ �����������
		boost::uint32_t * rowPixels = frameBuffer.GetPixels(y);

		// ���� �� x ����������� ������, ���� �� ���� �������
		// �� ������������ �� ��������� ���������� �����������
		// ���������� break ��� ������ �� ����� ����� ������������
		// ������ (����������� OpenMP)
		if (!IsStopping())
		{
			// ��������� ��� ������� � ������
			for (int x = 0; x < width; ++x)
			{
				// ��������� ���� �������� ������� � ���������� ��� � ����� �����
				rowPixels[size_t(x)] = CalculatePixelColor(x, y, width, height);
			}

			// ����������� ���������������� ������� ���������� ������������ ������ (�����) �����������
			atomic_inc32(&m_renderedChunks);	//-V1206
		}
	}

	// ���������� ���� ���������
	SetStopping(false);
	// �������� �� ��������� ���������� �����������
	SetRendering(false);
}

// ��������� ������������ ����� � ������ ����� � ������� ������
// ���������� false, ���� ��� �� ���� ��������� ������ ����� ����������� ������
bool CRenderer::Render(CFrameBuffer & frameBuffer)
{
	// �������� ������� � ����� ����������
	if (!SetRendering(true))
	{
		// � ������ ������ ��� ���� ���������� ����������� � ������������ ������
		return false;
	}

	// ��������� ������ � ����� (��� �������� � ��������� ������) ������ ������
	// ������ �� ���������� ������ ������ Render
	mutex::scoped_lock lock(m_mutex);

	// ������� ����� �����
	frameBuffer.Clear();

	// ���������� ���������� ������������ � ����� ���������� ������ �����������
	// ������������ � ���, ��� ��� ������ �� �������	
	atomic_write32(&m_totalChunks, 0);
	atomic_write32(&m_renderedChunks, 0);

	// ���������� ������ �� ��������� ���������� �����������
	if (SetStopping(false))
	{
		// ���� ��� �� ������� �������� ������ ������ ������ �� ���������,
		// �������, ��������� ���� "���� ���������� �����������"
		SetRendering(false);
		return false;
	}

	// ��������� ����� RenderFrame � ������������ ������, ��������� ���
	// ������ �� ������ frameBuffer
	m_thread = boost::thread(&CRenderer::RenderFrame, this, boost::ref(frameBuffer));

	// �������, ������� � ���, ��� ������� ���������� ����������� �������
	return true;
}

void CRenderer::Stop()
{
	// ���� ���������� ���������� �����������
	if (IsRendering())
	{
		// �������� ������, ������������ ���������� �����������, � �������������
		// ��������� ������
		SetStopping(true);

		// ���������� ��������� ������ �������� ������
		m_thread.join();

		// ���������� ���� ���������, ���� ����� �������� ���� ������ �� ������ SetStopping(true)
		SetStopping(false);
	}
}

// ��������� ���� ������� ������ �����
boost::uint32_t CRenderer::CalculatePixelColor(int x, int y, unsigned frameWidth, unsigned frameHeight)const
{
	// ���� ����� � ��� �������� - ����������� ���� ����� �������� ������������
	// (��. ���������)

	// � ���������� ���� ����� ����� ��������� ������ ��� �������������
	// ���� ����, ����������� ����� ������ �������, ������ �����

	double x0 = 2.0 * x / frameWidth - 1.5;
	double y0 = 2.0 * y / frameHeight - 1.0;

	// �����������: ���������, �� �������� �� ������� ����� � ������� ������� ���������
	double rho = sqrt((x0 - 0.25) * (x0 - 0.25) + y0 * y0);
	double theta = atan2(y0, x0 - 0.25);
	double rhoC = 0.5 - 0.5 * cos(theta);
	if (rho <= rhoC)
	{
		return 0x000000;
	}

	double re = 0, im = 0;

	// ������� ���������� �������� "�������� ����� Zi � �������������"
	int iterCount = 10000;
	// ����������� ���������� ������������������ Z(i+1) = (Z(i))^2 + C, ���
	//	Z(0) = 0 + 0*i
	//	C = x0 + y0*i
	while ((iterCount > 0) && (re * re + im * im < 1e18))
	{
		double re1 = re * re - im * im + x0;
		im = 2 * re * im + y0;
		re = re1;
		--iterCount;
	}

	// ������������ ����� �� ������ ���������� ��������
	boost::uint8_t r = static_cast<boost::uint8_t>((iterCount / 3) & 0xff);
	boost::uint8_t g = static_cast<boost::uint8_t>(iterCount & 0xff);
	boost::uint8_t b = static_cast<boost::uint8_t>((iterCount / 2) & 0xff);
	boost::uint8_t a = 0xff;
	return (a << 24) | (r << 16) | (g << 8) | b;
}
