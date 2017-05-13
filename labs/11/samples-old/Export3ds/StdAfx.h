// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__B79946BE_D120_4A15_BE4D_A2B9729C44AF__INCLUDED_)
#define AFX_STDAFX_H__B79946BE_D120_4A15_BE4D_A2B9729C44AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <AFXTEMPL.H>

#include <gl/GL.H>
#include <gl/GLU.H>
//#include <GL/glext.h>
//#include <gl/WGLEXT.H>
#include <3dsftk.h>

#include "nv_math.h"
#include "nv_algebra.h"


#define WRITE_STR(f,s) {int Len = (s).GetLength(); (f).Write(&Len,sizeof(Len));(f).Write((LPCTSTR)(s), (s).GetLength()+1);}
#define WRITE_BIN(f,b) (f).Write(&(b), sizeof(b))
#define WRITE_SIZE(f,arr) {int tmp = (arr).GetSize();(f).Write(&tmp,sizeof(tmp));}
#define WRITE_ARR(f,arr) if ((arr).GetSize()) (f).Write((arr).GetData(),sizeof((arr)[0])*(arr).GetSize())


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__B79946BE_D120_4A15_BE4D_A2B9729C44AF__INCLUDED_)
