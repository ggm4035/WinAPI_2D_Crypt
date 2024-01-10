#pragma once
#include "Weapon.h"
class CLongSword :
	public CWeapon
{
private:
	virtual void Render_Text(HDC _hDC) override;

public:
	CLongSword();
	virtual ~CLongSword();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hDC) override;
	virtual void Release() override;
	virtual void Render_Effect(HDC _hDC) override;

public:
	virtual void Attack(const DIRECTION& _eDir) override;
	virtual bool IsRange(const DIRECTION & _eDir) override;
};