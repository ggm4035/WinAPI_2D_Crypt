#include "stdafx.h"
#include "Dagger.h"
#include "TileMgr.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "BitmapMgr.h"
#include "Monster.h"
#include "Boss.h"

void CDagger::Render_Text(HDC _hDC)
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


	TextOut(_hDC, int(m_tInfo.fX + fScrollX), int(m_tInfo.fY + fScrollY - 60.f), L"dagger", 7);

	(HFONT)DeleteObject(oldFont); // 원래 있던폰트 제거
}

CDagger::CDagger()
{
}

CDagger::~CDagger()
{
}

void CDagger::Initialize()
{
	m_eID = OBJ_ITEM;
	m_eItemID = ITEM_WEAPON;
	m_tInfo.fCX = 48.f;
	m_tInfo.fCY = 48.f;
	
	m_tEffect.iFrameStart = 0;
	m_tEffect.iFrameEnd = 2;
	m_tEffect.dwSpeed = 150;
	m_tEffect.iMotion = 0;

	m_tState.iAttack = 1;
}

int CDagger::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CDagger::Late_Update()
{
	if(m_bEffect)
		__super::Move_Frame(m_tEffect, LTOR);

	__super::Set_Distance();
}

void CDagger::Render(HDC _hDC)
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

void CDagger::Release()
{
}

void CDagger::Attack(const DIRECTION & _eDir)
{
	CObj* pObj = OBJ_MGR->Get_Player();
	m_eDir = _eDir;

	switch (_eDir)
	{
	case LEFT:
		m_tTarget.fX = pObj->Get_Info().fX - TILECX;
		m_tTarget.fY = pObj->Get_Info().fY;
		break;

	case RIGHT:
		m_tTarget.fX = pObj->Get_Info().fX + TILECX;
		m_tTarget.fY = pObj->Get_Info().fY;
		break;

	case UP:
		m_tTarget.fX = pObj->Get_Info().fX;
		m_tTarget.fY = pObj->Get_Info().fY - TILECY;
		break;

	case DOWN:
		m_tTarget.fX = pObj->Get_Info().fX;
		m_tTarget.fY = pObj->Get_Info().fY + TILECY;
		break;

	default:
		break;
	}

	OBJ_MGR->Find_Obj(OBJ_MONSTER, m_tTarget.fX, m_tTarget.fY)->Set_Damage(m_tState.iAttack);
	OBJ_MGR->Find_Obj(OBJ_MONSTER, m_tTarget.fX, m_tTarget.fY)->Play_Hit_Sound();
	if (static_cast<CMonster*>(OBJ_MGR->Find_Obj(OBJ_MONSTER, m_tTarget.fX, m_tTarget.fY))->Get_MonsterID() == BOSS)
		static_cast<CBoss*>(OBJ_MGR->Find_Obj(OBJ_MONSTER, m_tTarget.fX, m_tTarget.fY))->Move_Around();
}

bool CDagger::IsRange(const DIRECTION & _eDir)
{
	CObj* pObj = OBJ_MGR->Get_Player();

	switch (_eDir)
	{
	case LEFT:
		if (OBJ_MGR->Find_Obj(OBJ_MONSTER, pObj->Get_Info().fX - TILECX, pObj->Get_Info().fY))
			return true;
		break;

	case RIGHT:
		if (OBJ_MGR->Find_Obj(OBJ_MONSTER, pObj->Get_Info().fX + TILECX, pObj->Get_Info().fY))
			return true;
		break;

	case UP:
		if (OBJ_MGR->Find_Obj(OBJ_MONSTER, pObj->Get_Info().fX, pObj->Get_Info().fY - TILECY))
			return true;
		break;

	case DOWN:
		if (OBJ_MGR->Find_Obj(OBJ_MONSTER, pObj->Get_Info().fX, pObj->Get_Info().fY + TILECY))
			return true;
		break;

	default:
		break;
	}
	return false;
}

void CDagger::Render_Effect(HDC _hDC)
{
	if (!m_bEffect)
		return;

	float fScrollX = SCROLL_MGR->Get_ScrollX();
	float fScrollY = SCROLL_MGR->Get_ScrollY();

	HDC hEffectDC;

	switch (m_eDir)
	{
	case LEFT:
		hEffectDC = BITMAP_MGR->Find_Img(L"swipe_dagger_Left");
		break;

	case RIGHT:
		hEffectDC = BITMAP_MGR->Find_Img(L"swipe_dagger");
		break;

	case UP:
		hEffectDC = BITMAP_MGR->Find_Img(L"swipe_dagger_Up");
		break;

	case DOWN:
		hEffectDC = BITMAP_MGR->Find_Img(L"swipe_dagger_Down");
		break;

	default:
		return;
	}

	GdiTransparentBlt(_hDC, 
		int((m_tTarget.fX - TILECX * 0.5f) + fScrollX),
		int((m_tTarget.fY - TILECY * 0.5f) + fScrollY - 20),
		TILECX, TILECY, hEffectDC,
		TILECX * m_tEffect.iFrameStart, TILECY * m_tEffect.iMotion,
		TILECX, TILECY, RGB(255, 0, 144));

	if (m_tEffect.iFrameStart == 2)
	{
		m_tEffect.iFrameStart = 0;
		m_bEffect = false;
	}
}