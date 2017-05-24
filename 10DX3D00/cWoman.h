#pragma once

#include "cFrame.h"
#include "cWomanMove.h"
#include "cAseLoader.h"

class cWoman : public cWomanMove
{
public:
	cWoman();
	virtual ~cWoman() override;

private:
	cFrame*					m_pRunFrame;
	cFrame*					m_pStandingFrame;
public:
	virtual void Setup() override;
	virtual void Update(iMap* pMap) override;
	virtual void Render() override;
};

