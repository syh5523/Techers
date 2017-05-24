#pragma once
#include "cUIObject.h"
class cUITextView :	public cUIObject
{
public:
	cUITextView();
	virtual ~cUITextView() override;

protected:
	SYNTHESIZE(cFontManager::eFontType, m_eFontType, FontType);
	SYNTHESIZE(string, m_sText, Text);
	SYNTHESIZE(DWORD, m_dwDrawTextFormat, DrawTextFormat);
	SYNTHESIZE(D3DCOLOR, m_dwTextColor, TextColor);
public:
	virtual void Render(LPD3DXSPRITE pSprite) override;
};

