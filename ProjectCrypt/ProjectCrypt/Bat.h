#pragma once
#include "Monster.h"
class CBat :
	public CMonster
{
private:
	virtual bool Collision() override;
	virtual void Play_Hit_Sound();
	virtual void Play_Attack_Sound();
	void Check_Move();

public:
	CBat();
	virtual ~CBat();

	public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hDC) override;
	virtual void Release() override;

};

