#include "stdafx.h"
#include "BroadSword.h"
#include "TileMgr.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "BitmapMgr.h"
#include "Monster.h"
#include "Boss.h"

void CBroadSword::Render_Text(HDC _hDC)
{
	if (m_fDistance > sqrtf(TILECX * TILECX + TILECY * TILECY))
		return;

	float fScrollX = SCROLL_MGR->Get_ScrollX();
	float fScrollY = SCROLL_MGR->Get_ScrollY();

	HFONT hFont = CreateFont(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("necrosans")); // 폰트변경
	HFONT oldFont = (HFONT)SelectObject(_hDC, hFont); // DC관련
	SetTextAlign(_hDC, TA_CENTER); // 텍스트 정렬
	SetTextColor(_hDC, RGB(255, 255, 255)); // 텍스트 컬러 설정
	SetBkMode(_hDC, TRANSPARENT); // 매개변수 DC의 글자배경을 투명하게 한다.


	TextOut(_hDC, int(m_tInfo.fX + fScrollX), int(m_tInfo.fY + fScrollY - 60.f), L"broadsword", 11);

	(HFONT)DeleteObject(oldFont); // 원래 있던폰트 제거
}

CBroadSword::CBroadSword()
{
}

CBroadSword::~CBroadSword()
{
	Release();
}

void CBroadSword::Initialize()
{
	m_eID = OBJ_ITEM;
	m_eItemID = ITEM_WEAPON;
	m_tInfo.fCX = 48.f;
	m_tInfo.fCY = 48.f;

	m_tEffect.iFrameStart = 0;
	m_tEffect.iFrameEnd = 2;
	m_tEffect.dwSpeed = 100;
	m_tEffect.iMotion = 0;
	m_tEffect.iMotionEnd = 2;

	m_tState.iAttack = 1;
}

int CBroadSword::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CBroadSword::Late_Update()
{
	if (m_bEffect)
		__super::Move_Frame(m_tEffect, m_eOption);

	__super::Set_Distance();
}

void CBroadSword::Render(HDC _hDC)
{
	float fScrollX = SCROLL_MGR->Get_ScrollX();
	float fScrollY = SCROLL_MGR->Get_ScrollY();

	HDC hMemDC = BITMAP_MGR->Find_Img(m_szImgKey);

	GdiTransparentBlt(_hDC, int(m_tRect.left + fScrollX), int(m_tRect.top + fScrollY - 20),
		int(m_tInfo.fCX), int(m_tInfo.fCY), hMemDC,
		0, int((m_tInfo.fCY) * m_tFrame.iMotion),
		int(m_tInfo.fCX), int(m_tInfo.fCY), RGB(255, 0, 144));

	__super::Show_Money(_hDC);
	Render_Text(_hDC);
}

void CBroadSword::Release()
{
}

void CBroadSword::Render_Effect(HDC _hDC)
{
	if (!m_bEffect)
		return;

	float fScrollX = SCROLL_MGR->Get_ScrollX();
	float fScrollY = SCROLL_MGR->Get_ScrollY();

	HDC hEffectDC;

	switch (m_eDir)
	{
	case LEFT:
		if (m_tEffect.iFrameStart == 0)
			m_bEffect = false;

		hEffectDC = BITMAP_MGR->Find_Img(L"swipe_broadsword_titanium_Left");
		break;

	case RIGHT:
		if (m_tEffect.iFrameStart == 2)
			m_bEffect = false;

		hEffectDC = BITMAP_MGR->Find_Img(L"swipe_broadsword_titanium");
		break;

	case UP:
		if (m_tEffect.iMotion == 0)
			m_bEffect = false;

		hEffectDC = BITMAP_MGR->Find_Img(L"swipe_broadsword_titanium_Up");
		break;

	case DOWN:
		if (m_tEffect.iMotion == 2)
			m_bEffect = false;

		hEffectDC = BITMAP_MGR->Find_Img(L"swipe_broadsword_titanium_Down");
		break;

	default:
		return;
	}

	GdiTransparentBlt(_hDC,
		int((m_tTarget.fX - m_tTarget.fCX * 0.5f) + fScrollX),
		int((m_tTarget.fY - m_tTarget.fCY * 0.5f) + fScrollY - 20),
		int(m_tTarget.fCX), int(m_tTarget.fCY), hEffectDC,
		int(m_tTarget.fCX) * m_tEffect.iFrameStart, int(m_tTarget.fCY) * m_tEffect.iMotion,
		int(m_tTarget.fCX), int(m_tTarget.fCY), RGB(255, 0, 144));
}

void CBroadSword::Attack(const DIRECTION & _eDir)
{
	CObj* pObj = OBJ_MGR->Get_Player();
	m_eDir = _eDir;

	switch (_eDir)
	{
	case LEFT:
		m_tTarget.fX = pObj->Get_Info().fX - TILECX;
		m_tTarget.fY = pObj->Get_Info().fY;
		m_tTarget.fCX = TILECX;
		m_tTarget.fCY = TILECY * 3;

		m_tEffect.iFrameStart = 2;
		m_tEffect.iMotion = 0;
		m_eOption = RTOL;

		if (pObj = dynamic_cast<CMonster*>(OBJ_MGR->Find_Obj(OBJ_MONSTER, m_tTarget.fX, m_tTarget.fY)))
		{
			pObj->Play_Hit_Sound();
			pObj->Set_Damage(m_tState.iAttack);

			if (static_cast<CMonster*>(pObj)->Get_MonsterID() == BOSS)
				static_cast<CBoss*>(pObj)->Move_Around();
		}
		if (pObj = dynamic_cast<CMonster*>(OBJ_MGR->Find_Obj(OBJ_MONSTER, m_tTarget.fX, m_tTarget.fY - TILECY)))
		{
			pObj->Play_Hit_Sound();
			pObj->Set_Damage(m_tState.iAttack);

			if (static_cast<CMonster*>(pObj)->Get_MonsterID() == BOSS)
				static_cast<CBoss*>(pObj)->Move_Around();
		}

		if (pObj = dynamic_cast<CMonster*>(OBJ_MGR->Find_Obj(OBJ_MONSTER, m_tTarget.fX, m_tTarget.fY + TILECY)))
		{
			pObj->Play_Hit_Sound();
			pObj->Set_Damage(m_tState.iAttack);

			if (static_cast<CMonster*>(pObj)->Get_MonsterID() == BOSS)
				static_cast<CBoss*>(pObj)->Move_Around();
		}
		break;

	case RIGHT:
		m_tTarget.fX = pObj->Get_Info().fX + TILECX;
		m_tTarget.fY = pObj->Get_Info().fY;
		m_tTarget.fCX = TILECX;
		m_tTarget.fCY = TILECY * 3;

		m_tEffect.iFrameStart = 0;
		m_tEffect.iMotion = 0;
		m_eOption = LTOR;

		if (pObj = dynamic_cast<CMonster*>(OBJ_MGR->Find_Obj(OBJ_MONSTER, m_tTarget.fX, m_tTarget.fY)))
		{
			pObj->Play_Hit_Sound();
			pObj->Set_Damage(m_tState.iAttack);

			if (static_cast<CMonster*>(pObj)->Get_MonsterID() == BOSS)
				static_cast<CBoss*>(pObj)->Move_Around();
		}

		if (pObj = dynamic_cast<CMonster*>(OBJ_MGR->Find_Obj(OBJ_MONSTER, m_tTarget.fX, m_tTarget.fY - TILECY)))
		{
			pObj->Play_Hit_Sound();
			pObj->Set_Damage(m_tState.iAttack);

			if (static_cast<CMonster*>(pObj)->Get_MonsterID() == BOSS)
				static_cast<CBoss*>(pObj)->Move_Around();
		}

		if (pObj = dynamic_cast<CMonster*>(OBJ_MGR->Find_Obj(OBJ_MONSTER, m_tTarget.fX, m_tTarget.fY + TILECY)))
		{
			pObj->Play_Hit_Sound();
			pObj->Set_Damage(m_tState.iAttack);

			if (static_cast<CMonster*>(pObj)->Get_MonsterID() == BOSS)
				static_cast<CBoss*>(pObj)->Move_Around();
		}
		break;

	case UP:
		m_tTarget.fX = pObj->Get_Info().fX;
		m_tTarget.fY = pObj->Get_Info().fY - TILECY;
		m_tTarget.fCX = TILECX * 3;
		m_tTarget.fCY = TILECY;

		m_tEffect.iMotion = 2;
		m_tEffect.iFrameStart = 0;
		m_eOption = BTOT;

		if (pObj = dynamic_cast<CMonster*>(OBJ_MGR->Find_Obj(OBJ_MONSTER, m_tTarget.fX, m_tTarget.fY)))
		{
			pObj->Play_Hit_Sound();
			pObj->Set_Damage(m_tState.iAttack);

			if (static_cast<CMonster*>(pObj)->Get_MonsterID() == BOSS)
				static_cast<CBoss*>(pObj)->Move_Around();
		}

		if (pObj = dynamic_cast<CMonster*>(OBJ_MGR->Find_Obj(OBJ_MONSTER, m_tTarget.fX + TILECX, m_tTarget.fY)))
		{
			pObj->Play_Hit_Sound();
			pObj->Set_Damage(m_tState.iAttack);

			if (static_cast<CMonster*>(pObj)->Get_MonsterID() == BOSS)
				static_cast<CBoss*>(pObj)->Move_Around();
		}

		if (pObj = dynamic_cast<CMonster*>(OBJ_MGR->Find_Obj(OBJ_MONSTER, m_tTarget.fX - TILECX, m_tTarget.fY)))
		{
			pObj->Play_Hit_Sound();
			pObj->Set_Damage(m_tState.iAttack);

			if (static_cast<CMonster*>(pObj)->Get_MonsterID() == BOSS)
				static_cast<CBoss*>(pObj)->Move_Around();
		}
		break;

	case DOWN:
		m_tTarget.fX = pObj->Get_Info().fX;
		m_tTarget.fY = pObj->Get_Info().fY + TILECY;
		m_tTarget.fCX = TILECX * 3;
		m_tTarget.fCY = TILECY;

		m_tEffect.iMotion = 0;
		m_tEffect.iFrameStart = 0;
		m_eOption = TTOB;

		if (pObj = dynamic_cast<CMonster*>(OBJ_MGR->Find_Obj(OBJ_MONSTER, m_tTarget.fX, m_tTarget.fY)))
		{
			pObj->Play_Hit_Sound();
			pObj->Set_Damage(m_tState.iAttack);

			if (static_cast<CMonster*>(pObj)->Get_MonsterID() == BOSS)
				static_cast<CBoss*>(pObj)->Move_Around();
		}

		if (pObj = dynamic_cast<CMonster*>(OBJ_MGR->Find_Obj(OBJ_MONSTER, m_tTarget.fX + TILECX, m_tTarget.fY)))
		{
			pObj->Play_Hit_Sound();
			pObj->Set_Damage(m_tState.iAttack);

			if (static_cast<CMonster*>(pObj)->Get_MonsterID() == BOSS)
				static_cast<CBoss*>(pObj)->Move_Around();
		}

		if (pObj = dynamic_cast<CMonster*>(OBJ_MGR->Find_Obj(OBJ_MONSTER, m_tTarget.fX - TILECX, m_tTarget.fY)))
		{
			pObj->Play_Hit_Sound();
			pObj->Set_Damage(m_tState.iAttack);

			if (static_cast<CMonster*>(pObj)->Get_MonsterID() == BOSS)
				static_cast<CBoss*>(pObj)->Move_Around();
		}
		break;

	default:
		break;
	}
}

bool CBroadSword::IsRange(const DIRECTION & _eDir)
{
	CObj* pObj = OBJ_MGR->Get_Player();

	switch (_eDir)
	{
	case LEFT:
		if (OBJ_MGR->Find_Obj(OBJ_MONSTER, pObj->Get_Info().fX - TILECX, pObj->Get_Info().fY) ||
			OBJ_MGR->Find_Obj(OBJ_MONSTER, pObj->Get_Info().fX - TILECX, pObj->Get_Info().fY + TILECY) ||
			OBJ_MGR->Find_Obj(OBJ_MONSTER, pObj->Get_Info().fX - TILECX, pObj->Get_Info().fY - TILECY))
			return true;
		break;

	case RIGHT:
		if (OBJ_MGR->Find_Obj(OBJ_MONSTER, pObj->Get_Info().fX + TILECX, pObj->Get_Info().fY) ||
			OBJ_MGR->Find_Obj(OBJ_MONSTER, pObj->Get_Info().fX + TILECX, pObj->Get_Info().fY + TILECY) ||
			OBJ_MGR->Find_Obj(OBJ_MONSTER, pObj->Get_Info().fX + TILECX, pObj->Get_Info().fY - TILECY))
			return true;
		break;

	case UP:
		if (OBJ_MGR->Find_Obj(OBJ_MONSTER, pObj->Get_Info().fX, pObj->Get_Info().fY - TILECY) ||
			OBJ_MGR->Find_Obj(OBJ_MONSTER, pObj->Get_Info().fX + TILECX, pObj->Get_Info().fY - TILECY) ||
			OBJ_MGR->Find_Obj(OBJ_MONSTER, pObj->Get_Info().fX - TILECX, pObj->Get_Info().fY - TILECY))
			return true;
		break;

	case DOWN:
		if (OBJ_MGR->Find_Obj(OBJ_MONSTER, pObj->Get_Info().fX, pObj->Get_Info().fY + TILECY) ||
			OBJ_MGR->Find_Obj(OBJ_MONSTER, pObj->Get_Info().fX + TILECX, pObj->Get_Info().fY + TILECY) ||
			OBJ_MGR->Find_Obj(OBJ_MONSTER, pObj->Get_Info().fX - TILECX, pObj->Get_Info().fY + TILECY))
			return true;
		break;

	default:
		break;
	}
	return false;
}
