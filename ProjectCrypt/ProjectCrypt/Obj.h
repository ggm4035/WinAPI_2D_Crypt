#pragma once
#include "Define.h"
#include "TimeMgr.h"

class CObj
{
protected:
	enum DIRECTION { LEFT, RIGHT, UP, DOWN, DIR_END };

protected:
	INFO m_tInfo;
	INFO m_tNextPos;
	RECT m_tRect;
	FRAME m_tFrame;
	FRAME m_tEffect;
	STATE m_tState;

	CObj* m_pTarget;
	TCHAR m_szImgKey[32];

	bool m_bDead;
	bool m_bJump;
	bool m_bMoveTime;
	bool m_bWrongBeat;

	DIRECTION m_eDir;
	OBJID m_eID;

	float m_fTime;
	float m_fPower;

protected:
	void Update_Rect();
	void Move_Frame(FRAME& _rFrame);
	virtual bool Collision() PURE;
	void Jumping();

public:
	CObj();
	virtual ~CObj();

public:
	virtual void Initialize() PURE;
	virtual int Update() PURE;
	virtual void Late_Update() PURE;
	virtual void Render(HDC _hDC) PURE;
	virtual void Release() PURE;

public:
	const INFO& Get_Info() const { return m_tInfo; }
	const INFO& Get_Next_Info() const { return m_tNextPos; }
	const RECT& Get_Rect() const { return m_tRect; }
	const OBJID& Get_ObjID() const { return m_eID; }
	const TCHAR* Get_ImgKey() const { return m_szImgKey; }
	const STATE& Get_State() const { return m_tState; }

	void Set_Info_Pos(const float& _fX, const float& _fY);
	void Set_Info_Size(const float& _fCX, const float& _fCY)
	{
		m_tInfo.fCX = _fCX;
		m_tInfo.fCY = _fCY;
	}
	void Set_ImgKey(TCHAR* _pKey) { lstrcpy(m_szImgKey, _pKey); }
	void Set_MoveTime(bool _bool) { m_bMoveTime = _bool; }
	void Set_Damage(const int& _iAttack) 
	{
		int iAtk = _iAttack - m_tState.iDefence;
		if (iAtk < 1) iAtk = 1;

		m_tState.iHP -= iAtk;
	}
	void Set_State(const STATE& _tState) { m_tState = _tState; }
	void Add_MaxHP(const int& _iMaxHP) { m_tState.iMaxHP += _iMaxHP; }
	void Set_Money(const int& _iMoney) { m_tState.iMoney = _iMoney; }
	void Set_Dia(const int& _iDia) { m_tState.iDiamond = _iDia; }
	void Set_DrawX(const int& _iX) { m_tFrame.iFrameStart = _iX; }
	void Set_HP_Full() { m_tState.iHP = m_tState.iMaxHP; }
	void Add_HP(const int& _iHeal) 
	{ 
		m_tState.iHP += _iHeal;
		if (m_tState.iHP > m_tState.iMaxHP)
			m_tState.iHP = m_tState.iMaxHP;
	}
	void Set_Wrong_Beat(bool _bool) { m_bWrongBeat = _bool; }
	void Reset_Direction() { m_eDir = UP; }
	void Set_Dead() { m_bDead = true; }
	bool Get_Dead() { return m_bDead; }

public:
	virtual void Play_Hit_Sound() {};
	virtual void Play_Attack_Sound() {};
};

