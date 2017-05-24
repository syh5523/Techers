#pragma once
#include "cObject.h"

class cUIButton;
class iButtonDelegate
{
public:
	virtual void OnClick(cUIButton* pSender) = 0;
};

class cUIObject : public cObject
{
public:
	cUIObject();
	virtual ~cUIObject();


protected:
	vector<cUIObject*>		m_vecChild;
	D3DXMATRIXA16			m_matWorld;
	bool					m_isHidden;
	SYNTHESIZE(D3DXVECTOR3, m_vPosition, Position)
	SYNTHESIZE(cUIObject*,	m_pParent, Parent)
	SYNTHESIZE(ST_SIZEN,	m_stSize, Size)

	//--tag
	SYNTHESIZE(int,			m_nTag, nTag)

public:
	virtual void SetPosition(float x, float y, float z = 0);
	virtual void AddChild(cUIObject* pChild);
	virtual void Update();
	virtual void Render(LPD3DXSPRITE pSprite);
	virtual void Destroy();

	virtual cUIObject* FindChildByTag(int nTag);
};

