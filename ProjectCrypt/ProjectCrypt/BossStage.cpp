#include "stdafx.h"
#include "BossStage.h"
#include "TileMgr.h"
#include "ObjMgr.h"
#include "SoundMgr.h"
#include "BeatMgr.h"
#include "BitmapMgr.h"
#include "ScrollMgr.h"
#include "SceneMgr.h"
#include "Boss.h"

CBossStage::CBossStage()
	:m_iBeatCount(0), m_bClose(false), m_pPlayer(nullptr), m_fVolume(0.f), m_bEnd(false), m_bStart(true),
	m_dwTime(GetTickCount()), m_iMoveLen(0), m_bTurn(false), m_bEnding(false)
{
}

CBossStage::~CBossStage()
{
	Release();
}

void CBossStage::Initialize()
{
	BEAT_MGR->Release();
	TILE_MGR->Load_Tiles(L"../Maps/BossStageTile.dat");
	OBJ_MGR->Load_Object(L"../Maps/BossStageObj.dat");
	m_pPlayer = OBJ_MGR->Get_Player();
	m_pPlayer->Set_Info_Pos(600.f, 1224.f);
	TILE_MGR->Push_Object(m_pPlayer->Get_Info().fX, m_pPlayer->Get_Info().fY, m_pPlayer);
	m_pPlayer->Reset_Direction();

	SOUND_MGR->StopSound(SOUND_LOBBY);
	TIME_MGR->Reset();
	TIME_MGR->Set_BPM(121 * g_iSpeed);
	TIME_MGR->Set_Delay(1.f * float(g_iSpeed));
	BEAT_MGR->Set_Delay(-810);
	SOUND_MGR->PlaySound(L"vo_announcer_kingconga.ogg", SOUND_MONSTER_VOICE, 0.3f);
	SOUND_MGR->PlayBGM(L"boss_1.ogg", SOUND_BOSS_STAGE, 0.f);
}

void CBossStage::Update()
{
	static bool bBeatEnd = false;

	if (TIME_MGR->Tick())
	{
		if (g_iSpeed == 1)
		{
			if (m_iBeatCount < 7)
			{
				BEAT_MGR->Make_Beat();
				++m_iBeatCount;
			}
			else
				m_iBeatCount = 0;
		}
		else
		{
			if (m_iBeatCount < 13)
			{
				BEAT_MGR->Make_Beat();
				++m_iBeatCount;
			}
			else
			{
				if (bBeatEnd)
				{
					m_iBeatCount = 0;
					bBeatEnd = false;
					TIME_MGR->Reset();
				}
				else
				{
					--m_iBeatCount;
					bBeatEnd = true;
				}
			}
		}
	}

	if (m_bClose && !m_bEnd && !OBJ_MGR->Find_Monster(BOSS))
	{
		TILE_MGR->Load_Tiles(L"../Maps/BossStageTileEnd.dat");
		OBJ_MGR->Load_Object(L"../Maps/BossStageObjEnd.dat");
		m_bEnd = true;
	}

	BEAT_MGR->Update();
	TILE_MGR->Update();
	OBJ_MGR->Update();
}

void CBossStage::Late_Update()
{
	if (OBJ_MGR->Get_Player()->Get_Info().fY > 888.f)
	{
		m_fVolume = 0.175f - (OBJ_MGR->Get_Player()->Get_Info().fY - 888.f) / 48.f * 0.025f;
		SOUND_MGR->SetChannelVolume(SOUND_BOSS_STAGE, m_fVolume);
	}
	if (!m_bClose && OBJ_MGR->Get_Player()->Get_Info().fY == 888.f)
	{
		OBJ_MGR->Set_ImgKey(L"boss_wall");
		SOUND_MGR->PlaySound(L"boss_zone1_walls.ogg", SOUND_BGM, 0.3f);
		TILE_MGR->Load_Tiles(L"../Maps/BossStageTileStart.dat");
		OBJ_MGR->Load_Object(L"../Maps/BossStageObjStart.dat");
		m_bClose = true;
	}

	if (!m_bEnding && !OBJ_MGR->Find_Monster(ZOMBIE))
	{
		if (OBJ_MGR->Find_Monster(BOSS))
		{
			OBJ_MGR->Find_Monster(BOSS)->Set_Wrong_Beat(true);
			static_cast<CBoss*>(OBJ_MGR->Find_Monster(BOSS))->Set_Finish();
		}
	}

	if (!m_bEnding && (int)OBJ_MGR->Get_Player()->Get_Info().fX == 600 &&
		(int)OBJ_MGR->Get_Player()->Get_Info().fY == 216)
	{
		TIME_MGR->Rap_end();
		SOUND_MGR->StopAll();
		m_bEnding = true;
	}

	BEAT_MGR->Late_Update();
	TILE_MGR->Late_Update();
	OBJ_MGR->Late_Update();
}

void CBossStage::Render(HDC _hDC)
{
	TCHAR buf[16] = {};

	HFONT hFont = CreateFont(36, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("necrosans")); // ��Ʈ����
	HFONT oldFont = (HFONT)SelectObject(_hDC, hFont); // DC����
	SetTextAlign(_hDC, TA_CENTER); // �ؽ�Ʈ ����
	SetTextColor(_hDC, RGB(255, 255, 255)); // �ؽ�Ʈ �÷� ����
	SetBkMode(_hDC, TRANSPARENT); // �Ű����� DC�� ���ڹ���� �����ϰ� �Ѵ�. 

	int iMilli = int(TIME_MGR->Get_Cur_Rap() % 100);
	int iSec = int(TIME_MGR->Get_Cur_Rap() % 6000 / 100);
	int iMin = int(TIME_MGR->Get_Cur_Rap() / 6000);

	swprintf_s(buf, L"%d:%d:%d", iMin, iSec, iMilli);

	float fScrollX = SCROLL_MGR->Get_ScrollX();
	float fScrollY = SCROLL_MGR->Get_ScrollY();

	HDC hMemDC = BITMAP_MGR->Find_Img(L"Ground");


	BitBlt(_hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
	if (!m_bEnding)
	{
		TILE_MGR->Render(_hDC);
		OBJ_MGR->Render(_hDC);
		BEAT_MGR->Render(_hDC);

		HFONT hFont = CreateFont(36, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			VARIABLE_PITCH | FF_ROMAN, TEXT("necrosans")); // ��Ʈ����
		HFONT oldFont = (HFONT)SelectObject(_hDC, hFont); // DC����
		SetTextAlign(_hDC, TA_CENTER); // �ؽ�Ʈ ����
		SetTextColor(_hDC, RGB(255, 255, 255)); // �ؽ�Ʈ �÷� ����
		SetBkMode(_hDC, TRANSPARENT); // �Ű����� DC�� ���ڹ���� �����ϰ� �Ѵ�.

		TextOut(_hDC, 400, 36, buf, lstrlen(buf));
	}
	else
	{
		iMilli = int(TIME_MGR->Get_End_Rap() % 100);
		iSec = int(TIME_MGR->Get_End_Rap() % 6000 / 100);
		iMin = int(TIME_MGR->Get_End_Rap() / 6000);

		swprintf_s(buf, L"%d:%d:%d", iMin, iSec, iMilli);

		TextOut(_hDC, 400, 250, L"THANK YOU FOR WATCHING!", 24);
		TextOut(_hDC, 400, 300, L"RECORD", 7);
		TextOut(_hDC, 400, 350, buf, lstrlen(buf));
	}

	if (m_bStart)
		Render_CutScene(_hDC);

	(HFONT)DeleteObject(oldFont); // ���� �ִ���Ʈ ����
}

void CBossStage::Release()
{
}

void CBossStage::Render_CutScene(HDC _hDC)
{

	//����,�ʺ�,?,ori,weight,���Ÿ�,����,?,--------,?,?,����Ƽ?
	HFONT hFont = CreateFont(40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("necrosans")); // ��Ʈ����
	HFONT oldFont = (HFONT)SelectObject(_hDC, hFont); // DC����
	SetTextAlign(_hDC, TA_LEFT); // �ؽ�Ʈ ����
	SetTextColor(_hDC, RGB(255, 255, 255)); // �ؽ�Ʈ �÷� ����
	SetBkMode(_hDC, TRANSPARENT); // �Ű����� DC�� ���ڹ���� �����ϰ� �Ѵ�. 
	HDC hCutDC = BITMAP_MGR->Find_Img(L"bg_gradient");

	GdiTransparentBlt(_hDC, 820 - m_iMoveLen, 262,
		960, 148, hCutDC,
		0, 0,
		960, 148, RGB(255, 0, 144));	// �����ʿ��� ����

	hCutDC = BITMAP_MGR->Find_Img(L"bg_topblade");

	GdiTransparentBlt(_hDC, -1200 + m_iMoveLen, 194,
		602, 68, hCutDC,
		0, 0,
		602, 68, RGB(255, 0, 144));		// ���ʿ��� ����

	hCutDC = BITMAP_MGR->Find_Img(L"bosscreen_kingconga");

	GdiTransparentBlt(_hDC, 964 - m_iMoveLen, 140,
		672, 273, hCutDC,
		0, 0,
		960, 390, RGB(255, 0, 144));	// �����ʿ��� ����

	hCutDC = BITMAP_MGR->Find_Img(L"bg_bottomblade");

	GdiTransparentBlt(_hDC, 1250 - m_iMoveLen, 410,
		728, 96, hCutDC,
		0, 0,
		728, 96, RGB(255, 0, 144));		// �����ʿ��� ����

	TextOut(_hDC, 1360 - m_iMoveLen, 450, L"King Conga", 11);

	if (m_bTurn)
		m_iMoveLen -= 20;

	else if(m_iMoveLen != 900)
		m_iMoveLen += 20;

	if (m_iMoveLen < 0)
		m_bStart = false;

	if (m_iMoveLen >= 900)
	{
		if(m_dwTime + 2000 < GetTickCount())
			m_bTurn = true;
	}

	(HFONT)DeleteObject(oldFont); // ���� �ִ���Ʈ ����
}
