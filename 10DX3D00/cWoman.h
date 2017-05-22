#pragma once

#include "cFrame.h"
#include "cWomanMove.h"
#include "cAseLoader.h"

class cWoman : public cWomanMove
{
public:
	cWoman();
	~cWoman();

private:
	cFrame*					m_pRunFrame;
	cFrame*					m_pStandingFrame;
public:
	virtual void Setup() override;
	virtual void Update() override;
	void Render();
};

