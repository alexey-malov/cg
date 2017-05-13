// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__1E23D997_5776_4DFE_BB5A_060AFFCC2C1D__INCLUDED_)
#define AFX_STDAFX_H__1E23D997_5776_4DFE_BB5A_060AFFCC2C1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

// link glew statically
#define GLEW_STATIC

#include <stdio.h>
#include <gl/glew.h>
#include <glut.h>
#include <string>
#include <stdexcept>
#include <windows.h>
#include <math.h>
static const double M_PI = 3.1415926535897932384626433832795;

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__1E23D997_5776_4DFE_BB5A_060AFFCC2C1D__INCLUDED_)
