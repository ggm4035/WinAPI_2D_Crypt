#pragma once
#include "Obj.h"
class CChest :
	public CObj
{
private:
	virtual bool Collision() override;
	void Make_Item();

public:
	CChest();
	virtual ~CChest();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hDC) override;
	virtual void Release() override;
};

