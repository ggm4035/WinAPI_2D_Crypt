#include "stdafx.h"
#include "Player.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "BitmapMgr.h"
#include "TileMgr.h"
#include "BeatMgr.h"
#include "ObjMgr.h"
#include "SoundMgr.h"
#include "Wall.h"
#include "Shovel.h"
#include "Weapon.h"
#include "Armor.h"
#include "Bomb.h"
#include "Money.h"
#include "SceneMgr.h"
#include "Monster.h"
#include "Boss.h"
#include "Dagger.h"
#include "BroadSword.h"
#include "LongSword.h"
#include "Food.h"
#include "Heart.h"

CPlayer::CPlayer()
	:m_bHit(false), m_dwTime(GetTickCount()), m_pBodyImgKey(nullptr), m_pHeadImgKey(nullptr),
	m_pBodyHitImgKey(nullptr), m_pHeadHitImgKey(nullptr), m_bDig(false), m_bUseSpell(false)
{
	ZeroMemory(&m_tHpPos, sizeof(INFO));

	for (size_t i = 0; i < m_InvenArr.size(); ++i)
		m_InvenArr[i].second = nullptr;
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	m_tInfo.fCX = TILECX;
	m_tInfo.fCY = TILECY;

	m_tFrame.dwTime = GetTickCount();
	m_tFrame.dwSpeed = 125;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iMotion = 0;

	m_tNextPos = m_tInfo;

	//리소스 세팅
	m_pBodyImgKey = L"player1_armor_body";
	m_pHeadImgKey = L"player1_heads";
	m_pBodyHitImgKey = L"player1_armor_body_hit";
	m_pHeadHitImgKey = L"player1_heads_hit";

	m_eID = OBJ_PLAYER;
	m_eDir = UP;
	m_fPower = 9.f;
	m_dwImagin = GetTickCount();

	m_tState.iMaxHP = 6;
	m_tState.iHP = 6;
	m_tState.iAttack = 1;
	// 이건 임시로 설정한것 나중에 지우기
	m_tState.iMoney = 500;
	m_tState.iDiamond = 50;

	// HP UI 세팅
	m_tHpPos = { 650.f, 50.f, 48.f, 44.f };
	m_HPvec.push_back(m_tHpPos);
	m_tHpPos.fX -= 50;
	m_HPvec.push_back(m_tHpPos);
	m_tHpPos.fX -= 50;
	m_HPvec.push_back(m_tHpPos);
	m_tHpPos.fX -= 50;

	//인벤 세팅
	CObj* pObj = new CShovel;
	pObj->Set_Info_Pos(85.f, 80.f);
	pObj->Initialize();
	pObj->Set_ImgKey(L"shovel_basic");
	m_InvenArr[ITEM_SHOVEL].second = pObj;

	pObj = new CDagger;
	pObj->Set_Info_Pos(155.f, 80.f);
	pObj->Initialize();
	pObj->Set_ImgKey(L"weapon_dagger");
	m_InvenArr[ITEM_WEAPON].second = pObj;

	pObj = new CBomb;
	pObj->Set_Info_Pos(85.f, 160.f);
	pObj->Initialize();
	pObj->Set_ImgKey(L"bomb");
	m_InvenArr[ITEM_BOMB].second = pObj;

	m_InvenArr[ITEM_SHOVEL].first = L"hud_slot_1";
	m_InvenArr[ITEM_WEAPON].first = L"hud_slot_2";
	m_InvenArr[ITEM_ARMOR].first = L"hud_slot_3";
	m_InvenArr[ITEM_TORCH].first = L"hud_slot_6";
	m_InvenArr[ITEM_KEY].first = L"hud_slot_8";			// 60 * 66
	m_InvenArr[ITEM_BOMB].first = L"hud_slot_bomb";
	m_InvenArr[ITEM_FOOD].first = L"hud_slot_action1";	// 60 * 84
	m_InvenArr[ITEM_SPELL].first = L"hud_slot_spell1";	
}

int CPlayer::Update()
{
	if (m_bDead)
	{
		SOUND_MGR->PlaySound(L"vo_cad_death_02.ogg", SOUND_PLAYER_VOICE, 0.4f);
		m_bDead = false;
		return PLAYER_DEAD;
	}

	Key_Input();

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CPlayer::Late_Update()
{
	if (m_tState.iHP <= 0)
	{
		m_bDead = true;
		return;
	}

	if (m_bJump)
		__super::Jumping();

	if (m_bUseSpell && m_dwStop + 9000 < GetTickCount())
	{
		m_bUseSpell = false;
		SOUND_MGR->RestartSound(SOUND_BOSS_STAGE);
		SOUND_MGR->RestartSound(SOUND_STAGE_ONE);
		SOUND_MGR->RestartSound(SOUND_BGM);
		SCENE_MGR->Set_Restart_EveryOne();
		OBJ_MGR->Set_Stop_Monster(false);
		TIME_MGR->Reset();
	}
	Set_Scroll();

#ifdef _DEBUG

	//cout << "플레이어 좌표 : " << m_tInfo.fX << "\t" << m_tInfo.fY << endl;
	//cout << "Next 좌표 : " << m_tNextPos.fX << "\t" << m_tNextPos.fY << endl;

#endif // _DEBUG

	__super::Move_Frame(m_tFrame);

	if (m_InvenArr[ITEM_WEAPON].second)
		static_cast<CWeapon*>(m_InvenArr[ITEM_WEAPON].second)->Late_Update();

	if (m_bUseSpell && m_dwImagin + 100 < GetTickCount())
	{
		static int iCount = 0;
		IMAGINE tImagine{ m_tInfo, m_tFrame, nullptr, nullptr };

		switch (iCount)
		{
		case 0:
			if (!lstrcmp(m_pHeadImgKey, L"player1_heads"))
			{
				tImagine.pHead = L"player1_heads_img";
				tImagine.pBody = L"player1_armor_body_img";
			}
			else
			{
				tImagine.pHead = L"player1_heads_img_Flip";
				tImagine.pBody = L"player1_armor_body_img_Flip";
			}
			break;

		case 1:
			if (!lstrcmp(m_pHeadImgKey, L"player1_heads"))
			{
				tImagine.pHead = L"player1_heads_img1";
				tImagine.pBody = L"player1_armor_body_img1";
			}
			else
			{
				tImagine.pHead = L"player1_heads_img1_Flip";
				tImagine.pBody = L"player1_armor_body_img1_Flip";
			}
			break;

		case 2:
			if (!lstrcmp(m_pHeadImgKey, L"player1_heads"))
			{
				tImagine.pHead = L"player1_heads_img2";
				tImagine.pBody = L"player1_armor_body_img2";
			}
			else
			{
				tImagine.pHead = L"player1_heads_img2_Flip";
				tImagine.pBody = L"player1_armor_body_img2_Flip";
			}
			break;

		case 3:
			if (!lstrcmp(m_pHeadImgKey, L"player1_heads"))
			{
				tImagine.pHead = L"player1_heads_img3";
				tImagine.pBody = L"player1_armor_body_img3";
			}
			else
			{
				tImagine.pHead = L"player1_heads_img3_Flip";
				tImagine.pBody = L"player1_armor_body_img3_Flip";
			}
			break;

		default:
			break;
		}
		m_ImginList.push_front(tImagine);

		if (m_ImginList.size() > 4)
			m_ImginList.erase(--m_ImginList.end());

		m_dwImagin = GetTickCount();
		++iCount;
		if (iCount > 3)
			iCount = 0;
	}
}

void CPlayer::Render(HDC _hDC)
{
	if (m_tState.iHP <= 0)
		return;

	if (m_bUseSpell)
		Render_AfterImagin(_hDC);

	float fScrollX = SCROLL_MGR->Get_ScrollX();
	float fScrollY = SCROLL_MGR->Get_ScrollY();

	HDC hBodyDC = BITMAP_MGR->Find_Img(m_pBodyImgKey);
	HDC hHeadDC = BITMAP_MGR->Find_Img(m_pHeadImgKey);

	if (!m_bHit)
	{
		GdiTransparentBlt(_hDC, int(m_tRect.left + fScrollX), int(m_tRect.top + fScrollY - 20),
			int(TILECX), int(TILECY), hBodyDC,
			TILECX * m_tFrame.iFrameStart, TILECY * m_tFrame.iMotion,
			TILECX, TILECY, RGB(255, 0, 144));

		GdiTransparentBlt(_hDC, int(m_tRect.left + fScrollX), int(m_tRect.top + fScrollY - 20),
			int(TILECX), int(TILECY), hHeadDC,
			TILECX * m_tFrame.iFrameStart, 0,
			TILECX, TILECY, RGB(255, 0, 144));
	}

	else
	{
		hBodyDC = BITMAP_MGR->Find_Img(m_pBodyHitImgKey);
		hHeadDC = BITMAP_MGR->Find_Img(m_pHeadHitImgKey);

		GdiTransparentBlt(_hDC, int(m_tRect.left + fScrollX), int(m_tRect.top + fScrollY - 20),
			int(TILECX), int(TILECY), hBodyDC,
			TILECX * m_tFrame.iFrameStart, TILECY * m_tFrame.iMotion,
			TILECX, TILECY, RGB(255, 0, 144));

		GdiTransparentBlt(_hDC, int(m_tRect.left + fScrollX), int(m_tRect.top + fScrollY - 20),
			int(TILECX), int(TILECY), hHeadDC,
			TILECX * m_tFrame.iFrameStart, TILECY * m_tFrame.iMotion,
			TILECX, TILECY, RGB(255, 0, 144));

		if (m_dwTime + 100 < GetTickCount())
			m_bHit = false;
	}
	//여기다가 UI 그리기
	Render_UI(_hDC);

	// 이펙트
	if (m_InvenArr[ITEM_WEAPON].second)
		static_cast<CWeapon*>(m_InvenArr[ITEM_WEAPON].second)->Render_Effect(_hDC);

	// 삽 출력
	if (m_bDig)
		Render_Dig(_hDC);
}

void CPlayer::Release()
{
	for_each(m_InvenArr.begin(), m_InvenArr.end(), CDeleteMap());
}

void CPlayer::Play_Hit_Sound()
{
	SOUND_MGR->PlaySound(L"vo_cad_hurt_02.ogg", SOUND_PLAYER_VOICE, 0.6f);
}

void CPlayer::Play_Attack_Sound()
{
	int iNum = rand() % 10;

	switch (iNum)
	{
	case 0:
		SOUND_MGR->PlaySound(L"vo_cad_melee_1_01.ogg", SOUND_PLAYER_VOICE, 0.4f);
		break;
	case 1:
		SOUND_MGR->PlaySound(L"vo_cad_melee_1_02.ogg", SOUND_PLAYER_VOICE, 0.4f);
		break;
	case 2:
		SOUND_MGR->PlaySound(L"vo_cad_melee_1_03.ogg", SOUND_PLAYER_VOICE, 0.4f);
		break;
	case 3:
		SOUND_MGR->PlaySound(L"vo_cad_melee_1_04.ogg", SOUND_PLAYER_VOICE, 0.4f);
		break;
	case 4:
		SOUND_MGR->PlaySound(L"vo_cad_melee_1_05.ogg", SOUND_PLAYER_VOICE, 0.4f);
		break;
	case 5:
		SOUND_MGR->PlaySound(L"vo_cad_melee_1_06.ogg", SOUND_PLAYER_VOICE, 0.4f);
		break;
	case 6:
		SOUND_MGR->PlaySound(L"vo_cad_melee_1_07.ogg", SOUND_PLAYER_VOICE, 0.4f);
		break;
	case 7:
		SOUND_MGR->PlaySound(L"vo_cad_melee_1_08.ogg", SOUND_PLAYER_VOICE, 0.4f);
		break;
	case 8:
		SOUND_MGR->PlaySound(L"vo_cad_melee_1_09.ogg", SOUND_PLAYER_VOICE, 0.4f);
		break;
	case 9:
		SOUND_MGR->PlaySound(L"vo_cad_melee_1_10.ogg", SOUND_PLAYER_VOICE, 0.4f);
		break;
	}
	SOUND_MGR->PlaySound(L"sfx_cauldron_hit.ogg", SOUND_EFFECT, 0.4f);
}

void CPlayer::Change_Item()
{
	CObj* pObj = TILE_MGR->Peek_Object(m_tNextPos.fX, m_tNextPos.fY);

	if (m_InvenArr[static_cast<CItem*>(pObj)->Get_ItemID()].second &&
		!lstrcmp(m_InvenArr[static_cast<CItem*>(pObj)->Get_ItemID()].second->Get_ImgKey(), pObj->Get_ImgKey()))
	{
		m_tState.iDiamond += 10;
		OBJ_MGR->Delete_Object(pObj->Get_Info().fX, pObj->Get_Info().fY);
	}
	else if (m_InvenArr[static_cast<CItem*>(pObj)->Get_ItemID()].second)
	{
		TILE_MGR->Pop_Object(pObj->Get_Info().fX, pObj->Get_Info().fY);
		TILE_MGR->Push_Object(pObj->Get_Info().fX, pObj->Get_Info().fY, m_InvenArr[static_cast<CItem*>(pObj)->Get_ItemID()].second);
		m_InvenArr[static_cast<CItem*>(pObj)->Get_ItemID()].second->Set_Info_Pos(pObj->Get_Info().fX, pObj->Get_Info().fY);
		OBJ_MGR->AddObject(OBJ_ITEM, m_InvenArr[static_cast<CItem*>(pObj)->Get_ItemID()].second);
		OBJ_MGR->Erase_Object(pObj);
		pObj->Set_Info_Pos(float(85.f + static_cast<CItem*>(pObj)->Get_ItemID() * 70.f), 80.f);
		m_InvenArr[static_cast<CItem*>(pObj)->Get_ItemID()].second = pObj;
		m_tState.iMoney -= pObj->Get_State().iMoney;
		m_tState.iDiamond -= pObj->Get_State().iDiamond;
		pObj->Set_Money(0);
		pObj->Set_Dia(0);
	}
	else
	{
		TILE_MGR->Pop_Object(pObj->Get_Info().fX, pObj->Get_Info().fY);
		OBJ_MGR->Erase_Object(pObj);
		pObj->Set_Info_Pos(float(85.f + static_cast<CItem*>(pObj)->Get_ItemID() * 70.f), 80.f);
		m_InvenArr[static_cast<CItem*>(pObj)->Get_ItemID()].second = pObj;
		m_tState.iMoney -= pObj->Get_State().iMoney;
		m_tState.iDiamond -= pObj->Get_State().iDiamond;
		pObj->Set_Money(0);
		pObj->Set_Dia(0);
	}
}

void CPlayer::Key_Input()
{
	if (INPUT_MGR->Key_Down(VK_LEFT))
	{
		if (!m_bMoveTime)	// 타이밍이 아니면 보스가 화나요
		{
			if (OBJ_MGR->Find_Monster(BOSS))
				OBJ_MGR->Find_Monster(BOSS)->Set_Wrong_Beat(true);
			return;
		}

		m_pBodyImgKey = L"player1_armor_body_Flip";
		m_pHeadImgKey = L"player1_heads_Flip";
		m_pBodyHitImgKey = L"player1_armor_body_hit_Flip";
		m_pHeadHitImgKey = L"player1_heads_hit_Flip";

		if (m_tInfo.fX != m_tNextPos.fX || m_tInfo.fY != m_tNextPos.fY)
			return;

		m_tNextPos.fX = m_tInfo.fX - TILECX;
		m_tNextPos.fY = m_tInfo.fY;

		m_eDir = LEFT;
		TILE_MGR->Pop_Object(m_tInfo.fX, m_tInfo.fY);

		if (!Collision())
		{
			m_bJump = true;
			TILE_MGR->Push_Object(m_tNextPos.fX, m_tNextPos.fY, this);
		}
		else
		{
			m_tNextPos = m_tInfo;
			TILE_MGR->Push_Object(m_tNextPos.fX, m_tNextPos.fY, this);
		}

		BEAT_MGR->Set_Beat_Dead();
	}

	else if (INPUT_MGR->Key_Down(VK_RIGHT))
	{
		if (!m_bMoveTime)
		{
			if (OBJ_MGR->Find_Monster(BOSS))
				OBJ_MGR->Find_Monster(BOSS)->Set_Wrong_Beat(true);
			return;
		}

		m_pBodyImgKey = L"player1_armor_body";
		m_pHeadImgKey = L"player1_heads";
		m_pBodyHitImgKey = L"player1_armor_body_hit";
		m_pHeadHitImgKey = L"player1_heads_hit";

		if (m_tInfo.fX != m_tNextPos.fX || m_tInfo.fY != m_tNextPos.fY)
			return;

		m_tNextPos.fX = m_tInfo.fX + TILECX;
		m_tNextPos.fY = m_tInfo.fY;

		m_eDir = RIGHT;
		TILE_MGR->Pop_Object(m_tInfo.fX, m_tInfo.fY);

		if (!Collision())
		{
			m_bJump = true;
			TILE_MGR->Push_Object(m_tNextPos.fX, m_tNextPos.fY, this);
		}
		else
		{
			m_tNextPos = m_tInfo;
			TILE_MGR->Push_Object(m_tNextPos.fX, m_tNextPos.fY, this);
		}

		BEAT_MGR->Set_Beat_Dead();
	}

	else if (INPUT_MGR->Key_Down(VK_UP))
	{
		if (!m_bMoveTime)
		{
			if (OBJ_MGR->Find_Monster(BOSS))
				OBJ_MGR->Find_Monster(BOSS)->Set_Wrong_Beat(true);
			return;
		}

		if (m_tInfo.fX != m_tNextPos.fX || m_tInfo.fY != m_tNextPos.fY)
			return;

		m_tNextPos.fX = m_tInfo.fX;
		m_tNextPos.fY = m_tInfo.fY - TILECY;

		m_eDir = UP;
		TILE_MGR->Pop_Object(m_tInfo.fX, m_tInfo.fY);

		if (!Collision())
		{
			m_bJump = true;
			TILE_MGR->Push_Object(m_tNextPos.fX, m_tNextPos.fY, this);
		}
		else
		{
			m_tNextPos = m_tInfo;
			TILE_MGR->Push_Object(m_tNextPos.fX, m_tNextPos.fY, this);
		}

		BEAT_MGR->Set_Beat_Dead();
	}

	else if (INPUT_MGR->Key_Down(VK_DOWN))
	{
		if (!m_bMoveTime)
		{
			if (OBJ_MGR->Find_Monster(BOSS))
				OBJ_MGR->Find_Monster(BOSS)->Set_Wrong_Beat(true);
			return;
		}

		if (m_tInfo.fX != m_tNextPos.fX || m_tInfo.fY != m_tNextPos.fY)
			return;

		m_tNextPos.fX = m_tInfo.fX;
		m_tNextPos.fY = m_tInfo.fY + TILECY;

		m_eDir = DOWN;
		TILE_MGR->Pop_Object(m_tInfo.fX, m_tInfo.fY);

		if (!Collision())
		{
			m_bJump = true;
			TILE_MGR->Push_Object(m_tNextPos.fX, m_tNextPos.fY, this);
		}
		else
		{
			m_tNextPos = m_tInfo;
			TILE_MGR->Push_Object(m_tNextPos.fX, m_tNextPos.fY, this);
		}

		BEAT_MGR->Set_Beat_Dead();
	}

	else if (INPUT_MGR->Key_Down('Z'))	// 폭탄
	{
		if (!m_bMoveTime)
		{
			if (OBJ_MGR->Find_Monster(BOSS))
				OBJ_MGR->Find_Monster(BOSS)->Set_Wrong_Beat(true);
			return;
		}

		if (m_InvenArr[ITEM_BOMB].second)
		{
			static_cast<CBomb*>(m_InvenArr[ITEM_BOMB].second)->Add_Bomb(-1);
			if (static_cast<CBomb*>(m_InvenArr[ITEM_BOMB].second)->Get_Bomb_Count() == 0)
				SAFE_DELETE(CObj*)(m_InvenArr[ITEM_BOMB].second);

			CObj* pBomb = new CBomb;
			pBomb->Set_Info_Pos(m_tInfo.fX, m_tInfo.fY);
			pBomb->Initialize();
			TILE_MGR->Pop_Object(m_tInfo.fX, m_tInfo.fY);
			TILE_MGR->Push_Object(m_tInfo.fX, m_tInfo.fY, pBomb);
			TILE_MGR->Push_Object(m_tInfo.fX, m_tInfo.fY, this);
			OBJ_MGR->AddObject(OBJ_ITEM, pBomb);
			static_cast<CBomb*>(pBomb)->Use_Bomb();
			SOUND_MGR->PlaySound(L"sfx_bomb_lit.ogg", SOUND_EFFECT, 0.3f);
		}
		BEAT_MGR->Set_Beat_Dead();
	}

	else if (INPUT_MGR->Key_Down('X'))	// 음식
	{
		if (!m_bMoveTime)
		{
			if (OBJ_MGR->Find_Monster(BOSS))
				OBJ_MGR->Find_Monster(BOSS)->Set_Wrong_Beat(true);
			return;
		}

		if (m_InvenArr[ITEM_FOOD].second)
		{
			SOUND_MGR->PlaySound(L"sfx_item_food.ogg", SOUND_EFFECT, 0.5f);
			static_cast<CFood*>(m_InvenArr[ITEM_FOOD].second)->Eat_Food();
			SAFE_DELETE(CObj*)(m_InvenArr[ITEM_FOOD].second);
		}
		BEAT_MGR->Set_Beat_Dead();
	}

	else if (INPUT_MGR->Key_Down('A'))	// 산데비스탄
	{
		if (m_InvenArr[ITEM_SPELL].second)
		{
			m_bMoveTime = true;
			m_bUseSpell = true;
			SOUND_MGR->PlaySound(L"slowly.ogg", SOUND_EFFECT, 0.3f);
			m_dwStop = GetTickCount();
			SOUND_MGR->PauseSound(SOUND_STAGE_ONE);
			SOUND_MGR->PauseSound(SOUND_BOSS_STAGE);
			SOUND_MGR->PauseSound(SOUND_BGM);
			SCENE_MGR->Set_Pause_EveryOne();
			OBJ_MGR->Set_Stop_Monster(true);
			SAFE_DELETE(CObj*)(m_InvenArr[ITEM_SPELL].second);
		}
	}
}

void CPlayer::Set_Scroll()
{
	float fScrollX = -(m_tInfo.fX - WINCX * 0.5f);
	SCROLL_MGR->Set_ScrollX(fScrollX);

	if (m_eDir == UP || m_eDir == DOWN)
	{
		float fScrollY = -(m_tInfo.fY - WINCY * 0.5f);
		SCROLL_MGR->Set_ScrollY(fScrollY);
	}
}

void CPlayer::Render_Dig(HDC _hDC)
{
	float fScrollX = SCROLL_MGR->Get_ScrollX();
	float fScrollY = SCROLL_MGR->Get_ScrollY();

	HDC hEffectDC = BITMAP_MGR->Find_Img(m_InvenArr[ITEM_SHOVEL].second->Get_ImgKey());
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
	tEffectPos.fCX = 48.f;
	tEffectPos.fCY = 48.f;

	GdiTransparentBlt(_hDC, int((tEffectPos.fX - tEffectPos.fCX * 0.5f) + fScrollX),
		int((tEffectPos.fY - tEffectPos.fCY * 0.5f) + fScrollY - 20),
		48, 48, hEffectDC,
		0, 0,
		48, 48, RGB(255, 0, 144));

	if (m_dwTime + 100 < GetTickCount())
		m_bDig = false;
}

void CPlayer::Render_UI(HDC _hDC)
{
	TCHAR buf[100] = {};
	//높이,너비,?,ori,weight,이탤릭,밑줄,?,--------,?,?,퀄리티?
	HFONT hFont = CreateFont(24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("necrosans")); // 폰트변경
	HFONT oldFont = (HFONT)SelectObject(_hDC, hFont); // DC관련
	SetTextAlign(_hDC, TA_LEFT); // 텍스트 정렬
	SetTextColor(_hDC, RGB(255, 255, 255)); // 텍스트 컬러 설정
	SetBkMode(_hDC, TRANSPARENT); // 매개변수 DC의 글자배경을 투명하게 한다. 

	// 체력 칸 출력
	int idx = 0;
	int iEmptyHeart = int((float)m_tState.iMaxHP / 2.f - (float)m_tState.iHP / 2.f);

	// 빈 체력칸 출력
	HDC hMemDC = BITMAP_MGR->Find_Img(L"heart_empty");
	for (idx = 0; idx < iEmptyHeart; ++idx)
	{
		GdiTransparentBlt(_hDC,
			int(m_HPvec[idx].fX - m_HPvec[idx].fCX * 0.5f),
			int(m_HPvec[idx].fY - m_HPvec[idx].fCY * 0.5f),
			int(m_HPvec[idx].fCX), int(m_HPvec[idx].fCY), hMemDC,
			0, 0,
			int(m_HPvec[idx].fCX), int(m_HPvec[idx].fCY), RGB(255, 0, 144));
	}

	// 반칸 체력 출력
	hMemDC = BITMAP_MGR->Find_Img(L"heart_half");
	if (m_tState.iHP % 2 == 1)
	{
		GdiTransparentBlt(_hDC,
			int(m_HPvec[idx].fX - m_HPvec[idx].fCX * 0.5f),
			int(m_HPvec[idx].fY - m_HPvec[idx].fCY * 0.5f),
			int(m_HPvec[idx].fCX), int(m_HPvec[idx].fCY), hMemDC,
			0, 0,
			int(m_HPvec[idx].fCX), int(m_HPvec[idx].fCY), RGB(255, 0, 144));
		++idx;
	}

	// 가득 찬 체력칸 출력
	hMemDC = BITMAP_MGR->Find_Img(L"heart");
	for (auto& iter = m_HPvec.begin() + idx; iter != m_HPvec.end(); ++iter)
	{
		GdiTransparentBlt(_hDC,
			int(iter->fX - iter->fCX * 0.5f),
			int(iter->fY - iter->fCY * 0.5f),
			int(iter->fCX), int(iter->fCY), hMemDC,
			0, 0,
			int(iter->fCX), int(iter->fCY), RGB(255, 0, 144));
	}
	// 인벤토리 출력

	// 골드
	hMemDC = BITMAP_MGR->Find_Img(L"hud_coins");
	_itow_s(m_tState.iMoney, buf, 10); // hp값 int -> wstring
	GdiTransparentBlt(_hDC,
		676, 26,
		48, 48, hMemDC,
		0, 0,
		48, 48, RGB(255, 0, 144));
	TextOut(_hDC, 730, 40, buf, lstrlen(buf));

	// 다이아
	hMemDC = BITMAP_MGR->Find_Img(L"diamond");
	_itow_s(m_tState.iDiamond, buf, 10); // hp값 int -> wstring
	GdiTransparentBlt(_hDC,
		675, 76,
		50, 48, hMemDC,
		0, 0,
		50, 48, RGB(255, 0, 144));
	TextOut(_hDC, 730, 90, buf, lstrlen(buf));

	// 인벤
	for (size_t i = 0; i < ITEM_BOMB; ++i)
	{
		if (m_InvenArr[i].second)	// CObj* 에 값이 들어있으면
		{
			static_cast<CItem*>(m_InvenArr[i].second)->Render_UI(_hDC);
			hMemDC = BITMAP_MGR->Find_Img(m_InvenArr[i].first);

			GdiTransparentBlt(_hDC,
				30 + i * 70, 20,
				60, 66, hMemDC,
				0, 0,
				60, 66, RGB(255, 0, 144));
		}
	}
	// 폭탄, 음식, 스킬
	if (m_InvenArr[ITEM_BOMB].second)
	{
		static_cast<CItem*>(m_InvenArr[ITEM_BOMB].second)->Render_UI(_hDC);
		hMemDC = BITMAP_MGR->Find_Img(m_InvenArr[ITEM_BOMB].first);

		GdiTransparentBlt(_hDC,
			30, 100,
			60, 66, hMemDC,
			0, 0,
			60, 66, RGB(255, 0, 144));

		wsprintf(buf, L"x%d", static_cast<CBomb*>(m_InvenArr[ITEM_BOMB].second)->Get_Bomb_Count());
		TextOut(_hDC, 95, 135, buf, lstrlen(buf));
	}

	if (m_InvenArr[ITEM_FOOD].second)
	{
		static_cast<CItem*>(m_InvenArr[ITEM_FOOD].second)->Render_UI(_hDC);
		hMemDC = BITMAP_MGR->Find_Img(m_InvenArr[ITEM_FOOD].first);

		GdiTransparentBlt(_hDC,
			30, 180,
			60, 66, hMemDC,
			0, 0,
			60, 66, RGB(255, 0, 144));
	}

	if (m_InvenArr[ITEM_SPELL].second)
	{
		static_cast<CItem*>(m_InvenArr[ITEM_SPELL].second)->Render_UI(_hDC);
		hMemDC = BITMAP_MGR->Find_Img(m_InvenArr[ITEM_SPELL].first);

		GdiTransparentBlt(_hDC,
			675, 150,
			64, 84, hMemDC,
			0, 0,
			64,84, RGB(255, 0, 144));
	}
	(HFONT)DeleteObject(oldFont); // 원래 있던폰트 제거
}

void CPlayer::Render_AfterImagin(HDC _hDC)
{
	float fScrollX = SCROLL_MGR->Get_ScrollX();
	float fScrollY = SCROLL_MGR->Get_ScrollY();

	HDC hBodyDC = nullptr;
	HDC hHeadDC = nullptr;

	for (auto& iter = m_ImginList.begin(); iter != m_ImginList.end(); ++iter)
	{
		hBodyDC = BITMAP_MGR->Find_Img(iter->pBody);
		hHeadDC = BITMAP_MGR->Find_Img(iter->pHead);

		GdiTransparentBlt(_hDC, 
			int(iter->tInfo.fX - 24 + fScrollX), 
			int(iter->tInfo.fY - 24 + fScrollY - 20),
			TILECX, TILECY, hBodyDC,
			TILECX * iter->tFrame.iFrameStart, TILECY * iter->tFrame.iMotion,
			TILECX, TILECY, RGB(255, 0, 144));

		GdiTransparentBlt(_hDC,
			int(iter->tInfo.fX - 24 + fScrollX),
			int(iter->tInfo.fY - 24 + fScrollY - 20),
			int(TILECX), int(TILECY), hHeadDC,
			TILECX * iter->tFrame.iFrameStart, 0,
			TILECX, TILECY, RGB(255, 0, 144));
	}
}

bool CPlayer::Collision()
{
	if (m_InvenArr[ITEM_WEAPON].second && 
		static_cast<CWeapon*>(m_InvenArr[ITEM_WEAPON].second)->IsRange(m_eDir)) // 몬스터와 충돌 했을 때 플레이어는 공격을 시도해야 한다.
	{
		Play_Attack_Sound();
		static_cast<CWeapon*>(m_InvenArr[ITEM_WEAPON].second)->Attack(m_eDir);
		static_cast<CWeapon*>(m_InvenArr[ITEM_WEAPON].second)->Set_Effect();
		OBJ_MGR->QuakeOn();
		return true;
	}

	int iX = int(m_tNextPos.fX / TILECX);
	int iY = int(m_tNextPos.fY / TILECY);

	CObj* pObj = TILE_MGR->Peek_Object(m_tNextPos.fX, m_tNextPos.fY);

	if (!pObj)
		return false;

	switch (pObj->Get_ObjID())
	{
	case OBJ_WALL:		// 벽과 충돌한 경우에는 흙벽일 경우 부술 수 있지만 그 외의 벽은 폭탄으로 제거할 수 있다.
		switch (static_cast<CWall*>(pObj)->Get_WallID())
		{
		case WALL_BREAK:
			SOUND_MGR->PlaySound(L"vo_cad_dig_01", SOUND_PLAYER_VOICE, 0.5f);
			SOUND_MGR->PlaySound(L"mov_dig_dirt.ogg", SOUND_EFFECT, 0.6f);
			pObj->Set_Damage(m_tState.iAttack);
			m_bDig = true;
			m_dwTime = GetTickCount();
			OBJ_MGR->QuakeOn();
			break;

		case WALL_BOOM_BREAK:
			if (!lstrcmp(m_InvenArr[ITEM_SHOVEL].second->Get_ImgKey(), L"pickaxe"))
			{
				SOUND_MGR->PlaySound(L"mov_dig_brick.ogg", SOUND_EFFECT, 0.6f);
				static_cast<CWall*>(pObj)->Set_Shovel(m_InvenArr[ITEM_SHOVEL].second->Get_ImgKey());
				m_bDig = true;
				m_dwTime = GetTickCount();
				pObj->Set_Damage(1);
				OBJ_MGR->QuakeOn();
			}
			else
			{
				SOUND_MGR->PlaySound(L"mov_dig_fail.ogg", SOUND_EFFECT, 0.6f);
				static_cast<CWall*>(pObj)->Set_Shovel(m_InvenArr[ITEM_SHOVEL].second->Get_ImgKey());
			}

			break;

		case WALL_UNBREAK:
			SOUND_MGR->PlaySound(L"mov_dig_fail.ogg", SOUND_EFFECT, 0.6f);
			static_cast<CWall*>(pObj)->Set_Shovel(m_InvenArr[ITEM_SHOVEL].second->Get_ImgKey());
			break;

		case WALL_DOOR:
			SOUND_MGR->PlaySound(L"obj_door_open.ogg", SOUND_EFFECT, 0.6f);
			pObj->Set_Damage(m_tState.iAttack);
			OBJ_MGR->QuakeOn();
			break;
		}
		return true;

	case OBJ_TORCH:
		pObj->Set_Damage(m_tState.iAttack);
		OBJ_MGR->QuakeOn();
		return true;

	case OBJ_CHEST:
		if (!lstrcmp(pObj->Get_ImgKey(), L"chest_locked"))
		{
			if (m_InvenArr[ITEM_KEY].second)
			{
				SOUND_MGR->PlaySound(L"obj_chest_open.ogg", SOUND_EFFECT, 0.6f);
				pObj->Set_Damage(m_tState.iAttack);
				SAFE_DELETE(CObj*)(m_InvenArr[ITEM_KEY].second);
			}
		}
		else
		{
			SOUND_MGR->PlaySound(L"obj_chest_open.ogg", SOUND_EFFECT, 0.6f);
			pObj->Set_Damage(m_tState.iAttack);
		}
		OBJ_MGR->QuakeOn();
		return true;

	case OBJ_NPC:
		return true;

	case OBJ_ITEM:
		if (m_tState.iMoney < pObj->Get_State().iMoney || m_tState.iDiamond < pObj->Get_State().iDiamond)
			return false;

		switch (static_cast<CItem*>(pObj)->Get_ItemID())
		{
		case ITEM_SHOVEL:
			if (pObj->Get_State().iMoney > 0 || pObj->Get_State().iDiamond > 0)
				SOUND_MGR->PlaySound(L"sfx_pickup_purchase.ogg", SOUND_EFFECT, 0.6f);
			else
				SOUND_MGR->PlaySound(L"sfx_pickup_general_ST.ogg", SOUND_EFFECT, 0.5f);
			Change_Item();
			break;

		case ITEM_WEAPON:
			if (pObj->Get_State().iMoney > 0 || pObj->Get_State().iDiamond > 0)
				SOUND_MGR->PlaySound(L"sfx_pickup_purchase.ogg", SOUND_EFFECT, 0.6f);
			else
				SOUND_MGR->PlaySound(L"sfx_pickup_weapon.ogg", SOUND_EFFECT, 0.5f);
			Change_Item();
			break;

		case ITEM_ARMOR:
			if (pObj->Get_State().iMoney > 0 || pObj->Get_State().iDiamond > 0)
				SOUND_MGR->PlaySound(L"sfx_pickup_purchase.ogg", SOUND_EFFECT, 0.6f);
			else
				SOUND_MGR->PlaySound(L"sfx_pickup_armor.ogg", SOUND_EFFECT, 0.5f);
			Change_Item();
			m_tFrame.iMotion = int(static_cast<CArmor*>(m_InvenArr[ITEM_ARMOR].second)->Get_Armor());
			m_tState.iDefence = static_cast<CArmor*>(m_InvenArr[ITEM_ARMOR].second)->Get_Defence();
			break;

		case ITEM_TORCH:
			if (pObj->Get_State().iMoney > 0 || pObj->Get_State().iDiamond > 0)
				SOUND_MGR->PlaySound(L"sfx_pickup_purchase.ogg", SOUND_EFFECT, 0.6f);
			else
				SOUND_MGR->PlaySound(L"sfx_pickup_general_ST.ogg", SOUND_EFFECT, 0.5f);
			Change_Item();
			break;

		case ITEM_KEY:
			if (pObj->Get_State().iMoney > 0 || pObj->Get_State().iDiamond > 0)
				SOUND_MGR->PlaySound(L"sfx_pickup_purchase.ogg", SOUND_EFFECT, 0.6f);
			else
				SOUND_MGR->PlaySound(L"sfx_pickup_general_ST.ogg", SOUND_EFFECT, 0.5f);
			Change_Item();
			break;

		case ITEM_BOMB:
			if (pObj->Get_State().iMoney > 0 || pObj->Get_State().iDiamond > 0)
				SOUND_MGR->PlaySound(L"sfx_pickup_purchase.ogg", SOUND_EFFECT, 0.6f);
			else
				SOUND_MGR->PlaySound(L"sfx_pickup_general_ST.ogg", SOUND_EFFECT, 0.5f);

			if (m_InvenArr[ITEM_BOMB].second)
			{
				static_cast<CBomb*>(m_InvenArr[ITEM_BOMB].second)->Add_Bomb(static_cast<CBomb*>(pObj)->Get_Bomb_Count());
				m_tState.iMoney -= pObj->Get_State().iMoney;
				pObj->Set_Money(0);
				OBJ_MGR->Delete_Object(pObj->Get_Info().fX, pObj->Get_Info().fY);
			}
			else
			{
				TILE_MGR->Pop_Object(pObj->Get_Info().fX, pObj->Get_Info().fY);
				OBJ_MGR->Erase_Object(pObj);
				m_InvenArr[ITEM_BOMB].second = pObj;
				pObj->Set_Info_Pos(85.f, 160.f);
				m_tState.iMoney -= pObj->Get_State().iMoney;
				m_tState.iDiamond -= pObj->Get_State().iDiamond;
				pObj->Set_Money(0);
				pObj->Set_Dia(0);
			}
			break;

		case ITEM_FOOD:
			if (pObj->Get_State().iMoney > 0)
				SOUND_MGR->PlaySound(L"sfx_pickup_purchase.ogg", SOUND_EFFECT, 0.6f);
			else
				SOUND_MGR->PlaySound(L"sfx_pickup_general_ST.ogg", SOUND_EFFECT, 0.5f);
			Change_Item();
			pObj->Set_Info_Pos(85.f, 240.f);
			break;

		case ITEM_SPELL:
			if (pObj->Get_State().iMoney > 0)
				SOUND_MGR->PlaySound(L"sfx_pickup_purchase.ogg", SOUND_EFFECT, 0.6f);
			else
				SOUND_MGR->PlaySound(L"sfx_pickup_general_ST.ogg", SOUND_EFFECT, 0.5f);
			Change_Item();
			pObj->Set_Info_Pos(730.f, 210.f);
			break;

		case ITEM_HEART:
			SOUND_MGR->PlaySound(L"sfx_pickup_purchase.ogg", SOUND_EFFECT, 0.6f);
			static_cast<CHeart*>(pObj)->Use_Item();
			m_tState.iDiamond -= pObj->Get_State().iDiamond;
			pObj->Set_Dia(0);
			OBJ_MGR->Delete_Object(pObj->Get_Info().fX, pObj->Get_Info().fY);
			m_HPvec.push_back(m_tHpPos);
			m_tHpPos.fX -= 50.f;
			break;
		}
		return false;

	case OBJ_MONEY:
		SOUND_MGR->PlaySound(L"sfx_pickup_gold_01.ogg", SOUND_EFFECT, 0.6f);
		while (pObj->Get_ObjID() == OBJ_MONEY)
		{
			pObj = TILE_MGR->Pop_Object(m_tNextPos.fX, m_tNextPos.fY);

			if (!pObj)
				break;

			m_tState.iMoney += static_cast<CMoney*>(pObj)->Get_Money();
			pObj->Set_Dead();
		}
		return false;
	}
	return false;
}