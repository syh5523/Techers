#include "stdafx.h"
#include "cWomanMove.h"

cWomanMove::cWomanMove()
	:m_IsMove(false), m_IsBooster(false)
	, m_fRotY(0.0f)
	, m_vDirection(0, 0, 0)
	, m_vPosition(0, 0, 0)
	, m_fSpeed(0.8f)
{
	D3DXMatrixIdentity(&m_matWorld);
}


cWomanMove::~cWomanMove()
{
}

void cWomanMove::Setup()
{
}

void cWomanMove::Update(iMap* pMap)
{


	D3DXVECTOR3 vPosition = m_vPosition;
	m_IsMove = false;

	if (GetKeyState('W') & 0x8000)
	{
		m_IsMove = true;
		vPosition = m_vPosition - m_vDirection * m_fSpeed;
	}
	if (GetKeyState('S') & 0x8000)
	{
		m_IsMove = true;
		vPosition = m_vPosition + m_vDirection * m_fSpeed;
	}
	if (GetKeyState('A') & 0x8000)
	{
		m_fRotY -= 0.03f;
	}
	if (GetKeyState('D') & 0x8000)
	{
		m_fRotY += 0.03f;
	}

	if (m_IsMove)
	{
		if (GetKeyState('Q') & 0x8000)
		{
			m_fSpeed = 0.16f;
			m_IsBooster = true;
		}
		else if (GetKeyState('E') & 0x8000)
		{
			m_fSpeed = 0.08f;
			m_IsBooster = false;
		}
	}
	else
	{
		m_fSpeed = 0.03f;
		m_IsBooster = false;
	}


	//Rotate
	D3DXMATRIXA16 matR, matRY;
	D3DXMatrixRotationY(&matRY, m_fRotY);
	matR = matRY;

	//방향 설정
	m_vDirection = D3DXVECTOR3(0, 0, 1);
	D3DXVec3TransformNormal(&m_vDirection, &m_vDirection, &matR);


	if (pMap)
	{
		if (pMap->GetHeight(vPosition.x, vPosition.y, vPosition.z))
		{
			m_vPosition = vPosition;
		}
	}

	//Translate
	D3DXMATRIXA16 matT;
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	m_matWorld = matR * matT;

}

void cWomanMove::Render()
{
}



//바닥들의 버텍스
//넘겨줄 포지션

//만약 트루면 = 현재포지션 변경