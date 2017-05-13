// Export3ds.h : main header file for the EXPORT3DS application
//

#if !defined(AFX_EXPORT3DS_H__7525076E_EC30_4C58_9D9F_2016D6706E83__INCLUDED_)
#define AFX_EXPORT3DS_H__7525076E_EC30_4C58_9D9F_2016D6706E83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CExport3dsApp:
// See Export3ds.cpp for the implementation of this class
//

class CExport3dsApp : public CWinApp
{
public:
	CExport3dsApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExport3dsApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(CExport3dsApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPORT3DS_H__7525076E_EC30_4C58_9D9F_2016D6706E83__INCLUDED_)
