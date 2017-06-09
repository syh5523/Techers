#pragma once

class cCamera; 
class cSkinnedMesh;

class cMainGame
{
public:
	cMainGame();
	~cMainGame();

private:
	cCamera*				m_pCamera;
	cSkinnedMesh*			m_pSkinnedMesh;
public :
	void Setup();
	void Update(); 
	void Render(); 

	void WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	void Set_Light();


	vector<ST_PC_VERTEX>			m_vecParticleVertext;
	void SetUp_Particle();
	void Update_Particle();
	void Render_Particle();
};

