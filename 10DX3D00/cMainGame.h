#pragma once

// >> : 
class cCubePC; 
class cCamera; 
class cGrid; 
// << : 
class cCubeMan; 
class cGroup;
class iMap;

class cMainGame
{
public:
	cMainGame();
	~cMainGame();

	// >> :
private :

public :
	void Setup();
	void Update(); 
	void Render(); 
	// << :

		// >> : 
private : 
	//cCubePC*				m_pCubePC; 
	cCamera*				m_pCamera; 
	cGrid*					m_pGrid; 
	cCubeMan*				m_pCubeMan; 
	vector<cGroup*>			m_vecGroup;
 
	LPDIRECT3DTEXTURE9		m_pTexture; 
	vector<ST_PT_VERTEX>	m_vecVertex; 
 
	iMap*					m_pMap;		//<<
public :
	void WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	void Obj_Render();
	void Set_Light();

	void Load_Surface();				//<<

};

