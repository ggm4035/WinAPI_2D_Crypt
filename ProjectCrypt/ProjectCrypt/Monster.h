#pragma once
#include "Obj.h"

class CMonster :
	public CObj
{
protected:
	MONSTERID m_eMonID;
	FRAME m_tEffect;
	bool m_bJumpTime;
	bool m_bAttack;
	bool m_bStop;

protected:
	void Render_Attack(HDC _hDC);

public:
	CMonster();
	virtual ~CMonster();

public:
	virtual void Initialize() PURE;
	virtual int Update() PURE;
	virtual void Late_Update() PURE;
	virtual void Render(HDC _hDC) PURE;
	virtual void Release() PURE;

public:
	const MONSTERID& Get_MonsterID() const { return m_eMonID; }
	void Set_MonsterID(const MONSTERID& _eID) { m_eMonID = _eID; }
	void Set_Stop_Monster(bool _bool) { m_bStop = _bool; }
};

