#pragma once

class iCanyon;

class cWomanMove
{
public:
	cWomanMove();
	~cWomanMove();

protected:
	SYNTHESIZE(bool, m_IsMove, IsMove)
	SYNTHESIZE(bool, m_IsBooster, IsBooster)

	float					m_fRotY;
	float					m_fSpeed;
	D3DXVECTOR3				m_vDirection;
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vPosition, Position)
	D3DXMATRIXA16			m_matWorld;

public:
	virtual void Setup();
	virtual void Update();
};

