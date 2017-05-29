#include "stdafx.h"
#include "cMainGame.h"
#include "cDeviceManager.h"

#include "cCamera.h"
#include "cSkinnedMesh.h"

enum 
{
	E_BUTTON_OK = 11,
	E_BUTTON_OK_CANCEL,
	E_BUTTON_OK_EXIT,
	E_TEXT_VIEW,
};

cMainGame::cMainGame()
	: m_pCamera(NULL)
{
}


cMainGame::~cMainGame()
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pSkinnedMesh);

	g_pFontManager->Destroy();
	g_pObjectManager->Destroy();
	g_pTextureManager->Destroy();
	g_pDeviceManager->Destroy();
}

void cMainGame::Setup()
{
	
	m_pCamera = new cCamera;
	m_pCamera->Setup(NULL);

	Set_Light();

	m_pSkinnedMesh = new cSkinnedMesh;
	m_pSkinnedMesh->Setup("Zealot","zealot.X");
}

void cMainGame::Update()
{
	if (m_pCamera) m_pCamera->Update();

	m_pSkinnedMesh->Update();
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
	m_pSkinnedMesh->Render(NULL);
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

	switch (message)
	{
	case WM_RBUTTONDOWN:
		static int n = 0;
		//m_pSkinnedMesh->SetAnimationIndex(n++);
		m_pSkinnedMesh->SetAnimationIndexBlend(n++);
		break;
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