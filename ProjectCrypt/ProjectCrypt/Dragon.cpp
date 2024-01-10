#include "stdafx.h"
#include "Dragon.h"
#include "TileMgr.h"
#include "BitmapMgr.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "SoundMgr.h"
#include "Player.h"
#include "Money.h"

CDragon::CDragon()
{
}

CDragon::~CDragon()
{
	Release();
}

bool CDragon::Collision()
{
	int iX = int(m_tNextPos.fX / TILECX);
	int iY = int(m_tNextPos.fY / TILECY);

	CObj* pObj = TILE_MGR->Peek_Object(m_tNextPos.fX, m_tNextPos.fY);

	if (!pObj)
		return false;

	switch (pObj->Get_ObjID())
	{
	case OBJ_PLAYER:
		m_bAttack = true;
		Play_Attack_Sound();
		pObj->Set_Damage(m_tState.iAttack);
		static_cast<CPlayer*>(pObj)->Set_Hit();
		return true;

	case OBJ_NPC:
	case OBJ_CHEST:
	case OBJ_ITEM:
	case OBJ_MONSTER:
		return true;

	case OBJ_TORCH:
		OBJ_MGR->Delete_Object(m_tNextPos.fX, m_tNextPos.fY);
		OBJ_MGR->Delete_Object(m_tNextPos.fX, m_tNextPos.fY);
		return false;

	case OBJ_WALL:
		OBJ_MGR->Delete_Object(m_tNextPos.fX, m_tNextPos.fY);
		return false;
	}
	return false;
}

void CDragon::Play_Hit_Sound()
{
	SOUND_MGR->PlaySound(L"en_dragon_hit_01.ogg", SOUND_MONSTER_VOICE, 0.3f);
}

void CDragon::Play_Attack_Sound()
{
	SOUND_MGR->PlaySound(L"en_dragon_attack_melee.ogg", SOUND_MONSTER_VOICE, 0.3f);
}

void CDragon::Render_HP(HDC _hDC, const float& _fScrollX, const float& _fScrollY)
{
	int i = 0;
	int iEmptyHeart = m_tState.iMaxHP - m_tState.iHP;

	// ºó Ã¼·ÂÄ­ Ãâ·Â
	HDC hMemDC = BITMAP_MGR->Find_Img(L"TEMP_heart_empty_small");
	while ( i < iEmptyHeart)
	{
		GdiTransparentBlt(_hDC,
			int(m_tInfo.fX + 24.f - i * 24.f + _fScrollX),
			int(m_tInfo.fY - 90.f + _fScrollY),
			24, 24, hMemDC,
			0, 0,
			24, 24, RGB(255, 0, 144));
		++i;
	}

	// °¡µæ Âù Ã¼·ÂÄ­ Ãâ·Â
	hMemDC = BITMAP_MGR->Find_Img(L"TEMP_heart_small");
	while(i < m_tState.iMaxHP)
	{
		GdiTransparentBlt(_hDC,
			int(m_tInfo.fX + 24.f - i * 24.f + _fScrollX),
			int(m_tInfo.fY - 90.f + _fScrollY),
			24, 24, hMemDC,
			0, 0,
			24, 24, RGB(255, 0, 144));
		++i;
	}
}

void CDragon::Check_Move()
{
	if (m_bJumpTime == false)
	{
		m_bJumpTime = true;
		m_bMoveTime = false;
		return;
	}

	if (m_eDir == DOWN)
		m_tNextPos.fY += TILECY;

	if (m_eDir == UP)
		m_tNextPos.fY -= TILECY;

	if (m_eDir == LEFT)
		m_tNextPos.fX -= TILECX;

	if (m_eDir == RIGHT)
		m_tNextPos.fX += TILECX;

	if (!Collision())
	{
		TILE_MGR->Pop_Object(m_tInfo.fX, m_tInfo.fY);
		TILE_MGR->Push_Object(m_tNextPos.fX, m_tNextPos.fY, this);
		m_bJump = true;
	}
	else
		m_tNextPos = m_tInfo;

	m_bJumpTime = false;
	m_bMoveTime = false;
}

void CDragon::Find_Player()
{
	m_pTarget = OBJ_MGR->Get_Player();

	float fDisX = m_pTarget->Get_Info().fX - m_tInfo.fX;
	float fDisY = m_pTarget->Get_Info().fY - m_tInfo.fY;

	if (fDisX > 0)
	{
		if (fDisY > 0)
		{
			if (fabs(fDisX) < fabs(fDisY))
				m_eDir = DOWN;
			else
				m_eDir = RIGHT;
		}
		else if (fDisY < 0)
		{
			if (fabs(fDisX) < fabs(fDisY))
				m_eDir = UP;
			else
				m_eDir = RIGHT;
		}
		else
			m_eDir = RIGHT;
	}
	else if (fDisX < 0)
	{
		if (fDisY > 0)
		{
			if (fabs(fDisX) < fabs(fDisY))
				m_eDir = DOWN;
			else
				m_eDir = LEFT;
		}
		else if (fDisY < 0)
		{
			if (fabs(fDisX) < fabs(fDisY))
				m_eDir = UP;
			else
				m_eDir = LEFT;
		}
		else
			m_eDir = LEFT;
	}
	else	// 0
	{
		if (fDisY > 0)
			m_eDir = DOWN;
		else
			m_eDir = UP;
	}
}

void CDragon::Initialize()
{
	lstrcpy(m_szImgKey, L"dragon_green");
	m_tInfo.fCX = 108.f;
	m_tInfo.fCY = 102.f;

	m_fPower = 9.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 1;
	m_tFrame.iMotion = 0;
	m_tFrame.dwSpeed = 500;

	m_tEffect.iFrameStart = 0;
	m_tEffect.iFrameEnd = 4;
	m_tEffect.iMotion = 0;
	m_tEffect.dwSpeed = 50;

	m_tNextPos = m_tInfo;

	m_eID = OBJ_MONSTER;
	m_eMonID = GREEN_DRAGON;

	m_tState.iAttack = 3;
	m_tState.iHP = 4;
	m_tState.iMaxHP = 4;
}

int CDragon::Update()
{
	if (m_bDead)
	{
		SOUND_MGR->PlaySound(L"en_dragon_death.ogg", SOUND_MONSTER_VOICE, 0.3f);
		return OBJ_DEAD;
	}

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CDragon::Late_Update()
{
	if (m_tState.iHP <= 0)
	{
		TILE_MGR->Pop_Object(m_tInfo.fX, m_tInfo.fY);
		CObj* pMoney = new CMoney(100);
		pMoney->Set_Info_Pos(m_tInfo.fX, m_tInfo.fY);
		pMoney->Initialize();
		TILE_MGR->Push_Object(m_tInfo.fX, m_tInfo.fY, pMoney);
		OBJ_MGR->AddObject(OBJ_MONEY, pMoney);
		m_bDead = true;
		return;
	}

	if (m_bMoveTime)
	{
		Find_Player();
		Check_Move();
	}

	if (m_bJump)
	{
		__super::Jumping();

		if (!m_bJump)
		{
			SOUND_MGR->PlaySound(L"en_dragon_walk_01.ogg", SOUND_MONSTER, 0.3f);
			OBJ_MGR->QuakeOn();
		}
	}

	if (m_bAttack)
		__super::Move_Frame(m_tEffect);

	if (!m_bStop)
		__super::Move_Frame(m_tFrame);
}

void CDragon::Render(HDC _hDC)
{
	float fScrollX = SCROLL_MGR->Get_ScrollX();
	float fScrollY = SCROLL_MGR->Get_ScrollY();

	HDC hMemDC = BITMAP_MGR->Find_Img(m_szImgKey);

	GdiTransparentBlt(_hDC, int(m_tRect.left + fScrollX), int(m_tRect.top + fScrollY - 20),
		int(m_tInfo.fCX), int(m_tInfo.fCY), hMemDC,
		int((m_tInfo.fCX) * m_tFrame.iFrameStart), int((m_tInfo.fCY) * m_tFrame.iMotion),
		int(m_tInfo.fCX), int(m_tInfo.fCY), RGB(255, 0, 144));

	Render_HP(_hDC, fScrollX, fScrollY);

	if(m_bAttack)
		Render_Attack(_hDC);
}

void CDragon::Release()
{
}
