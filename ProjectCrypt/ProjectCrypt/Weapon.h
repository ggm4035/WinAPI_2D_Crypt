#pragma once
#include "Item.h"
class CWeapon :
	public CItem
{
private:
	virtual bool Collision() override;

protected:
	enum FRAME_OPTION { LTOR, RTOL, TTOB, BTOT, OP_END };

protected:
	FRAME_OPTION m_eOption;
	INFO m_tTarget;
	bool m_bEffect;

protected:
	void Move_Frame(FRAME& _rFrame, FRAME_OPTION _eOption);
	virtual void Render_Text(HDC _hDC) PURE;


public:
	CWeapon();
	virtual ~CWeapon();

public:
	virtual void Initialize() PURE;
	virtual int Update() PURE;
	virtual void Late_Update() PURE;
	virtual void Render(HDC _hDC) PURE;
	virtual void Release() PURE;
	virtual void Render_UI(HDC _hDC) override;
	virtual void Render_Effect(HDC _hDC) PURE;

public:
	virtual void Attack(const DIRECTION& _eDir) PURE;
	virtual bool IsRange(const DIRECTION & _eDir) PURE;
	void Set_Effect() { m_bEffect = true; }
};