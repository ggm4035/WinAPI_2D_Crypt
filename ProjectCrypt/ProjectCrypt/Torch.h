#pragma once
#include "Obj.h"
class CTorch :
	public CObj
{
private:
	virtual bool Collision() override;

public:
	CTorch();
	virtual ~CTorch();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hDC) override;
	virtual void Release() override;
};

// 먼 훗날에 시야작업에 들어갈 경우에 플레이어의 시야를 늘려주는 역할을 가질것