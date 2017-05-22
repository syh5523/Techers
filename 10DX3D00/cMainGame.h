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
	cWomanMove*					m_pWoman;
	//cWomanMove*				m_pWoman_Canyon;
	iMap*					m_pMap;
public :
	void Setup();
	void Update(); 
	void Render(); 

	void WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	void Set_Light();
	void Setup_HeightMap();


};

