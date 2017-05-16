#pragma once

class cCamera; 
class cGrid; 
class cFrame;
class cMtlTex;
class cGroup;

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

	///--------------------------------------------------
	LPD3DXMESH				m_pMeshTeapot;
	LPD3DXMESH				m_pMeshSphere;
	D3DMATERIAL9			m_stMtlTeapot;
	D3DMATERIAL9			m_stMtlSphere;
	
	LPD3DXMESH				m_pObjMesh;
	vector<cMtlTex*>		m_vecObjMtlTex;
	vector<cGroup*>			m_vecGroup;
public :
	void Setup();
	void Update(); 
	void Render(); 

	void WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	void Set_Light();

	///--------------------------------------------------
	//					폰트
	///--------------------------------------------------

	void Create_Font();
	void Text_Render();

	///--------------------------------------------------
	//				 주전자
	///--------------------------------------------------
	void Setup_MeshObject();
	void Mesh_Render();

};

