#pragma once

class cCamera; 
class cGrid; 
class cFrame;

class cMainGame
{
public:
	cMainGame();
	~cMainGame();

private:
	cCamera*				m_pCamera;
	cGrid*					m_pGrid;

	cFrame*					m_pRootFrame;

	LPD3DXFONT				m_pFont;
	ID3DXMesh*				m_p3DText;
public :
	void Setup();
	void Update(); 
	void Render(); 

	void WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	void Set_Light();
		
	void Create_Font();
	void Text_Render();
};

