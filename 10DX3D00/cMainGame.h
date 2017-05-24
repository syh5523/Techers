#pragma once

class cCamera; 
class cWomanMove;
class iMap;
class cUIObject;
#include "cUIButton.h"

class cMainGame : public iButtonDelegate
{
public:
	cMainGame();
	~cMainGame();

private:
	cCamera*				m_pCamera;
	cWomanMove*				m_pWoman;
	iMap*					m_pMap;

	LPD3DXSPRITE			m_pSprite;
	cUIObject*				m_pUIRoot;
public :
	void Setup();
	void Update(); 
	void Render(); 

	void WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	void Set_Light();
	void Setup_HeightMap();

	void Setup_UI();
	void UI_Render();


	virtual void OnClick(cUIButton* pSender) override;
};

