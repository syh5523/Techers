#pragma once
#include "cUIObject.h"
class cUIOmageView : public cUIObject
{
public:
	cUIOmageView();
	virtual ~cUIOmageView();

protected:
	LPDIRECT3DTEXTURE9 m_pTexture;

public:
	virtual void SetTexture(char* szFullPath);
	virtual void Render(LPD3DXSPRITE pSprite) override;
};

