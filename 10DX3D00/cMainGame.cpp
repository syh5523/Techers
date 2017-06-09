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

DWORD FtoDw(float f)
{
	return *((DWORD*)&f);
}

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

	SetUp_Particle();

	m_pSkinnedMesh = new cSkinnedMesh;
	m_pSkinnedMesh->Setup("Zealot","zealot.X");
}

void cMainGame::Update()
{
	if (m_pCamera) m_pCamera->Update();

	m_pSkinnedMesh->Update();

	Update_Particle();
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

	Render_Particle();
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

void cMainGame::SetUp_Particle()
{
	m_vecParticleVertext.resize(1000);

	for (int i = 0; i < m_vecParticleVertext.size(); ++i)
	{
		float fRadius = rand() % 100 / 10.0f;
		m_vecParticleVertext[i].p = D3DXVECTOR3(0, 0, fRadius);

		D3DXVECTOR3 vAngle = D3DXVECTOR3(D3DXToRadian(rand() % 3600 / 10.0f),
			D3DXToRadian(rand() % 3600 / 10.0f),
			D3DXToRadian(rand() % 3600 / 10.0f));

		D3DXMATRIX matRX, matRY, matRZ, matWorld;
		D3DXMatrixRotationX(&matRX, vAngle.x);
		D3DXMatrixRotationY(&matRY, vAngle.y);
		D3DXMatrixRotationZ(&matRZ, vAngle.z);
		matWorld = matRX * matRY * matRZ;

		D3DXVec3TransformCoord(&m_vecParticleVertext[i].p,
			&m_vecParticleVertext[i].p, &matWorld);

		m_vecParticleVertext[i].c = D3DCOLOR_ARGB(255, 180, 70, 20);
	}

	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_POINTSIZE, FtoDw(5.0f));
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(0.0f));
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(0.0f));
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(1.0f));	//나중에 바꿔서 확인해보기
	g_pD3DDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDw(0.0f));
	g_pD3DDevice->SetRenderState(D3DRS_POINTSIZE_MAX, FtoDw(100.0f));

	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	//g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG0, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	g_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
}

void cMainGame::Update_Particle()
{
	static int nAlpha = 0;
	static int nDelta = 4;

	nAlpha += nDelta;
	if (nAlpha > 255)
	{
		nAlpha = 255;
		nDelta *= -1;
	}

	if (nAlpha < 0)
	{
		nAlpha = 0;
		nDelta *= -1;
	}

	for (int i = 0; i < m_vecParticleVertext.size(); ++i)
	{
		if (i % 2) continue;

		m_vecParticleVertext[i].c = D3DCOLOR_ARGB(
			nAlpha, 180, 70, 20);
 	}
}

void cMainGame::Render_Particle()
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);

	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	g_pD3DDevice->SetTexture(0, g_pTextureManager->GetTexture("batman.png"));

	g_pD3DDevice->DrawPrimitiveUP(D3DPT_POINTLIST, m_vecParticleVertext.size(),
		&m_vecParticleVertext[0], sizeof(ST_PC_VERTEX));

	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
}
