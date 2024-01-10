#pragma once
#include "Item.h"
class CFood :
	public CItem
{
private:
	virtual bool Collision() override;
	virtual void Render_Text(HDC _hDC) override;

public:
	CFood();
	virtual ~CFood();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hDC) override;
	virtual void Release() override;
	virtual void Render_UI(HDC _hDC) override;

public:
	void Eat_Food();
};

