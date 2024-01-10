#pragma once
#include "Item.h"
class CArmor :
	public CItem
{
private:
	Armor m_eArmor;

private:
	virtual bool Collision() override;
	virtual void Render_Text(HDC _hDC) override;

public:
	CArmor();
	virtual ~CArmor();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hDC) override;
	virtual void Release() override;
	virtual void Render_UI(HDC _hDC) override;

public:
	void Set_Armor(const Armor& _eID) { m_eArmor = _eID; }
	const Armor& Get_Armor() { return m_eArmor; }
	const int& Get_Defence();
};