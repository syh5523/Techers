#include "stdafx.h"
#include "cMainGame.h"
#include "cDeviceManager.h"

#include "cCamera.h"
#include "cWoman.h"
#include "cHeightMap.h"

#include "cUIOmageView.h"
#include "cUITextView.h"

enum 
{
	E_BUTTON_OK = 11,
	E_BUTTON_OK_CANCEL,
	E_BUTTON_OK_EXIT,
	E_TEXT_VIEW,
};

cMainGame::cMainGame()
	: m_pCamera(NULL)
	, m_pWoman(NULL)
	, m_pUIRoot(NULL)
	, m_pSprite(NULL)
{
}


cMainGame::~cMainGame()
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pWoman);
	SAFE_RELEASE(m_pSprite)
	
	SAFE_DELETE(m_pMap);
	if (m_pUIRoot) m_pUIRoot->Destroy();
	g_pFontManager->Destroy();
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

	m_pUIRoot->Update();
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
	pMap->Setup("HeightMapData/", "HeightMap.raw", "terrain.jpg");
	m_pMap = pMap;
}

void cMainGame::Setup_UI()
{
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);

	cUIOmageView* pImageView = new cUIOmageView;
	pImageView->SetPosition(0, 0, 0);
	pImageView->SetTexture("UI/panel-info.png");
	m_pUIRoot = pImageView;


	cUITextView* pTextView = new cUITextView;
	pTextView->SetText("좀비출현");
	pTextView->SetSize(ST_SIZEN(300, 200));
	pTextView->SetPosition(100, 100);
	pTextView->SetDrawTextFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	pTextView->SetTextColor(D3DCOLOR_XRGB(255, 255, 0));
	pTextView->SetnTag(E_TEXT_VIEW);

	m_pUIRoot->AddChild(pTextView);

	cUIButton* pButtonOnOK = new cUIButton;
	pButtonOnOK->SetTexture("UI/btn-med-up.png", "UI/btn-med-over.png", "UI/btn-med-down.png");
	pButtonOnOK->SetPosition(135, 330);
	pButtonOnOK->SetDelegate(this);
	pButtonOnOK->SetnTag(E_BUTTON_OK);
	m_pUIRoot->AddChild(pButtonOnOK);

}

void cMainGame::UI_Render()
{

	if (m_pUIRoot)
		m_pUIRoot->Render(m_pSprite);
}

void cMainGame::OnClick(cUIButton * pSender)
{
	cUITextView* pTextView = (cUITextView*)m_pUIRoot->FindChildByTag(E_TEXT_VIEW);

	if (pSender->GetnTag() == E_BUTTON_OK)
	{
		pTextView->SetText("확인");
	}
	else
	{

	}
}
