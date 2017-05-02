#pragma once

#define g_pObjectManager cObjectManager::GetInstance()

class cObject;

class cObjectManager
{
//public:
//	cObjectManager();
//	~cObjectManager();

private:
	SINGLETONE(cObjectManager);
	set<cObject*> m_setObject;

public:
	void AddObject(cObject* pObject) { m_setObject.insert(pObject); }
	void RemoveObject(cObject* pObject) {m_setObject.erase(pObject);}
	void Destroy() { m_setObject.clear(); }
};

