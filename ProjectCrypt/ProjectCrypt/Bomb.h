#pragma once
#include "Item.h"
class CBomb :
	public CItem
{
private:
	int m_iCount;
	int m_iDrawX;
	bool m_blit;
	bool m_bBoom;

private:
	virtual bool Collision() override;
	void Set_Damages();
	virtual void Render_Text(HDC _hDC) override;

public:
	CBomb();
	virtual ~CBomb();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hDC) override;
	virtual void Release() override;
	virtual void Render_UI(HDC _hDC) override;

public:
	void Add_Bomb(const int& _iCount) { m_iCount += _iCount; }
	const int& Get_Bomb_Count() { return m_iCount; }
	void Use_Bomb() { m_blit = true; }
};

