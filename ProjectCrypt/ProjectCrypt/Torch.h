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

// �� �ʳ��� �þ��۾��� �� ��쿡 �÷��̾��� �þ߸� �÷��ִ� ������ ������