#pragma once
#include "Obj.h"
class CMoney :
	public CObj
{
private:
	virtual bool Collision() override;

public:
	CMoney(const int& _iMoney);
	virtual ~CMoney();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hDC) override;
	virtual void Release() override;

public:
	const int& Get_Money() { return m_tState.iMoney; }
};

