#pragma once
#include "Monster.h"
class CZombie :
	public CMonster
{
private:
	CObj* m_pFront;
	INFO m_tPrePos;
	int m_iMoveMotion;
	bool m_bIsFront;

private:
	virtual bool Collision() override;
	void Check_Move();
	void Find_Player();
	void Follow_Zombie();
	const INFO& Get_Pre_Info() { return m_tPrePos; }

public:
	CZombie();
	virtual ~CZombie();
	
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hDC) override;
	virtual void Release() override;

public:
	void Set_Front() { m_bIsFront = true; }
	void Chain_Zombie();
};

