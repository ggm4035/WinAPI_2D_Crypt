#pragma once
#include "Obj.h"
class CWall :
	public CObj
{
private:
	WALLID m_eWallID;
	DWORD m_dwTime;
	const TCHAR* m_pShovel;
	bool m_bShovel;

private:
	virtual bool Collision() override;
	void Render_Shovel(HDC _hDC);

public:
	CWall();
	virtual ~CWall();
	
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hDC) override;
	virtual void Release() override;

public:
	void Set_WallID(const WALLID& _eID) { m_eWallID = _eID; }
	const WALLID& Get_WallID() { return m_eWallID; }
	void Set_Shovel(const TCHAR* _pShovel) 
	{ 
		m_bShovel = true; 
		m_dwTime = GetTickCount();
		m_pShovel = _pShovel;
	}
};