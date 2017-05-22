#include "stdafx.h"
#include "cWoman.h"
#include "cAseLoader.h"


cWoman::cWoman()
	:m_pRunFrame(NULL)
	, m_pStandingFrame(NULL)
{
}


cWoman::~cWoman()
{
	m_pRunFrame->Destroy();
	m_pStandingFrame->Destroy();
}

void cWoman::Setup()
{
	cWomanMove::Setup();

	cAseLoader load;
	m_pStandingFrame = load.LoadAse("woman/woman_01_all_stand.ASE");
	m_pStandingFrame->SetIsMove(false);
	m_pStandingFrame->Setup(false);

	cAseLoader load2;
	m_pRunFrame = load2.LoadAse("woman/woman_01_all.ASE");
	m_pRunFrame->SetIsMove(true);
	m_pRunFrame->Setup(false);
}

void cWoman::Update(iMap* pMap)
{
	cWomanMove::Update(pMap);

	if (m_IsMove)
	{
		m_pRunFrame->Update(m_pRunFrame->GetKeyFrame(), &m_matWorld);
		if (m_IsBooster) m_pRunFrame->SetfFrameSpeed(4);
		else m_pRunFrame->SetfFrameSpeed(2);
	}
	else
	{
		m_pStandingFrame->Update(m_pStandingFrame->GetKeyFrame(), &m_matWorld);
	}

}

void cWoman::Render()
{
	if (m_IsMove)	m_pRunFrame->Render();
	else m_pStandingFrame->Render();

}

