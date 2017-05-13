// Texture.cpp: implementation of the CTexture class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Export3ds.h"
#include "Texture.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTexture::CTexture()
{
	m_Index = 0;
}

CTexture::~CTexture()
{
	Destroy();
}

void CTexture::SetParent(C3dObject *pParent)
{
	m_pParentObject = pParent;
}

BOOL CTexture::LoadBitmap(CString FileName)
{
	CFile file;

	Create();

	if (!file.Open((LPCTSTR)FileName, CFile::modeRead))
	{
		return FALSE;
	}

	CString ext;
	ext=file.GetFileName().Right(3);
	ext.MakeLower();
	if (ext!="bmp") 
	{
		file.Close();
		return FALSE;
	}

	m_Name = file.GetFileName();

	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;

	file.Read(&bfh, sizeof (bfh));
	file.Read(&bih, sizeof (bih));

	CByteArray Buf;

	int width = bih.biWidth;
	int height = bih.biHeight;
	if (height<0) height=-height;

	Buf.SetSize(width*height*3);

	for (int y=0; y<height; y++)
	{
		BYTE *pPixels = (bih.biHeight<0)?(Buf.GetData()+width*3*(height-1-y)):(Buf.GetData()+width*3*(y));
//		file.Read(pPixels, 3*width);
		for (int x=0; x<width; x++)
		{
			BYTE pix[3];
			file.Read(pix, sizeof (pix));
			*(pPixels++) = pix[2];
			*(pPixels++) = pix[1];
			*(pPixels++) = pix[0];
		}
	}

	// upload to video card
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, Buf.GetData());

	file.Close();
	return TRUE;
}

void CTexture::Destroy()
{
	if (m_Index)
	{
		glDeleteTextures(1, &m_Index);
		m_Index = 0;
	}
}

void CTexture::Bind()
{
	if (m_Index)
	{
		glBindTexture(GL_TEXTURE_2D, m_Index);
	}
}

void CTexture::Create()
{
	Destroy();
	glGenTextures(1, &m_Index);
	Bind();
}

CString CTexture::GetName()
{
	return m_Name;
}
