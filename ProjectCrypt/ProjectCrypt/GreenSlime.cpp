#include "stdafx.h"
#include "GreenSlime.h"
#include "BitmapMgr.h"
#include "ScrollMgr.h"
#include "TileMgr.h"
#include "SoundMgr.h"
#include "ObjMgr.h"
#include "Money.h"

CGreenSlime::CGreenSlime()
{
}

CGreenSlime::~CGreenSlime()
{
	Release();
}

void CGreenSlime::Initialize()
{
	m_tInfo.fCX = 52.f;
	m_tInfo.fCY = 52.f;

	m_fPower = 9.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iMotion = 0;
	m_tFrame.dwSpeed = 125;

	m_tNextPos = m_tInfo;

	m_eID = OBJ_MONSTER;
	m_eMonID =GREEN_SLIME;

	m_tState.iHP = 1;
	m_tState.iMaxHP = 1;
}

int CGreenSlime::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CGreenSlime::Late_Update()
{
	if (m_tState.iHP <= 0)
	{
		TILE_MGR->Pop_Object(m_tInfo.fX, m_tInfo.fY);
		CObj* pMoney = new CMoney(5);
		pMoney->Set_Info_Pos(m_tInfo.fX, m_tInfo.fY);
		pMoney->Initialize();
		TILE_MGR->Push_Object(m_tInfo.fX, m_tInfo.fY, pMoney);
		OBJ_MGR->AddObject(OBJ_MONEY, pMoney);
		m_bDead = true;
		return;
	}

	if (m_bMoveTime)
	{
		m_bJump = true;
		m_bMoveTime = false;
	}

	if(m_bJump)
		__super::Jumping();

	if (!m_bStop)
		__super::Move_Frame(m_tFrame);
}

void CGreenSlime::Render(HDC _hDC)
{
	float fScrollX = SCROLL_MGR->Get_ScrollX();
	float fScrollY = SCROLL_MGR->Get_ScrollY();

	HDC hMemDC = BITMAP_MGR->Find_Img(L"slime_green");
	HDC hHPDC = BITMAP_MGR->Find_Img(L"TEMP_heart_small");

	GdiTransparentBlt(_hDC, int(m_tRect.left + fScrollX), int(m_tRect.top + fScrollY - 20),
		int(m_tInfo.fCX), int(m_tInfo.fCY), hMemDC,
		int((m_tInfo.fCX) * m_tFrame.iFrameStart), int((m_tInfo.fCY) * m_tFrame.iMotion),
		int(m_tInfo.fCX), int(m_tInfo.fCY), RGB(255, 0, 144));

	GdiTransparentBlt(_hDC, int(m_tInfo.fX - 12.f + fScrollX), int(m_tInfo.fY - 70.f + fScrollY),
		24, 24, hHPDC,
		0, 0,
		24, 24, RGB(255, 0, 144));
}

void CGreenSlime::Release()
{
}

bool CGreenSlime::Collision()
{
	return false;
}

void CGreenSlime::Play_Hit_Sound()
{
	SOUND_MGR->PlaySound(L"en_slime_hit_01.ogg", SOUND_MONSTER_VOICE, 0.5f);
}

void CGreenSlime::Play_Attack_Sound()
{
	SOUND_MGR->PlaySound(L"en_slime_attack.ogg", SOUND_MONSTER_VOICE, 0.5f);
}
