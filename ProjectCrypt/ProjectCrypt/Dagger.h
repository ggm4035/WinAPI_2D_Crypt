#pragma once
#include "Weapon.h"
class CDagger :
	public CWeapon
{
private:
	virtual void Render_Text(HDC _hDC) override;

public:
	CDagger();
	virtual ~CDagger();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hDC) override;
	virtual void Render_Effect(HDC _hDC) override;
	virtual void Release() override;

public:
	virtual void Attack(const DIRECTION& _eDir) override;
	virtual bool IsRange(const DIRECTION & _eDir) override;
};
