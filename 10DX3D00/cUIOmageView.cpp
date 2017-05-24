#include "stdafx.h"
#include "cUIOmageView.h"


cUIOmageView::cUIOmageView()
	:m_pTexture(NULL)
{
}


cUIOmageView::~cUIOmageView()
{
	SAFE_RELEASE(m_pTexture);
}

void cUIOmageView::SetTexture(char * szFullPath)
{
	D3DXIMAGE_INFO stImageInfo;
	
	m_pTexture = g_pTextureManager->GetTexture(szFullPath, &stImageInfo);

	m_stSize.nWidth = stImageInfo.Width;
	m_stSize.nHeight = stImageInfo.Height;
}

void cUIOmageView::Render(LPD3DXSPRITE pSprite)
{
	if (m_isHidden) return;

	pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	RECT rc;
	SetRect(&rc, 0, 0, m_stSize.nWidth, m_stSize.nHeight);

	pSprite->SetTransform(&m_matWorld);
	pSprite->Draw(m_pTexture, &rc, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 0),
		D3DCOLOR_ARGB(255, 255, 255, 255));

	pSprite->End();

	cUIObject::Render(pSprite);

}
