#pragma once
#include "Monster.h"
class CDragon :
	public CMonster
{
private:
	virtual bool Collision() override;
	virtual void Play_Hit_Sound();
	virtual void Play_Attack_Sound();
	void Render_HP(HDC _hDC, const float& _fScrollX, const float& _fScrollY);
	void Check_Move();
	void Find_Player();

public:
	CDragon();
	virtual ~CDragon();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hDC) override;
	virtual void Release() override;
};

