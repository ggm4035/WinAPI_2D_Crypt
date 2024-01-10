#include "stdafx.h"
#include "Boss.h"
#include "ScrollMgr.h"
#include "BitmapMgr.h"
#include "TileMgr.h"
#include "SoundMgr.h"
#include "ObjMgr.h"
#include "Player.h"

CBoss::CBoss()
	:m_eSecondDir(DIR_END), m_bCry(false), m_bFinish(false)
{
	ZeroMemory(&m_tAngry, sizeof(FRAME));
}

CBoss::~CBoss()
{
	Release();
}

bool CBoss::Collision()
{
	float fX = m_tInfo.fX - OBJ_MGR->Get_Player()->Get_Info().fX;
	float fY = m_tInfo.fY - OBJ_MGR->Get_Player()->Get_Info().fY;

	float fDistance = sqrtf(fX * fX + fY * fY);

	if (fDistance <= sqrtf(TILECX * TILECX + TILECY * TILECY))
	{
		m_bAttack = true;
		Play_Attack_Sound();
		OBJ_MGR->Get_Player()->Set_Damage(m_tState.iAttack);
		static_cast<CPlayer*>(OBJ_MGR->Get_Player())->Set_Hit();
		return true;
	}

	int iX = int(m_tNextPos.fX / TILECX);
	int iY = int(m_tNextPos.fY / TILECY);

	CObj* pObj = TILE_MGR->Peek_Object(m_tNextPos.fX, m_tNextPos.fY);

	if (!pObj)
		return false;

	switch (pObj->Get_ObjID())
	{
	case OBJ_NPC:
	case OBJ_CHEST:
	case OBJ_ITEM:
	case OBJ_MONSTER:
	case OBJ_WALL:
		return true;
	}
	return false;
}

void CBoss::Play_Hit_Sound()
{
	SOUND_MGR->PlaySound(L"en_kingconga_hit_01.ogg", SOUND_MONSTER_VOICE, 0.3f);
}

void CBoss::Play_Attack_Sound()
{
	SOUND_MGR->PlaySound(L"en_kingconga_attack_02.ogg", SOUND_MONSTER_VOICE, 0.3f);
}

void CBoss::Render_HP(HDC _hDC, const float & _fScrollX, const float & _fScrollY)
{
	int i = 0;
	int iEmptyHeart = m_tState.iMaxHP - m_tState.iHP;

	HDC hMemDC = BITMAP_MGR->Find_Img(L"TEMP_heart_empty_small");
	while (i < iEmptyHeart)
	{
		GdiTransparentBlt(_hDC,
			int(m_tInfo.fX + 36.f - i * 24.f + _fScrollX),
			int(m_tInfo.fY - 90.f + _fScrollY),
			24, 24, hMemDC,
			0, 0,
			24, 24, RGB(255, 0, 144));
		++i;
	}

	// 가득 찬 체력칸 출력
	hMemDC = BITMAP_MGR->Find_Img(L"TEMP_heart_small");
	while (i < m_tState.iMaxHP)
	{
		GdiTransparentBlt(_hDC,
			int(m_tInfo.fX + 36.f - i * 24.f + _fScrollX),
			int(m_tInfo.fY - 90.f + _fScrollY),
			24, 24, hMemDC,
			0, 0,
			24, 24, RGB(255, 0, 144));
		++i;
	}
}

void CBoss::Check_Move()
{
	if (m_eDir == DOWN)
	{
		m_tNextPos.fY += TILECY;
		if (m_eSecondDir == LEFT)
			m_tNextPos.fX -= TILECX;
		else if (m_eSecondDir == RIGHT)
			m_tNextPos.fX += TILECX;
	}

	if (m_eDir == UP)
	{
		m_tNextPos.fY -= TILECY;
		if (m_eSecondDir == LEFT)
			m_tNextPos.fX -= TILECX;
		else if (m_eSecondDir == RIGHT)
			m_tNextPos.fX += TILECX;
	}

	if (m_eDir == LEFT)
	{
		m_tNextPos.fX -= TILECX;
		if (m_eSecondDir == UP)
			m_tNextPos.fY -= TILECY;
		else if (m_eSecondDir == DOWN)
			m_tNextPos.fY += TILECY;
	}

	if (m_eDir == RIGHT)
	{
		m_tNextPos.fX += TILECX;
		if (m_eSecondDir == UP)
			m_tNextPos.fY -= TILECY;
		else if (m_eSecondDir == DOWN)
			m_tNextPos.fY += TILECY;
	}

	if (!Collision())
	{
		TILE_MGR->Pop_Object(m_tInfo.fX, m_tInfo.fY);
		TILE_MGR->Push_Object(m_tNextPos.fX, m_tNextPos.fY, this);
		m_bJump = true;
	}
	else
		m_tNextPos = m_tInfo;

	m_bMoveTime = false;
}

void CBoss::Find_Player()
{
	m_pTarget = OBJ_MGR->Get_Player();

	float fDisX = m_pTarget->Get_Info().fX - m_tInfo.fX;
	float fDisY = m_pTarget->Get_Info().fY - m_tInfo.fY;

	if (fDisX > 0)
	{
		if (fDisY > 0)
		{
			m_eDir = DOWN;
			m_eSecondDir = RIGHT;
		}
		else if (fDisY < 0)
		{
			m_eDir = UP;
			m_eSecondDir = RIGHT;
		}
		else
		{
			m_eDir = RIGHT;
			m_eSecondDir = RIGHT;
		}
	}
	else if (fDisX < 0)
	{
		if (fDisY > 0)
		{
			m_eDir = DOWN;
			m_eSecondDir = LEFT;
		}
		else if (fDisY < 0)
		{
			m_eDir = UP;
			m_eSecondDir = LEFT;
		}
		else
		{
			m_eDir = LEFT;
			m_eSecondDir = LEFT;
		}
	}
	else	// 0
	{
		if (fDisY > 0)
		{
			m_eDir = DOWN;
			m_eSecondDir = DOWN;
		}
		else
		{
			m_eDir = UP;
			m_eSecondDir = UP;
		}
	}
}

void CBoss::Jumping()
{
	float fParabolaF = m_fPower * m_fTime - 9.8f * 0.5f * m_fTime * m_fTime;
	m_tInfo.fY -= fParabolaF;
	m_fTime += 0.3f;

	switch (m_eDir)
	{
	case CObj::LEFT:
		if (m_eSecondDir == LEFT)
		{
			m_tInfo.fX -= 10.f;

			if (m_tInfo.fX < m_tNextPos.fX)
			{
				m_tInfo.fX = m_tNextPos.fX;
				m_tInfo.fY = m_tNextPos.fY;
				m_fTime = 0.f;
				m_bJump = false;
			}
		}
		else if (m_eSecondDir == UP)
		{
			m_tInfo.fX -= 10.f;
			m_tInfo.fY -= 10.f;

			if (m_tInfo.fX < m_tNextPos.fX && m_tInfo.fY < m_tNextPos.fY)
			{
				m_tInfo.fX = m_tNextPos.fX;
				m_tInfo.fY = m_tNextPos.fY;
				m_fTime = 0.f;
				m_bJump = false;
			}
		}
		else if (m_eSecondDir == DOWN)
		{
			m_tInfo.fX -= 10.f;
			m_tInfo.fY += 10.f;

			if (m_tInfo.fX < m_tNextPos.fX && m_tInfo.fY > m_tNextPos.fY)
			{
				m_tInfo.fX = m_tNextPos.fX;
				m_tInfo.fY = m_tNextPos.fY;
				m_fTime = 0.f;
				m_bJump = false;
			}
		}
		break;

	case CObj::RIGHT:
		if (m_eSecondDir == RIGHT)
		{
			m_tInfo.fX += 10.f;

			if (m_tInfo.fX > m_tNextPos.fX)
			{
				m_tInfo.fX = m_tNextPos.fX;
				m_tInfo.fY = m_tNextPos.fY;
				m_fTime = 0.f;
				m_bJump = false;
			}
		}
		else if (m_eSecondDir == UP)
		{
			m_tInfo.fX += 10.f;
			m_tInfo.fY -= 10.f;

			if (m_tInfo.fX > m_tNextPos.fX && m_tInfo.fY < m_tNextPos.fY)
			{
				m_tInfo.fX = m_tNextPos.fX;
				m_tInfo.fY = m_tNextPos.fY;
				m_fTime = 0.f;
				m_bJump = false;
			}
		}
		else if (m_eSecondDir == DOWN)
		{
			m_tInfo.fX += 10.f;
			m_tInfo.fY += 10.f;

			if (m_tInfo.fX > m_tNextPos.fX && m_tInfo.fY > m_tNextPos.fY)
			{
				m_tInfo.fX = m_tNextPos.fX;
				m_tInfo.fY = m_tNextPos.fY;
				m_fTime = 0.f;
				m_bJump = false;
			}
		}
		break;

	case CObj::UP:
		if (m_eSecondDir == UP)
		{
			m_tInfo.fY -= 10.f;

			if (m_tInfo.fY < m_tNextPos.fY)
			{
				m_tInfo.fX = m_tNextPos.fX;
				m_tInfo.fY = m_tNextPos.fY;
				m_fTime = 0.f;
				m_bJump = false;
			}
		}
		else if (m_eSecondDir == LEFT)
		{
			m_tInfo.fX -= 10.f;
			m_tInfo.fY -= 10.f;

			if (m_tInfo.fX < m_tNextPos.fX && m_tInfo.fY < m_tNextPos.fY)
			{
				m_tInfo.fX = m_tNextPos.fX;
				m_tInfo.fY = m_tNextPos.fY;
				m_fTime = 0.f;
				m_bJump = false;
			}
		}
		else if (m_eSecondDir == RIGHT)
		{
			m_tInfo.fX += 10.f;
			m_tInfo.fY -= 10.f;

			if (m_tInfo.fX > m_tNextPos.fX && m_tInfo.fY < m_tNextPos.fY)
			{
				m_tInfo.fX = m_tNextPos.fX;
				m_tInfo.fY = m_tNextPos.fY;
				m_fTime = 0.f;
				m_bJump = false;
			}
		}
		break;

	case CObj::DOWN:
		if (m_eSecondDir == DOWN)
		{
			m_tInfo.fY += 10.f;

			if (m_tInfo.fY > m_tNextPos.fY)
			{
				m_tInfo.fX = m_tNextPos.fX;
				m_tInfo.fY = m_tNextPos.fY;
				m_fTime = 0.f;
				m_bJump = false;
			}
		}
		else if (m_eSecondDir == LEFT)
		{
			m_tInfo.fX -= 10.f;
			m_tInfo.fY += 10.f;

			if (m_tInfo.fX < m_tNextPos.fX && m_tInfo.fY > m_tNextPos.fY)
			{
				m_tInfo.fX = m_tNextPos.fX;
				m_tInfo.fY = m_tNextPos.fY;
				m_fTime = 0.f;
				m_bJump = false;
			}
		}
		else if (m_eSecondDir == RIGHT)
		{
			m_tInfo.fX += 10.f;
			m_tInfo.fY += 10.f;

			if (m_tInfo.fX > m_tNextPos.fX && m_tInfo.fY > m_tNextPos.fY)
			{
				m_tInfo.fX = m_tNextPos.fX;
				m_tInfo.fY = m_tNextPos.fY;
				m_fTime = 0.f;
				m_bJump = false;
			}
		}
		break;
	}
}

void CBoss::Initialize()
{
	lstrcpy(m_szImgKey, L"king_conga");
	m_tInfo.fCX = 116.f;
	m_tInfo.fCY = 158.f;

	m_fPower = 9.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 4;
	m_tFrame.iMotion = 0;
	m_tFrame.dwSpeed = 250;
	m_tFrame.dwTime = GetTickCount();

	m_tEffect.iFrameStart = 0;
	m_tEffect.iFrameEnd = 4;
	m_tEffect.iMotion = 0;
	m_tEffect.dwSpeed = 50;

	m_tAngry.iFrameStart = 0; 
	m_tAngry.iFrameEnd = 1;
	m_tAngry.iMotion = 0;
	m_tAngry.dwSpeed = 500;
	m_tAngry.dwTime = GetTickCount();

	m_tNextPos = m_tInfo;

	m_eID = OBJ_MONSTER;
	m_eMonID = BOSS;

	m_tState.iAttack = 3;
	m_tState.iHP = 5;
	m_tState.iMaxHP = 5;
}

int CBoss::Update()
{
	if (m_bDead)
	{
		SOUND_MGR->PlaySound(L"en_kingconga_death.ogg", SOUND_MONSTER_VOICE, 0.4f);
		return OBJ_DEAD;
	}

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CBoss::Late_Update()
{
	if (m_tState.iHP <= 0)
	{
		TILE_MGR->Pop_Object(m_tInfo.fX, m_tInfo.fY);
		m_bDead = true;
		return;
	}

	if (m_bMoveTime && m_bWrongBeat)
	{
		++m_tAngry.iFrameStart;
		if (m_tAngry.iFrameStart > m_tAngry.iFrameEnd)
			m_tAngry.iFrameStart = 0;

		if (m_bCry)
		{
			SOUND_MGR->PlaySound(L"en_kingconga_cry_01.ogg", SOUND_MONSTER_VOICE, 0.6f);
			m_bCry = false;
		}

		Find_Player();
		Check_Move();
	}

	else if (!m_bWrongBeat)
	{
		if (!m_bStop)
			__super::Move_Frame(m_tFrame);

		m_bCry = true;
	}

	if (m_bAttack)
		__super::Move_Frame(m_tEffect);

	if (m_bJump)
		Jumping();
}

void CBoss::Render(HDC _hDC)
{
	float fScrollX = SCROLL_MGR->Get_ScrollX();
	float fScrollY = SCROLL_MGR->Get_ScrollY();

	HDC hMemDC = BITMAP_MGR->Find_Img(m_szImgKey);
	HDC hChairDC = BITMAP_MGR->Find_Img(L"king_conga_throne");

	if (m_bWrongBeat)
	{
		GdiTransparentBlt(_hDC, int(550 + fScrollX), int(282 + fScrollY - 20),
			int(m_tInfo.fCX), int(m_tInfo.fCY), hChairDC,
			0, 0,
			88, 156, RGB(255, 0, 144));

		GdiTransparentBlt(_hDC, int(m_tRect.left + fScrollX), int(m_tRect.top + fScrollY - 20),
			int(m_tInfo.fCX), int(m_tInfo.fCY), hMemDC,
			int((m_tInfo.fCX) * (m_tAngry.iFrameStart + 5)), int((m_tInfo.fCY) * m_tAngry.iMotion),
			int(m_tInfo.fCX), int(m_tInfo.fCY), RGB(255, 0, 144));
	}
	else
	{
		GdiTransparentBlt(_hDC, int(m_tRect.left + fScrollX), int(m_tRect.top + fScrollY - 20),
			int(m_tInfo.fCX), int(m_tInfo.fCY), hMemDC,
			int((m_tInfo.fCX) * m_tFrame.iFrameStart), int((m_tInfo.fCY) * m_tFrame.iMotion),
			int(m_tInfo.fCX), int(m_tInfo.fCY), RGB(255, 0, 144));
	}

	Render_HP(_hDC, fScrollX, fScrollY);

	if(m_bAttack)
		Render_Attack(_hDC);
}

void CBoss::Release()
{
}

void CBoss::Move_Around()
{
	// 좀비가 있는경우 화내는거 풀고 다시 의자에 앉으렴~
	if (!m_bFinish)
	{
		m_bWrongBeat = false;
		m_tState.iHP = m_tState.iMaxHP;
		TILE_MGR->Pop_Object(m_tInfo.fX, m_tInfo.fY);
		Set_Info_Pos(600.f, 360.f);
		TILE_MGR->Push_Object(m_tInfo.fX, m_tInfo.fY, this);
		return;
	}
	list<pair<INFO, float>> LenList;
	INFO LTPos{ 216.f, 360.f };
	INFO RTPos{ 984.f, 360.f };
	INFO LBPos{ 216.f, 888.f };
	INFO RBPos{ 984.f, 888.f };
	pair<INFO, float> LT{ LTPos, sqrtf((m_tInfo.fX - 216.f) * (m_tInfo.fX - 216.f) + (m_tInfo.fY - 360.f) * (m_tInfo.fY - 360.f)) };
	LenList.push_back(LT);
	pair<INFO, float> RT{ RTPos, sqrtf((984.f - m_tInfo.fX) * (984.f - m_tInfo.fX) + (m_tInfo.fY - 360.f) * (m_tInfo.fY - 360.f)) };
	LenList.push_back(RT);
	pair<INFO, float> LB{ LBPos, sqrtf((m_tInfo.fX - 216.f) * (m_tInfo.fX - 216.f) + (888.f - m_tInfo.fY) * (888.f - m_tInfo.fY)) };
	LenList.push_back(LB);
	pair<INFO, float> RB{ RBPos, sqrtf((984.f - m_tInfo.fX) * (984.f - m_tInfo.fX) + (888.f - m_tInfo.fY) * (888.f - m_tInfo.fY)) };
	LenList.push_back(RB);

	LenList.sort([&](pair<INFO, float>_Dest, pair<INFO, float>_Src)
	{
		return _Dest.second > _Src.second;
	});

	auto& iter = LenList.begin();

	if (iter->first.fX == (++iter)->first.fX)	// 가로 범위
	{
		if (iter->first.fX == 216.f)	// 왼쪽
		{
			TILE_MGR->Pop_Object(m_tInfo.fX, m_tInfo.fY);
			Set_Info_Pos(216.f, float((rand() % 528 + 360) / TILECX * TILECX) - 24.f);
			TILE_MGR->Push_Object(m_tInfo.fX, m_tInfo.fY, this);
		}
		else //(iter->first.fX == 984.f)
		{
			TILE_MGR->Pop_Object(m_tInfo.fX, m_tInfo.fY);
			Set_Info_Pos(984.f, float((rand() % 528 + 360) / TILECX * TILECX) - 24.f);
			TILE_MGR->Push_Object(m_tInfo.fX, m_tInfo.fY, this);
		}
	}
	else //(iter->first.fY == (++iter)->first.fY)	// 세로범위
	{
		if (iter->first.fY == 360.f)
		{
			TILE_MGR->Pop_Object(m_tInfo.fX, m_tInfo.fY);
			Set_Info_Pos(float((rand() % 768 + 216) / TILECY * TILECY) - 24.f, 360.f);
			TILE_MGR->Push_Object(m_tInfo.fX, m_tInfo.fY, this);
		}
		else //(iter->first.fY == 888.f)
		{
			TILE_MGR->Pop_Object(m_tInfo.fX, m_tInfo.fY);
			Set_Info_Pos(float((rand() % 768 + 216) / TILECY * TILECY) - 24.f, 888.f);
			TILE_MGR->Push_Object(m_tInfo.fX, m_tInfo.fY, this);
		}
	}
}

void CBoss::Render_Attack(HDC _hDC)
{
	float fScrollX = SCROLL_MGR->Get_ScrollX();
	float fScrollY = SCROLL_MGR->Get_ScrollY();

	HDC hEffectDC = BITMAP_MGR->Find_Img(L"swipe_enemy_Left");
	INFO tEffectPos{};

	switch (m_eDir)
	{
	case CObj::LEFT:
		tEffectPos.fX = m_tInfo.fX - TILECX;

		if (m_eSecondDir == LEFT)
			tEffectPos.fY = m_tInfo.fY;

		else if (m_eSecondDir == UP)
			tEffectPos.fY = m_tInfo.fY - TILECY;

		else if (m_eSecondDir == DOWN)
			tEffectPos.fY = m_tInfo.fY + TILECY;
		break;

	case CObj::RIGHT:
		tEffectPos.fX = m_tInfo.fX + TILECX;

		if(m_eSecondDir == RIGHT)
			tEffectPos.fY = m_tInfo.fY;

		else if (m_eSecondDir == UP)
			tEffectPos.fY = m_tInfo.fY - TILECY;

		else if (m_eSecondDir == DOWN)
			tEffectPos.fY = m_tInfo.fY + TILECY;
		break;

	case CObj::UP:
		tEffectPos.fY = m_tInfo.fY - TILECY;

		if(m_eSecondDir == UP)
			tEffectPos.fX = m_tInfo.fX;

		else if(m_eSecondDir == LEFT)
			tEffectPos.fX = m_tInfo.fX - TILECX;

		else if (m_eSecondDir == RIGHT)
			tEffectPos.fX = m_tInfo.fX + TILECX;
		break;

	case CObj::DOWN:
		tEffectPos.fY = m_tInfo.fY + TILECY;

		if(m_eSecondDir == DOWN)
			tEffectPos.fX = m_tInfo.fX;

		else if (m_eSecondDir == LEFT)
			tEffectPos.fX = m_tInfo.fX - TILECX;

		else if (m_eSecondDir == RIGHT)
			tEffectPos.fX = m_tInfo.fX + TILECX;
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