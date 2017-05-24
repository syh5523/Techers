#include "stdafx.h"
#include "cUIButton.h"


cUIButton::cUIButton()
	:m_eButtonState(E_NORMAL),
	m_pDelegate(NULL)
{
}


cUIButton::~cUIButton()
{
	for (int i = 0; i < E_STATE_CNT; ++i)
	{
		SAFE_RELEASE(m_aTexture[i]);
	}
}

void cUIButton::SetTexture(char * szNormal, char * szOver, char * szSelect)
{
	D3DXIMAGE_INFO stImageInfo;
	m_aTexture[E_NORMAL] = g_pTextureManager->GetTexture(szNormal, &stImageInfo);
	m_stSize.nWidth = stImageInfo.Width;
	m_stSize.nHeight = stImageInfo.Height;
	m_aTexture[E_MOUSEOVER] = g_pTextureManager->GetTexture(szOver, &stImageInfo);
	assert(m_stSize.nWidth == stImageInfo.Width &&
		m_stSize.nHeight == stImageInfo.Height);///이미지 사이즈가 다르다면 경고문을 출력해줌
	m_aTexture[E_SELECTED] = g_pTextureManager->GetTexture(szSelect, &stImageInfo);
	assert(m_stSize.nWidth == stImageInfo.Width &&
		m_stSize.nHeight == stImageInfo.Height);///이미지 사이즈가 다르다면 경고문을 출력해줌

}

void cUIButton::Update()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	RECT rc;
	SetRect(&rc,
		(int)m_matWorld._41,
		(int)m_matWorld._42,
		(int)m_matWorld._41 + (int)m_stSize.nWidth,
		(int)m_matWorld._42 + (int)m_stSize.nHeight);
	///외곽선이 딱 정해져 있을때 기준

	if (PtInRect(&rc, pt))
	{
		if (GetKeyState(VK_LBUTTON) & 0x8000)
		{
			if (m_eButtonState == E_MOUSEOVER)
			{
				m_eButtonState = E_SELECTED;
			}
		}
		else
		{
			if (m_eButtonState == E_SELECTED)
			{
				if (m_pDelegate) m_pDelegate->OnClick(this);
			}
			m_eButtonState = E_MOUSEOVER;
		}
	}
	else
	{
		m_eButtonState = E_NORMAL;
	}

	cUIObject::Update();
}

void cUIButton::Render(LPD3DXSPRITE pSprite)
{
	if (m_isHidden) return;
	pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	pSprite->SetTransform(&m_matWorld);

	RECT rc;
	SetRect(&rc, 0, 0, m_stSize.nWidth, m_stSize.nHeight);
	pSprite->Draw(m_aTexture[m_eButtonState],
		&rc,
		&D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(0, 0, 0),
		D3DCOLOR_ARGB(255, 255, 255, 255));
	pSprite->End();

	cUIObject::Render(pSprite);
}