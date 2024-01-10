#include "stdafx.h"
#include "Zombie.h"
#include "Player.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "BitmapMgr.h"
#include "TileMgr.h"
#include "Money.h"

CZombie::CZombie()
	:m_iMoveMotion(0), m_bIsFront(false), m_pFront(nullptr)
{
	ZeroMemory(&m_tPrePos, sizeof(INFO));
}

CZombie::~CZombie()
{
	Release();
}

bool CZombie::Collision()
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

void CZombie::Check_Move()
{
	if (m_bJumpTime == false)
	{
		m_bJumpTime = true;
		m_bMoveTime = false;
		return;
	}

	m_tPrePos = m_tInfo;

	if (m_eDir == DOWN)
	{
		m_iMoveMotion = 0;
		if(m_bIsFront)
			m_tNextPos.fY += TILECY;
		else
			m_tNextPos = static_cast<CZombie*>(m_pFront)->Get_Pre_Info();
	}

	if (m_eDir == UP)
	{
		m_iMoveMotion = 16;
		if (m_bIsFront)
			m_tNextPos.fY -= TILECY;
		else
			m_tNextPos = static_cast<CZombie*>(m_pFront)->Get_Pre_Info();
	}

	if (m_eDir == LEFT)
	{
		m_iMoveMotion = 8;
		lstrcpy(m_szImgKey, L"zombie");
		if (m_bIsFront)
			m_tNextPos.fX -= TILECX;
		else
			m_tNextPos = static_cast<CZombie*>(m_pFront)->Get_Pre_Info();
	}

	if (m_eDir == RIGHT)
	{
		m_iMoveMotion = 8;
		lstrcpy(m_szImgKey, L"zombie_Flip");
		if (m_bIsFront)
			m_tNextPos.fX += TILECX;
		else
			m_tNextPos = static_cast<CZombie*>(m_pFront)->Get_Pre_Info();
	}

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

void CZombie::Find_Player()
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

void CZombie::Follow_Zombie()
{
	float fDisX = static_cast<CZombie*>(m_pFront)->Get_Pre_Info().fX - m_tInfo.fX;
	float fDisY = static_cast<CZombie*>(m_pFront)->Get_Pre_Info().fY - m_tInfo.fY;

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

void CZombie::Initialize()
{
	lstrcpy(m_szImgKey, L"zombie");

	m_tInfo.fCX = 48.f;
	m_tInfo.fCY = 50.f;

	m_fPower = 9.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 7;
	m_tFrame.iMotion = 0;
	m_tFrame.dwSpeed = 150;

	m_tEffect.iFrameStart = 0;
	m_tEffect.iFrameEnd = 4;
	m_tEffect.iMotion = 0;
	m_tEffect.dwSpeed = 50;

	m_tPrePos = m_tNextPos = m_tInfo;

	m_eID = OBJ_MONSTER;
	m_eMonID = ZOMBIE;

	m_tState.iAttack = 1;
	m_tState.iHP = 1;
	m_tState.iMaxHP = 1;

	Chain_Zombie();
}

int CZombie::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CZombie::Late_Update()
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

	if (!m_bIsFront && m_pFront->Get_Dead())
		m_bIsFront = true;

	if (m_bMoveTime)
	{
		if (m_bIsFront)
			Find_Player();
		else
			Follow_Zombie();

		Check_Move();
	}

	if (m_bJump)
		__super::Jumping();

	if (m_bAttack)
		__super::Move_Frame(m_tEffect);

	if (!m_bStop)
		__super::Move_Frame(m_tFrame);
}

void CZombie::Render(HDC _hDC)
{
	float fScrollX = SCROLL_MGR->Get_ScrollX();
	float fScrollY = SCROLL_MGR->Get_ScrollY();

	HDC hMemDC = BITMAP_MGR->Find_Img(m_szImgKey);
	HDC hHPDC = BITMAP_MGR->Find_Img(L"TEMP_heart_small");

	GdiTransparentBlt(_hDC, int(m_tRect.left + fScrollX), int(m_tRect.top + fScrollY - 20),
		int(m_tInfo.fCX), int(m_tInfo.fCY), hMemDC,
		int((m_tInfo.fCX) * (m_tFrame.iFrameStart + m_iMoveMotion)),
		int((m_tInfo.fCY) * (m_tFrame.iMotion)),
		int(m_tInfo.fCX), int(m_tInfo.fCY), RGB(255, 0, 144));

	GdiTransparentBlt(_hDC, int(m_tInfo.fX - 12.f + fScrollX), int(m_tInfo.fY - 70.f + fScrollY),
		24, 24, hHPDC,
		0, 0,
		24, 24, RGB(255, 0, 144));

	if (m_bAttack)
		Render_Attack(_hDC);
}

void CZombie::Release()
{
}

void CZombie::Chain_Zombie()
{
	if (OBJ_MGR->Find_Obj(m_tInfo.fX, m_tInfo.fY + TILECY) && 
		OBJ_MGR->Find_Obj(m_tInfo.fX, m_tInfo.fY + TILECY)->Get_ObjID() == OBJ_MONSTER && 
		static_cast<CMonster*>(OBJ_MGR->Find_Obj(m_tInfo.fX, m_tInfo.fY + TILECY))->Get_MonsterID() == ZOMBIE)
		m_pFront = OBJ_MGR->Find_Obj(m_tInfo.fX, m_tInfo.fY + TILECY);

	else if (OBJ_MGR->Find_Obj(m_tInfo.fX + TILECX, m_tInfo.fY) && OBJ_MGR->Find_Obj(m_tInfo.fX + TILECX, m_tInfo.fY)->Get_ObjID() == OBJ_MONSTER &&
		!m_pFront && static_cast<CMonster*>(OBJ_MGR->Find_Obj(m_tInfo.fX + TILECX, m_tInfo.fY))->Get_MonsterID() == ZOMBIE)
		m_pFront = OBJ_MGR->Find_Obj(m_tInfo.fX + TILECX, m_tInfo.fY);

	else if (OBJ_MGR->Find_Obj(m_tInfo.fX - TILECX, m_tInfo.fY) && OBJ_MGR->Find_Obj(m_tInfo.fX - TILECX, m_tInfo.fY)->Get_ObjID() == OBJ_MONSTER &&
		!m_pFront && static_cast<CMonster*>(OBJ_MGR->Find_Obj(m_tInfo.fX - TILECX, m_tInfo.fY))->Get_MonsterID() == ZOMBIE)
		m_pFront = OBJ_MGR->Find_Obj(m_tInfo.fX - TILECX, m_tInfo.fY);

	else
		m_bIsFront = true;
}
