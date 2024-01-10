#include "stdafx.h"
#include "Monster.h"
#include "ScrollMgr.h"
#include "BitmapMgr.h"

void CMonster::Render_Attack(HDC _hDC)
{
	float fScrollX = SCROLL_MGR->Get_ScrollX();
	float fScrollY = SCROLL_MGR->Get_ScrollY();

	HDC hEffectDC = BITMAP_MGR->Find_Img(L"swipe_enemy_Left");
	INFO tEffectPos{};

	switch (m_eDir)
	{
	case CObj::LEFT:
		tEffectPos.fX = m_tInfo.fX - TILECX;
		tEffectPos.fY = m_tInfo.fY;
		break;

	case CObj::RIGHT:
		tEffectPos.fX = m_tInfo.fX + TILECX;
		tEffectPos.fY = m_tInfo.fY;
		break;

	case CObj::UP:
		tEffectPos.fX = m_tInfo.fX;
		tEffectPos.fY = m_tInfo.fY - TILECY;
		break;

	case CObj::DOWN:
		tEffectPos.fX = m_tInfo.fX;
		tEffectPos.fY = m_tInfo.fY + TILECY;
		break;
	}

	tEffectPos.fCX = 54.f;
	tEffectPos.fCY = 48.f;

	GdiTransparentBlt(_hDC, int((tEffectPos.fX - tEffectPos.fCX * 0.5f) + fScrollX),
		int((tEffectPos.fY - tEffectPos.fCY * 0.5f) + fScrollY - 20),
		54, 48, hEffectDC,
		54 * m_tEffect.iFrameStart, 0,
		54, 48, RGB(255, 0, 144));

	if (m_tEffect.iFrameStart == 4)
	{
		m_tEffect.iFrameStart = 0;
		m_bAttack = false;
	}
}

CMonster::CMonster()
	:m_eMonID(MONSTER_END), m_bJumpTime(false), m_bAttack(false), m_bStop(false)
{
	ZeroMemory(&m_tEffect, sizeof(FRAME));
}

CMonster::~CMonster()
{
}