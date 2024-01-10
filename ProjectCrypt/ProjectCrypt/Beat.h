#pragma once
#include "Define.h"

class CBeat 
{
private:
	INFO m_tInfo;
	RECT m_tRect;
	bool m_bHiting;
	bool m_bDead;
	float m_fSpeed;
	DWORD m_dwTime;

private:
	void Update_Rect();

public:
	CBeat();
	~CBeat();

public:
	void Initialize();
	int Update();
	void Late_Update();
	void Render(HDC _hDC);
	void Release();

public:
	void Set_Dead() { m_bDead = true; }
	void Set_Pos(const float& _fX) { m_tInfo.fX = _fX; }
};

