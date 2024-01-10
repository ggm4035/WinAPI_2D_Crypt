#pragma once
#include "Define.h"

class CScrollMgr
{
private:
	static CScrollMgr* m_pInstance;
	float m_fScrollX;
	float m_fScrollY;

private:
	CScrollMgr();
	~CScrollMgr();

public:
	static CScrollMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CScrollMgr;

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

public:
	const float& Get_ScrollX() const { return m_fScrollX; }
	const float& Get_ScrollY() const { return m_fScrollY; }
	void Set_ScrollX(const float& _fX) { m_fScrollX = _fX; }
	void Set_ScrollY(const float& _fY) { m_fScrollY = _fY; }
	void Add_ScrollX(const float& _fX) { m_fScrollX += _fX; }
	void Add_ScrollY(const float& _fY) { m_fScrollX += _fY; }
	void Quake() { m_fScrollX += 10; }
};

#define SCROLL_MGR	CScrollMgr::Get_Instance()