#include "stdafx.h"
#include "cFrame.h"

cFrame::cFrame()
	: m_pMtlTex(NULL)
	, m_IsMove(false)
	, m_fFrameSpeed(2)
	, m_pVB(NULL)
	, m_nNumTri(0)
	, m_Frame(0)
{
	D3DXMatrixIdentity(&m_matLocalTM);
	D3DXMatrixIdentity(&m_matWorldTM);
}


cFrame::~cFrame()
{
	SAFE_RELEASE(m_pMtlTex);
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pMesh);
}

void cFrame::Setup(bool IsMeshRender)
{
	//비교 렌더하기 위해 똑같은 클래스 2개 선언 후 시간차 비교함 (메쉬렌더는 트루 아닌건 폴스)
	m_IsMeshRender = true;

	//각각의 오브젝트(프레임 클래스)에 메쉬를 입힘.
	D3DXCreateMeshFVF(m_vecVertex.size() / 3, m_vecVertex.size(), D3DXMESH_MANAGED, ST_PNT_VERTEX::FVF,
		g_pD3DDevice, &m_pMesh);

	//버텍스 락
	ST_PNT_VERTEX * pV = NULL;
	m_pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
	memcpy(pV, &m_vecVertex[0], m_vecVertex.size() * sizeof(ST_PNT_VERTEX));
	m_pMesh->UnlockVertexBuffer();

	//인덱스 락
	WORD* pl = NULL;
	m_pMesh->LockIndexBuffer(0, (LPVOID*)&pl);
	for (int i = 0; i < m_vecVertex.size(); ++i)
	{
		pl[i] = i;
	}
	m_pMesh->UnlockIndexBuffer();

	//어트리뷰트 락 (한개의 오브젝트(프레임 클래스)는 같은 머터리얼을 가지고 있어 서브젝트를 나누지 않음) 
	DWORD* pA = NULL;
	m_pMesh->LockAttributeBuffer(0, &pA);
	for (int i = 0; i < m_vecVertex.size() / 3; ++i)
	{
		pA[i] = 0;
	}
	m_pMesh->UnlockAttributeBuffer();

	//최적화
	vector<DWORD> vecAdj(m_vecVertex.size());
	m_pMesh->GenerateAdjacency(0.0f, &vecAdj[0]);
	m_pMesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_COMPACT | D3DXMESHOPT_VERTEXCACHE,
		&vecAdj[0], 0, 0, 0);

	for each(auto c in m_vecChild)
	{
		c->Setup(m_IsMeshRender);
	}

}
void cFrame::Update(int nKeyFrame, D3DXMATRIXA16 * pMatParent)
{
	D3DXMATRIXA16 matR, matT;

	if (m_IsMove)
	{
		CalcLocalR(nKeyFrame, matR);
		CalcLocalT(nKeyFrame, matT);
		m_matLocalTM = matR * matT;
	}
	else
	{
		D3DXMatrixIdentity(&matR);
		matR = m_matLocalTM;
		matR._41 = 0.0f;
		matR._42 = 0.0f;
		matR._43 = 0.0f;

		D3DXMatrixIdentity(&matT);
		matT._41 = m_matLocalTM._41;
		matT._42 = m_matLocalTM._42;
		matT._43 = m_matLocalTM._43;
	}

	m_matWorldTM = m_matLocalTM;

	if (pMatParent)
	{
		m_matWorldTM *= (*pMatParent);
	}

	for each(auto c in m_vecChild)
	{
		c->SetIsMove(m_IsMove);
		c->Update(nKeyFrame, &m_matWorldTM);
	}


}

void cFrame::Render()
{

	if (m_pMtlTex)
	{
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorldTM);
		g_pD3DDevice->SetTexture(0, m_pMtlTex->GetTexture());
		g_pD3DDevice->SetMaterial(&m_pMtlTex->GetMaterial());
		g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);


		m_pMesh->DrawSubset(0);
	}






	for each(auto c in m_vecChild)
	{
		c->Render();
	}

}


void cFrame::AddChild(cFrame * pChild)
{
	m_vecChild.push_back(pChild);
}

void cFrame::Destroy()
{
	for each (auto c in m_vecChild)
		c->Destroy();
	this->Release();
}

void cFrame::CalcOriginalLocalTM(D3DXMATRIXA16 * pMatParent)
{
	m_matLocalTM = m_matWorldTM;
	if (pMatParent)
	{
		D3DXMATRIXA16 matInvParent;
		D3DXMatrixInverse(&matInvParent, 0, pMatParent);
		m_matLocalTM = m_matWorldTM * matInvParent;
	}

	for each(auto c in m_vecChild)
	{
		c->CalcOriginalLocalTM(&m_matWorldTM);
	}
}

int cFrame::GetKeyFrame()
{
	int first = m_dwFirstFrame * m_dwTicksPerFrame;
	int last = m_dwLastFrame * m_dwTicksPerFrame;

	return GetTickCount() * m_fFrameSpeed % (last - first) + first;
}

void cFrame::CalcLocalT(IN int nKeyFrame, OUT D3DXMATRIXA16 & matT)
{
	D3DXMatrixIdentity(&matT);

	if (m_vecPosTrack.empty())
	{
		matT._41 = m_matLocalTM._41;
		matT._42 = m_matLocalTM._42;
		matT._43 = m_matLocalTM._43;
		return;
	}

	if (nKeyFrame <= m_vecPosTrack.front().n)
	{
		matT._41 = m_vecPosTrack.front().v.x;
		matT._42 = m_vecPosTrack.front().v.y;
		matT._43 = m_vecPosTrack.front().v.z;
		return;
	}

	if (nKeyFrame >= m_vecPosTrack.front().n)
	{
		matT._41 = m_vecPosTrack.back().v.x;
		matT._42 = m_vecPosTrack.back().v.y;
		matT._43 = m_vecPosTrack.back().v.z;
		return;
	}

	int nNextIndex = 0;
	for (size_t i = 0; i < m_vecPosTrack.size(); i++)
	{
		if (nKeyFrame < m_vecPosTrack[i].n)
		{
			nNextIndex = i;
			break;
		}
	}

	int nPrevIndex = nNextIndex - 1;

	float t = (nKeyFrame - m_vecPosTrack[nPrevIndex].n) /
		(float)(m_vecPosTrack[nNextIndex].n - m_vecPosTrack[nPrevIndex].n);

	D3DXVECTOR3 v;
	D3DXVec3Lerp(&v, &m_vecPosTrack[nPrevIndex].v, &m_vecPosTrack[nNextIndex].v, t);

	matT._41 = v.x;
	matT._42 = v.y;
	matT._43 = v.z;
}

void cFrame::CalcLocalR(IN int nKeyFrame, OUT D3DXMATRIXA16 & matR)
{
	D3DXMatrixIdentity(&matR);
	if (m_vecRotTrack.empty())
	{
		matR = m_matLocalTM;
		matR._41 = 0.0f;
		matR._42 = 0.0f;
		matR._43 = 0.0f;
		return;
	}

	if (nKeyFrame <= m_vecRotTrack.front().n)
	{
		D3DXMatrixRotationQuaternion(&matR,
			&m_vecRotTrack.front().q);
		return;
	}
	if (nKeyFrame >= m_vecRotTrack.back().n)
	{
		D3DXMatrixRotationQuaternion(&matR,
			&m_vecRotTrack.back().q);
		return;
	}

	int nNextIndex = 0;
	for (size_t i = 0; i < m_vecRotTrack.size(); ++i)
	{
		if (nKeyFrame < m_vecRotTrack[i].n)
		{
			nNextIndex = i;
			break;
		}
	}

	int nPrevIndex = nNextIndex - 1;

	float t = (nKeyFrame - m_vecRotTrack[nPrevIndex].n) /
		(float)(m_vecRotTrack[nNextIndex].n -
			m_vecRotTrack[nPrevIndex].n);

	D3DXQUATERNION q;
	D3DXQuaternionSlerp(&q,
		&m_vecRotTrack[nPrevIndex].q,
		&m_vecRotTrack[nNextIndex].q,
		t);
	D3DXMatrixRotationQuaternion(&matR, &q);
}

void cFrame::BuildVB(vector<ST_PNT_VERTEX>& vecVertex)
{
	m_nNumTri = vecVertex.size() / 3;
	g_pD3DDevice->CreateVertexBuffer(vecVertex.size() * sizeof(ST_PNT_VERTEX), 0,
		ST_PNT_VERTEX::FVF, D3DPOOL_MANAGED, &m_pVB, NULL);

	ST_PNT_VERTEX* pV = NULL;

	m_pVB->Lock(
		0,						//시작버퍼 위치
		0,						//잠글 바이트 수
		(LPVOID*)&pV,
		0);

	memcpy(pV, &vecVertex[0], vecVertex.size() * sizeof(ST_PNT_VERTEX));

	m_pVB->Unlock();
}
