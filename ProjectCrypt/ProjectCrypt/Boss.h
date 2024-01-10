#pragma once
#include "Monster.h"
class CBoss :
	public CMonster
{
private:
	FRAME m_tAngry;
	DIRECTION m_eSecondDir;
	bool m_bCry;
	bool m_bFinish;

private:
	virtual bool Collision() override;
	virtual void Play_Hit_Sound();
	virtual void Play_Attack_Sound();
	void Render_HP(HDC _hDC, const float& _fScrollX, const float& _fScrollY);
	void Check_Move();
	void Find_Player();
	void Jumping();
	void Render_Attack(HDC _hDC);

public:
	CBoss();
	virtual ~CBoss();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hDC) override;
	virtual void Release() override;

public:
	void Move_Around();
	void Set_Finish() { m_bFinish = true; }
};

