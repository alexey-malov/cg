#include "StdAfx.h"
#include "MatrixFilter.h"

using namespace Gdiplus;

CMatrixFilter::CMatrixFilter(void)
:m_matrix(1, 1.0f)
,m_matrixWidth(1)
,m_matrixHeight(1)
{
}

CMatrixFilter::~CMatrixFilter(void)
{
}

void CMatrixFilter::InitializeMatrix(size_t width, size_t height, const float * initialValues, float scale)
{
	if ((width > 0) && (width <= MAX_MATRIX_SIZE) && 
		(height > 0) && (height <= MAX_MATRIX_SIZE))
	{
		m_matrixWidth = width;
		m_matrixHeight = height;

		if (initialValues != NULL)
		{
			m_matrix.assign(&initialValues[0], &initialValues[width * height]);
		}
		else
		{
			// ������ ������� ��������� ������ � �������� �� �������� 
			m_matrix.assign(width * height, 0.0f);

			// ������������� ����������� ������� � 1
			SetMatrixValue(width / 2, height / 2, 1);
		}

		if (scale != 1.0f)
		{
			// �������� ��� �������� ������� �� ���������� ���������
			std::transform(
				m_matrix.begin(), // ������� � ������� �������� �������
				m_matrix.end(), // �� ����������
				m_matrix.begin(), // ��������� �������� � ���� �������
				std::bind1st(	
					std::multiplies<float>(), 
					scale) // bind1st ������� ������� ������� ��
					// �������� ������� std::multiplies
					// ��������� ������� �������� �������� 
					// �������� ������� �� scale
				);
		}

	}
	else
	{
		throw std::out_of_range("Matrix dimensions is out of range");
	}
}

void CMatrixFilter::SetMatrixValue(size_t row, size_t column, float value)
{
	if ((row < m_matrixHeight) && (column < m_matrixWidth))
	{
		m_matrix[row * m_matrixWidth + column] = value;
	}
	else
	{
		throw std::invalid_argument("Row or Column index is out of range");
	}
}

Gdiplus::PixelFormat CMatrixFilter::ChoosePreferableOutputPixelFormat(Gdiplus::Bitmap & bitmapData)const
{
	return bitmapData.GetPixelFormat();
}

std::auto_ptr<Gdiplus::Bitmap> CMatrixFilter::ApplyFilter(Gdiplus::Bitmap & srcBitmap)
{
	// �������������� ���������� ��� �������� ����� ������������ ��������
	int w = srcBitmap.GetWidth();
	int h = srcBitmap.GetHeight();
	int matrixHalfWidth = m_matrixWidth / 2;
	int matrixHalfHeight = m_matrixHeight / 2;

	// ������� ��������� ����������� ��� �������� ����������,
	// ����� ����������� ������� ����������� �������������� ����� 
	// ������� �������� �����������
	std::auto_ptr<Bitmap> pResult(
		new Bitmap(w, h, ChoosePreferableOutputPixelFormat(srcBitmap)));
	// ������ �� �������������� �����������
	Bitmap & dstBitmap = *pResult;

	// ������� � ��������� ������������� ������� ��� ������������� ������� �������
	Rect lockRect(0, 0, srcBitmap.GetWidth(), srcBitmap.GetHeight());

	// �������� ������ ������ �� ���� �������� ��������� ����������� ��� ������
	BitmapData srcData;
	srcBitmap.LockBits(&lockRect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &srcData);
	BYTE const * pSrcData = reinterpret_cast<BYTE const*>(srcData.Scan0);

	// �������� ������ ������ �� ���� �������� ��������������� ����������� ��� ������
	BitmapData dstData;
	dstBitmap.LockBits(&lockRect, Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &dstData);
	BYTE * pDstData = reinterpret_cast<BYTE*>(dstData.Scan0);


	// ���� �� ������� �����������
	for (int y0 = 0; y0 < h; ++y0)
	{
		// ��������� ����� ������ ������ � �������� y0 � �������� �����������
		const DWORD * pSrcLine = 
			reinterpret_cast<const DWORD*>(pSrcData + (srcData.Stride * y0));

		// ��������� ����� ������ ������ � �������� y0 � �������������� �����������
		DWORD * pDstLine = 
			reinterpret_cast<DWORD*>(pDstData + (dstData.Stride * y0));

		// ���� �� �������� �����������
		for (int x0 = 0; x0 < w; ++x0)
		{
			int y = y0 - matrixHalfHeight;

			// ������ ��������� �������� ����� �������
			float r = 0, g = 0, b = 0, a = 0;

			// ���� �� ������� �������
			for (size_t row = 0; row < m_matrixHeight; ++row, ++y)
			{
				int x = x0 - matrixHalfWidth;

				// ������������ ���������� y �������� �����������
				int srcY = min(h - 1, max(y, 0));

				// ��������� ����� ������ ������ ��������� �����������, ����������� � �������
				const DWORD * pSrcFilterLine = 
					reinterpret_cast<const DWORD*>(pSrcData + (srcData.Stride * srcY));

				// ���� �� �������� �������
				for (size_t column = 0; column < m_matrixWidth; ++column, ++x)
				{
					// ������������ ���������� x �������� �����������
					int srcX = min(w - 1, max(x, 0));

					// �������� ���� �������, ������������ � ������� �������
					// � ���� 32-������� ������ ����� � ������� 0xAARRGGBB
					DWORD c = pSrcFilterLine[srcX];

					// �������� ������� ����������� �� ������� � 
					// ���������� ��� ��� ���������� ���������� �����
					// ����� �������
					float coeff = GetMatrixValue(row, column);
					a += ((c >> 24) & 0xff) * coeff;
					r += ((c >> 16) & 0xff) * coeff;
					g += ((c >> 8) & 0xff) * coeff;
					b += (c & 0xff) * coeff;
				}
			}

			// �������� ���� ��������� ������� � ���� 32-�������
			// ������ ����� � ������� 0xAARRGGBB
			DWORD srcColor = pSrcLine[x0];

			// ������������ ���� ������� (������ �������� ����������� 
			// � ����������� �� ���� �������)
			AdjustColor(r, g, b, a, srcColor);

			// ��������� ���� ��������� ������� � ���������� ���
			// � �������� ��������� �����������
			pDstLine[x0] = MakeARGB(Clamp(a), Clamp(r), Clamp(g), Clamp(b));
		}
	}

	// �������� UnlockBits ��� ����, ����� ������������������ � ����������
	// �������� ������� ������� � ��������� � ��������������� ������������.
	srcBitmap.UnlockBits(&srcData);
	// ��������� � ��������������� ����������� ��� ������� ������ ��� ������
	// �� �� ��� ������ UnlockBits ������ �� ������� �������������� ��������
	// ����� �������� � �������������� ��������� �����������
	dstBitmap.UnlockBits(&dstData);

	return pResult;
}

void CMatrixFilter::AdjustColor(float & /*r*/, float & /*g*/, float & /*b*/, float & /*a*/, DWORD /*srcColor*/)const
{
	// ��������� ��� ��������� r, g, b � a ������������ �����
	// ���� ����� ����� ���� ���������� � ����������� ������
}

