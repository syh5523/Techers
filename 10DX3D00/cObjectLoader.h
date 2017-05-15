#pragma once

class cMtlTex;
class cGroup;

class cObjectLoader
{
public:
	cObjectLoader();
	~cObjectLoader();

private:
	std::map<string, cMtlTex*>	m_mapMtlTex;


public:

	void Load(OUT vector<cGroup*>& vecGroup, IN char * szFolder, IN char * szFile);
	void LoadMtlLib(char* szFolder, char* szFile);

	void LoadSurface(OUT std::vector<D3DXVECTOR3>& vecSurface, IN char * szFolder,
		IN char * szFile, IN D3DXMATRIXA16 * pMat);

	LPD3DXMESH LoadMesh(OUT vector<cMtlTex*>& vecMtlTex, IN char * szFolder, IN char * szFile);
};

