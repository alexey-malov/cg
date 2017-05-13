
// sample04_simple_mfc_application.h : main header file for the sample04_simple_mfc_application application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// Csample04_simple_mfc_applicationApp:
// See sample04_simple_mfc_application.cpp for the implementation of this class
//

class Csample04_simple_mfc_applicationApp : public CWinAppEx
{
public:
	Csample04_simple_mfc_applicationApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

public:
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Csample04_simple_mfc_applicationApp theApp;
