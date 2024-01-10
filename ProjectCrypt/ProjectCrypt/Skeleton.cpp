#include "stdafx.h"
#include "Skeleton.h"
#include "ScrollMgr.h"
#include "BitmapMgr.h"
#include "TileMgr.h"
#include "ObjMgr.h"
#include "SoundMgr.h"
#include "Player.h"
#include "Money.h"

CSkeleton::CSkeleton()
	:iAtkMotion(0)
{
}

CSkeleton::~CSkeleton()
{
	Release();
}

bool CSkeleton::Collision()
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
		m_pTarget->Set_Damage(m_tState.iAttack);
		static_cast<CPlayer*>(m_pTarget)->Set_Hit();
		return true;

	case OBJ_NPC:
	case OBJ_WALL:
	case OBJ_TORCH:
	case OBJ_CHEST:
	case OBJ_ITEM:
	case OBJ_MONSTER:
		return true;
	}
	return false;
}

void CSkeleton::Play_Hit_Sound()
{
	SOUND_MGR->PlaySound(L"en_skel_hit.ogg", SOUND_MONSTER_VOICE, 0.5f);
}

void CSkeleton::Play_Attack_Sound()
{
	SOUND_MGR->PlaySound(L"en_skel_attack_melee.ogg", SOUND_MONSTER_VOICE, 0.5f);
}

void CSkeleton::Check_Move()
{
	if (m_bJumpTime == false)
	{
		iAtkMotion = 4;
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

	iAtkMotion = 0;
	m_bJumpTime = false;
	m_bMoveTime = false;
}

void CSkeleton::Find_Player()
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

void CSkeleton::Initialize()
{
	lstrcpy(m_szImgKey, L"skeleton");
	m_tInfo.fCX = 48.f;
	m_tInfo.fCY = 50.f;

	m_fPower = 9.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iMotion = 0;
	m_tFrame.dwSpeed = 300;

	m_tEffect.iFrameStart = 0;
	m_tEffect.iFrameEnd = 4;
	m_tEffect.iMotion = 0;
	m_tEffect.dwSpeed = 50;

	m_tNextPos = m_tInfo;

	m_eID = OBJ_MONSTER;
	m_eMonID = SKELETON;

	m_tState.iAttack = 1;
	m_tState.iHP = 1;
	m_tState.iMaxHP = 1;
}

int CSkeleton::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CSkeleton::Late_Update()
{
	if (m_tState.iHP <= 0)
	{
		TILE_MGR->Pop_Object(m_tInfo.fX, m_tInfo.fY);
		CObj* pMoney = new CMoney(10);
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
		__super::Jumping();

	if (m_bAttack)
		__super::Move_Frame(m_tEffect);

	if (!m_bStop)
		__super::Move_Frame(m_tFrame);
}

void CSkeleton::Render(HDC _hDC)
{
	float fScrollX = SCROLL_MGR->Get_ScrollX();
	float fScrollY = SCROLL_MGR->Get_ScrollY();

	HDC hMemDC = BITMAP_MGR->Find_Img(m_szImgKey);
	HDC hHPDC = BITMAP_MGR->Find_Img(L"TEMP_heart_small");

	GdiTransparentBlt(_hDC, int(m_tRect.left + fScrollX), int(m_tRect.top + fScrollY - 20),
		int(m_tInfo.fCX), int(m_tInfo.fCY), hMemDC,
		int((m_tInfo.fCX) * (m_tFrame.iFrameStart + iAtkMotion)), 
		int((m_tInfo.fCY) * (m_tFrame.iMotion)),
		int(m_tInfo.fCX), int(m_tInfo.fCY), RGB(255, 0, 144));

	GdiTransparentBlt(_hDC, int(m_tInfo.fX - 12.f + fScrollX), int(m_tInfo.fY - 70.f + fScrollY),
		24, 24, hHPDC,
		0, 0,
		24, 24, RGB(255, 0, 144));

	if(m_bAttack)
		Render_Attack(_hDC);
}

void CSkeleton::Release()
{
}
