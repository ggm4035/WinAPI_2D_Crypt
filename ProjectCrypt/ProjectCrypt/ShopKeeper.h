#pragma once
#include "Obj.h"

class CShopKeeper :
	public CObj
{
private:
	float m_fVolume;
	float m_fDistance;

private:
	virtual bool Collision() override;
	void Check_Player_Distance();

public:
	CShopKeeper();
	virtual ~CShopKeeper();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hDC) override;
	virtual void Release() override;
};

