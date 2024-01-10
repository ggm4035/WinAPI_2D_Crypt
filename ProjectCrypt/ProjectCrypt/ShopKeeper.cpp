#include "stdafx.h"
#include "ShopKeeper.h"
#include "TileMgr.h"
#include "BitmapMgr.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"
#include "SoundMgr.h"
#include "Key.h"
#include "ItemTorch.h"

CShopKeeper::CShopKeeper()
	:m_fVolume(0.f), m_fDistance(0.f)
{
}

CShopKeeper::~CShopKeeper()
{
	Release();
}

bool CShopKeeper::Collision()
{
	return false;
}

void CShopKeeper::Check_Player_Distance()
{
	float fX = fabs(m_tInfo.fX - m_pTarget->Get_Info().fX);
	float fY = fabs(m_tInfo.fY - m_pTarget->Get_Info().fY);
	m_fDistance = sqrtf(fX * fX + fY * fY);
}

void CShopKeeper::Initialize()
{
	m_tInfo.fCX = 94.f;
	m_tInfo.fCY = 76.f;

	m_fPower = 9.f;
	m_fDistance = 1000.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 7;
	m_tFrame.iMotion = 0;
	m_tFrame.dwSpeed = 500;

	m_tNextPos = m_tInfo;

	m_eID = OBJ_NPC;

	m_tState.iAttack = 100;
	m_tState.iHP = 8;
	m_tState.iMaxHP = 8;

	m_pTarget = OBJ_MGR->Get_Player();
	SOUND_MGR->PlaySound(L"zone1_1_shopkeeper.ogg", SOUND_BGM, 0.f);
}

int CShopKeeper::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_Rect();

	Check_Player_Distance();

	return OBJ_NOEVENT;
}

void CShopKeeper::Late_Update()
{
	if (m_fDistance <= 480)
	{
		m_fVolume = 0.2f - m_fDistance / 48.f * 0.01f;

		if (m_fVolume > 0.2f)
			m_fVolume = 0.2f;

		SOUND_MGR->SetChannelVolume(SOUND_BGM, m_fVolume);
	}
	else
	{
		m_fVolume = 0.f;
		SOUND_MGR->SetChannelVolume(SOUND_BGM, m_fVolume);
	}

	__super::Move_Frame(m_tFrame);
}

void CShopKeeper::Render(HDC _hDC)
{
	float fScrollX = SCROLL_MGR->Get_ScrollX();
	float fScrollY = SCROLL_MGR->Get_ScrollY();

	HDC hMemDC = BITMAP_MGR->Find_Img(L"shopkeeper");

	GdiTransparentBlt(_hDC, int(m_tRect.left + fScrollX), int(m_tRect.top + fScrollY - 20),
		int(m_tInfo.fCX), int(m_tInfo.fCY), hMemDC,
		int((m_tInfo.fCX) * m_tFrame.iFrameStart), int((m_tInfo.fCY) * m_tFrame.iMotion),
		int(m_tInfo.fCX), int(m_tInfo.fCY), RGB(255, 0, 144));
}

void CShopKeeper::Release()
{
}