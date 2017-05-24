#pragma once

#define g_pTextureManager cTextureManager::GetInstance()

class cTextureManager
{
//public:
//	cTextureManager();
//	~cTextureManager();

private:
	SINGLETONE(cTextureManager);

	map<string, LPDIRECT3DTEXTURE9> m_mapTexture;
	map<string, D3DXIMAGE_INFO>	m_mapImageInfo;
public:
	LPDIRECT3DTEXTURE9	GetTexture(char* szFullPath);
	LPDIRECT3DTEXTURE9	GetTexture(string& sFullPath);
	void Destroy();

	LPDIRECT3DTEXTURE9	GetTexture(char* szFullPath, D3DXIMAGE_INFO* pImageinfo);
	LPDIRECT3DTEXTURE9	GetTexture(string& sFullPath, D3DXIMAGE_INFO* pImageinfo);

};

