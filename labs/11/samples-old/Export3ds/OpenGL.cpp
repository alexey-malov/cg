#include "stdafx.h"

#define stringer(x) #x
#define InitExtension(type,extname) ((extname)=(type)wglGetProcAddress(#extname))

/*
PFNWGLGETEXTENSIONSSTRINGEXTPROC wglGetExtensionsStringEXT=NULL;

BOOL EXT_draw_range_elements = FALSE;
PFNGLDRAWRANGEELEMENTSEXTPROC glDrawRangeElementsEXT=NULL;

BOOL ARB_vertex_blend = FALSE;
PFNGLWEIGHTPOINTERARBPROC glWeightPointerARB=NULL;
PFNGLVERTEXBLENDARBPROC glVertexBlendARB=NULL;

BOOL ARB_multitexture = FALSE;
PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB=NULL;
PFNGLACTIVETEXTUREARBPROC glActiveTextureARB=NULL;

BOOL EXT_swap_control = FALSE;
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT=NULL;

BOOL EXT_compiled_vertex_array = FALSE;
PFNGLLOCKARRAYSEXTPROC glLockArraysEXT = NULL;
PFNGLUNLOCKARRAYSEXTPROC glUnlockArraysEXT = NULL;
*/

#define QueryExtension(extlist,extname) (strstr((const char*)(extlist),(const char*)(extname))!=NULL)

//
BOOL InitExtensions()
{
#define CheckExtension(type,name) {if (InitExtension(type,name)==NULL) return FALSE;}
/*
	CheckExtension(PFNWGLGETEXTENSIONSSTRINGEXTPROC,wglGetExtensionsStringEXT);

	const unsigned char *pGLExtensions = glGetString(GL_EXTENSIONS);

	// Init Vertex Blend functions
	if (QueryExtension(pGLExtensions,"GL_ARB_vertex_blend"))
	{
		ARB_vertex_blend = TRUE;
		InitExtension(PFNGLWEIGHTPOINTERARBPROC, glWeightPointerARB);
		InitExtension(PFNGLVERTEXBLENDARBPROC, glVertexBlendARB);
	}

	// Init draw range elements extension
	if (QueryExtension(pGLExtensions,"GL_EXT_draw_range_elements"))
	{
		EXT_draw_range_elements = TRUE;
		InitExtension(PFNGLDRAWRANGEELEMENTSEXTPROC,glDrawRangeElementsEXT);
	}

	// Init ARB multi texture extension
	if (QueryExtension(pGLExtensions, "GL_ARB_multitexture"))
	{
		ARB_multitexture = TRUE;
		InitExtension(PFNGLCLIENTACTIVETEXTUREARBPROC, glClientActiveTextureARB);
		InitExtension(PFNGLACTIVETEXTUREARBPROC, glActiveTextureARB);
	}

	if (QueryExtension(pGLExtensions, "GL_EXT_swap_control"))
	{
		InitExtension(PFNWGLSWAPINTERVALEXTPROC,wglSwapIntervalEXT);

		wglSwapIntervalEXT(1);
	}
	if (QueryExtension(pGLExtensions, "GL_EXT_compiled_vertex_array"))
	{
		EXT_compiled_vertex_array = TRUE;
		InitExtension(PFNGLLOCKARRAYSEXTPROC,glLockArraysEXT);
		InitExtension(PFNGLUNLOCKARRAYSEXTPROC,glUnlockArraysEXT);
	}
*/
	//bVertexBlendingSupported = FALSE;
	//InitParameters();
	return TRUE;
}

