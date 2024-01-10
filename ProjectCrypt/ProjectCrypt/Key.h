#pragma once
#include "Item.h"
class CKey :
	public CItem
{
private:
	virtual bool Collision() override;
	virtual void Render_Text(HDC _hDC) override;

public:
	CKey();
	virtual ~CKey();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hDC) override;
	virtual void Release() override;
	virtual void Render_UI(HDC _hDC) override;
};

