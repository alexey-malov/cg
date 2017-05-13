// Texture.h: interface for the CTexture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTURE_H__32B15D6C_8D4A_40BE_B038_C33983B58DC0__INCLUDED_)
#define AFX_TEXTURE_H__32B15D6C_8D4A_40BE_B038_C33983B58DC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class C3dObject;

class CTexture  
{
protected:
	CString m_Name;
	void Create();
	C3dObject *m_pParentObject;
	UINT	m_Index;
public:
	CString GetName();
	void Bind();
	void Destroy();
	BOOL LoadBitmap(CString FileName);
	void SetParent(C3dObject *pParent);
	CTexture();
	virtual ~CTexture();

};

#endif // !defined(AFX_TEXTURE_H__32B15D6C_8D4A_40BE_B038_C33983B58DC0__INCLUDED_)
