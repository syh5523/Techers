#pragma once

class cCamera; 
class cWoman;

class cMainGame
{
public:
	cMainGame();
	~cMainGame();

private:
	cCamera*				m_pCamera;
	cWoman*					m_pWoman;

public :
	void Setup();
	void Update(); 
	void Render(); 

	void WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	void Set_Light();



};

