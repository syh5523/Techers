#pragma once

class cCamera; 
class cWoman;
class cWomanMove;
class iMap;

class cMainGame
{
public:
	cMainGame();
	~cMainGame();

private:
	cCamera*				m_pCamera;
	cWomanMove*				m_pWoman;
	iMap*					m_pMap;

	LPD3DXSPRITE			m_pSprite;
	D3DXIMAGE_INFO			m_stImageInfo;
	LPDIRECT3DTEXTURE9		m_pTexture;
public :
	void Setup();
	void Update(); 
	void Render(); 

	void WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	void Set_Light();
	void Setup_HeightMap();

	void Setup_UI();
	void UI_Render();
};

