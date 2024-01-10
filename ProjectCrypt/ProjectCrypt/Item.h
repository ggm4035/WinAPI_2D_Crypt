#pragma once
#include "Obj.h"
#include "Define.h"

class CItem :
	public CObj
{
private:
	virtual bool Collision() PURE;

protected:
	float m_fDistance;
	ITEMID m_eItemID;

protected:
	void Show_Money(HDC _hDC);
	void Show_Dia(HDC _hDC);
	void Set_Distance();
	virtual void Render_Text(HDC _hDC) PURE;

public:
	CItem();
	virtual ~CItem();

public:
	virtual void Initialize() PURE;
	virtual int Update() PURE;
	virtual void Late_Update() PURE;
	virtual void Render(HDC _hDC) PURE;
	virtual void Release() PURE;
	virtual void Render_UI(HDC _hDC) PURE;

public:
	void Set_ItemID(const ITEMID& _eID) { m_eItemID = _eID; }
	const ITEMID& Get_ItemID() { return m_eItemID; }
};

