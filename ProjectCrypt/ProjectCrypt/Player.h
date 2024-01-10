#pragma once
#include "Obj.h"
#include "Item.h"

class CPlayer :
	public CObj
{
private:
	array<pair<TCHAR*, CObj*>, 8> m_InvenArr;
	vector<INFO> m_HPvec;

	INFO m_tHpPos;
	list<IMAGINE> m_ImginList;

	TCHAR* m_pHeadImgKey;
	TCHAR* m_pBodyImgKey;
	TCHAR* m_pHeadHitImgKey;
	TCHAR* m_pBodyHitImgKey;
	
	DWORD m_dwTime;
	DWORD m_dwStop;
	DWORD m_dwImagin;

	bool m_bDig;
	bool m_bHit;
	bool m_bUseSpell;

private:
	void Change_Item();
	void Key_Input();
	void Set_Scroll();
	void Render_Dig(HDC _hDC);
	void Render_UI(HDC _hDC);
	void Render_AfterImagin(HDC _hDC);
	virtual bool Collision() override;

public:
	CPlayer();
	virtual ~CPlayer();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hDC) override;
	virtual void Release() override;

public:
	virtual void Play_Hit_Sound();
	virtual void Play_Attack_Sound();
	void Set_Hit() { m_bHit = true; m_dwTime = GetTickCount(); }
};