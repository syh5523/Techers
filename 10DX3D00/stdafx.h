// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: reference additional headers your program requires here

#include <vector>
#include <string>
#include <map>
#include <set>
#include <list>
#include <assert.h>

#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

using namespace std;
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
#include <iostream>

using namespace std;

extern HWND	g_hWnd;
#define SAFE_RELEASE(p) { if(p) p->Release(); p = NULL; }
#define SAFE_DELETE(p) { if(p) delete p; p = NULL; }	// << :
#define SAFE_ADD_REF(p) { if(p) p->AddRef();}

// >> : 
#define SINGLETONE(class_name) \
		private : \
			class_name(void) ;  \
			~class_name(void) ; \
		public :\
			static class_name* GetInstance() \
			{ \
				static class_name instance ; \
				return &instance ; \
			} 

struct ST_PN_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3	n;
	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL };
};

struct ST_PC_VERTEX
{
	D3DXVECTOR3 p; 
	D3DCOLOR	c; 
	enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
};

struct ST_PT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR2 t;
	enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };
};

struct ST_PNT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	D3DXVECTOR2 t;

	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 };
};

struct ST_POS_SAMPLE
{
	int			n;
	D3DXVECTOR3 v;
	ST_POS_SAMPLE()
		:n(0), v(0,0,0)
	{
	}
};

struct ST_ROT_SAMPLE
{
	int				n;
	D3DXQUATERNION	q;

	ST_ROT_SAMPLE()
		:n(0)
	{
		D3DXQuaternionIdentity(&q);
	}
};

struct ST_SIZEN
{
	int nWidth;
	int nHeight;
	ST_SIZEN(): nWidth(0), nHeight(0) {}
	ST_SIZEN(int _w, int _h): nWidth(_w), nHeight(_h){}
};


#define SYNTHESIZE(varType, varName, funName)\
protected: varType varName;\
public: inline varType Get##funName(void) const { return varName; }\
public: inline void Set##funName(varType var){ varName = var; }

#define SYNTHESIZE_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: inline varType& Get##funName(void) { return varName; }\
public: inline void Set##funName(varType& var){ varName = var; }

#define SYNTHESIZE_ADD_REF(varType, varName, funName)\
protected: varType varName;\
public: virtual varType Get##funName(void) const { return varName; }\
public: virtual void Set##funName(varType var)\
	{ \
		if(varName != var) \
			{\
				SAFE_ADD_REF(var);\
			} \
		SAFE_RELEASE(varName); \
		varName = var;\
	}


// << :
#include "iMap.h"

#include "cDeviceManager.h"
#include "cTextureManager.h"
#include "cObjectManager.h"
#include "cFontManager.h"
#include "cTimeManager.h"