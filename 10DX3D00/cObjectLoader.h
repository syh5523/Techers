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

	void Load(OUT vector<cGroup*> & vecGroup, IN char* szFolder, IN char* szFile);
	void LoadMtlLib(char* szFolder, char* szFile);


};

