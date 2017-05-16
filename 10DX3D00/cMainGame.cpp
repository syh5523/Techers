#include "stdafx.h"
#include "cMainGame.h"
#include "cDeviceManager.h"

#include "cCamera.h"
#include "cGrid.h"
#include "cAseLoader.h"
#include "cFrame.h"
#include "cObjectLoader.h"
#include "cMtlTex.h"
#include "cGroup.h"

cMainGame::cMainGame()
	: m_pCamera(NULL)
	, m_pGrid(NULL)
	, m_pFont(NULL)
	, m_p3DText(NULL)
	, m_pMeshTeapot(NULL), m_pMeshSphere(NULL)
	, m_pObjMesh(NULL)
{
}


cMainGame::~cMainGame()
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);
	m_pRootFrame->Destroy();
	SAFE_RELEASE(m_pFont);
	SAFE_RELEASE(m_p3DText);
	SAFE_RELEASE(m_pMeshTeapot);
	SAFE_RELEASE(m_pMeshSphere);

	SAFE_RELEASE(m_pObjMesh);
	for each(auto p in m_vecObjMtlTex)
	{
		SAFE_RELEASE(p);
	}

	g_pObjectManager->Destroy();
	g_pTextureManager->Destroy();
	g_pDeviceManager->Destroy();
}

void cMainGame::Setup()
{

	cAseLoader load;
	m_pRootFrame = load.LoadAse("woman/woman_01_all.ASE");

	m_pCamera = new cCamera;
	m_pCamera->Setup(NULL); 

	m_pGrid = new cGrid; 
	m_pGrid->Setup();

	cObjectLoader l;
	m_pObjMesh = l.LoadMesh(m_vecObjMtlTex, "obj", "map.obj");

	l.Load(m_vecGroup, "obj", "map.obj");


	Set_Light(); //<<
	//Create_Font();//<<
	Setup_MeshObject();
}

void cMainGame::Update()
{
	if (m_pCamera) m_pCamera->Update();
	if (m_pRootFrame) m_pRootFrame->Update(m_pRootFrame->GetKeyFrame(), NULL);
}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(47, 121, 112),
		1.0f, 0);

	g_pD3DDevice->BeginScene();

	
	//if (m_pGrid) m_pGrid->Render(); 
	//if (m_pRootFrame) m_pRootFrame->Render();
	Mesh_Render();
	//Text_Render();  //<<

	g_pD3DDevice->EndScene();

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void cMainGame::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pCamera)
	{
		m_pCamera->WndProc(hwnd, message, wParam, lParam); 
	}
}

void cMainGame::Set_Light()
{
	D3DLIGHT9 stLight;
	{
		ZeroMemory(&stLight, sizeof(D3DLIGHT9));
		stLight.Type = D3DLIGHT_DIRECTIONAL;
		stLight.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
		stLight.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
		stLight.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
		D3DXVECTOR3 vDir(1.0f, -1.0f, 1.0f);
		D3DXVec3Normalize(&vDir, &vDir);
		stLight.Direction = vDir;
		g_pD3DDevice->SetLight(0, &stLight);
	}

	g_pD3DDevice->LightEnable(0, true);
}
//>>
void cMainGame::Create_Font()
{
	{
		D3DXFONT_DESC fd;
		ZeroMemory(&fd, sizeof(D3DXFONT_DESC));
		fd.Height = 50; 
		fd.Width = 25;
		fd.Weight = FW_MEDIUM;
		fd.Italic = false;
		fd.CharSet = DEFAULT_CHARSET;
		fd.OutputPrecision = OUT_DEFAULT_PRECIS;
		fd.PitchAndFamily = FF_DONTCARE;
		AddFontResource("font/umberto.ttf");
		strcpy_s(fd.FaceName, "umberto");

		D3DXCreateFontIndirect(g_pD3DDevice, &fd, &m_pFont);
	}


	{
		HDC hdc = CreateCompatibleDC(0);
		LOGFONT lf;
		ZeroMemory(&lf, sizeof(LOGFONT));
		lf.lfHeight = 25;
		lf.lfWidth = 12;
		lf.lfWeight = 500;  //(0 ~1000 범위)
		lf.lfItalic = false;
		lf.lfUnderline = false;
		lf.lfStrikeOut = false;
		lf.lfCharSet = DEFAULT_CHARSET;
		strcpy_s(lf.lfFaceName, "굴림체");

		HFONT hFont;
		HFONT hFontOld;

		hFont = CreateFontIndirect(&lf);
		hFontOld = (HFONT)SelectObject(hdc, hFont);
		D3DXCreateText(g_pD3DDevice, hdc, "Direct3D", 0.001f, 0.01f, &m_p3DText, 0, 0);

		SelectObject(hdc, hFontOld);
		DeleteObject(hFont);
		DeleteDC(hdc);
	}
}

void cMainGame::Text_Render()
{
	{
		string sText("ABC 123 #@#$ 가나다라");
		RECT rc;
		SetRect(&rc, 100, 100, 101, 100);
		m_pFont->DrawTextA(NULL, sText.c_str(), sText.length(), &rc, DT_LEFT | DT_TOP | DT_NOCLIP,
			D3DCOLOR_XRGB(255, 255, 0));
	}


	{
		D3DXMATRIXA16 matWorld, matS, matR, matT;
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixIdentity(&matS);
		D3DXMatrixIdentity(&matR);
		D3DXMatrixIdentity(&matT);

		D3DXMatrixScaling(&matS, 1.0f, 1.0f, 100.0f);
		D3DXMatrixRotationX(&matR, -D3DX_PI / 4.0f);
		D3DXMatrixTranslation(&matT, -2.0f, 2.0f, 0.0f);

		matWorld = matS * matR * matT;

		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
		m_p3DText->DrawSubset(0);
	}
}
void cMainGame::Setup_MeshObject()
{
	D3DXCreateTeapot(g_pD3DDevice, &m_pMeshTeapot, NULL);
	D3DXCreateSphere(g_pD3DDevice, 0.5f, 50,50, &m_pMeshSphere, NULL);

	ZeroMemory(&m_stMtlTeapot, sizeof(D3DMATERIAL9));
	m_stMtlTeapot.Ambient = D3DXCOLOR(0.0f, 0.7f, 0.7f, 1.0f);
	m_stMtlTeapot.Diffuse = D3DXCOLOR(0.0f, 0.7f, 0.7f, 1.0f);
	m_stMtlTeapot.Specular = D3DXCOLOR(0.0f, 0.7f, 0.7f, 1.0f);

	ZeroMemory(&m_stMtlSphere, sizeof(D3DMATERIAL9));
	m_stMtlSphere.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.0f, 1.0f);
	m_stMtlSphere.Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.0f, 1.0f);
	m_stMtlSphere.Specular = D3DXCOLOR(0.7f, 0.7f, 0.0f, 1.0f);
}
void cMainGame::Mesh_Render()
{
	D3DXMATRIXA16 matWorld, matS, matR;
	
	
	/*{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixIdentity(&matS);
		D3DXMatrixIdentity(&matR);

		D3DXMatrixScaling(&matS, 1.0f, 1.0f, 1.0f);
		matWorld = matS * matR;

		D3DXMatrixTranslation(&matWorld, 0, 0, 5);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

		g_pD3DDevice->SetMaterial(&m_stMtlTeapot);
		m_pMeshTeapot->DrawSubset(0);
	}

	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixIdentity(&matS);
		D3DXMatrixIdentity(&matR);

		D3DXMatrixScaling(&matS, 1.0f, 1.0f, 1.0f);
		matWorld = matS * matR;
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

		g_pD3DDevice->SetMaterial(&m_stMtlSphere);
		m_pMeshSphere->DrawSubset(0);
	}*/

	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixIdentity(&matS);
		D3DXMatrixIdentity(&matR);

		D3DXMATRIXA16 matWorld, matS, matR;
		D3DXMatrixScaling(&matS, 0.01f, 0.01f, 0.01f);
		D3DXMatrixRotationX(&matR, -D3DX_PI / 2.0f);
		matWorld = matS * matR;
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

		for (size_t i =0; i< m_vecObjMtlTex.size(); ++i)
		{
			g_pD3DDevice->SetTexture(0, m_vecObjMtlTex[i]->GetTexture());
			g_pD3DDevice->SetMaterial(&m_vecObjMtlTex[i]->GetMaterial());
			m_pObjMesh->DrawSubset(i);
		}
	}
}
//<<