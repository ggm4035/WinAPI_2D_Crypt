#pragma once
#include "Beat.h"

class CBeatMgr
{
private:
	static CBeatMgr* m_pInstance;
	list<CBeat*> m_BeatList;
	int m_iDrawX;
	float m_fDelay;
	DWORD m_dwTime;

private:
	CBeatMgr();
	~CBeatMgr();

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _hDC);
	void Release();

public:
	static CBeatMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CBeatMgr;

		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

	void Make_Beat();
	void Set_Beat_Dead() { m_BeatList.front()->Set_Dead(); }
	void Set_Delay(const float& _fDelay) { m_fDelay = _fDelay; }
	void Add_Delay(const float& _fDelay) { m_fDelay += _fDelay; }
};

#define BEAT_MGR	CBeatMgr::Get_Instance()