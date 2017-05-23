#include "stdafx.h"
#include "cMainGame.h"
#include "cDeviceManager.h"

#include "cCamera.h"
#include "cWoman.h"
#include "cHeightMap.h"

cMainGame::cMainGame()
	: m_pCamera(NULL)
	, m_pWoman(NULL)
	, m_pSprite(NULL)
{
}


cMainGame::~cMainGame()
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pWoman);
	SAFE_RELEASE(m_pSprite);
	
	g_pObjectManager->Destroy();
	g_pTextureManager->Destroy();
	g_pDeviceManager->Destroy();
}

void cMainGame::Setup()
{
	cWoman* pWoman = new cWoman;
	pWoman->Setup();
	m_pWoman = pWoman;
	
	m_pCamera = new cCamera;
	m_pCamera->Setup(&m_pWoman->GetPosition());

	Setup_UI();
	Setup_HeightMap();
	Set_Light();
}

void cMainGame::Update()
{
	if (m_pCamera) m_pCamera->Update();
	
	if (m_pWoman) m_pWoman->Update(m_pMap);
}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(47, 121, 112),
		1.0f, 0);

	g_pD3DDevice->BeginScene();
	///-----------------------------------------------------------------------------------
	if (m_pMap) m_pMap->Render();
	if (m_pWoman) m_pWoman->Render();
	UI_Render();
	///-----------------------------------------------------------------------------------
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

void cMainGame::Setup_HeightMap()
{
	cHeightMap *pMap = new cHeightMap;
	pMap->Setup("HeightMapData/", "HAT.raw", "HAT.jpg");
	m_pMap = pMap;
}

void cMainGame::Setup_UI()
{
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);

	//m_pTexture = g_pTextureManager->GetTexture("UI/±èÅÂÈñ.jpg");
	D3DXCreateTextureFromFileEx(g_pD3DDevice, "UI/±èÅÂÈñ.jpg", D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, &m_stImageInfo,
		NULL, &m_pTexture);
}

void cMainGame::UI_Render()
{
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	RECT rc;
	SetRect(&rc, 0, 0, m_stImageInfo.Width, m_stImageInfo.Height);

	D3DXMATRIXA16 matT, matR, matS, mat;
	D3DXMatrixIdentity(&matT);
	D3DXMatrixIdentity(&matR);
	D3DXMatrixIdentity(&mat);

	static float fAngle = 0.0f;
	fAngle += 0.01f;

	D3DXMatrixRotationZ(&matR, fAngle);

	D3DXMatrixTranslation(&matT, 500, 200, 0.0f);

	mat = matR * matT;

	m_pSprite->SetTransform(&mat);
	
	m_pSprite->Draw(m_pTexture, &rc, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(-300, -200, 0), D3DCOLOR_ARGB(120, 255, 255, 255));
		
	m_pSprite->End();
}
