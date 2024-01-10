#pragma once
#include "Monster.h"
class CSkeleton :
	public CMonster
{
private:
	int iAtkMotion;

private:
	virtual bool Collision() override;
	virtual void Play_Hit_Sound();
	virtual void Play_Attack_Sound();
	void Check_Move();
	void Find_Player();

public:
	CSkeleton();
	virtual ~CSkeleton();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hDC) override;
	virtual void Release() override;
};

