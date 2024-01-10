#pragma once
#include "Obj.h"
class CTrap :
	public CObj
{
private:
	virtual bool Collision() override;

public:
	CTrap();
	virtual ~CTrap();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hDC) override;
	virtual void Release() override;

public:
	void Move_Object(CObj* _pObj);
};

