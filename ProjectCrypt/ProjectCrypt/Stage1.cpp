#include "stdafx.h"
#include "Stage1.h"
#include "TimeMgr.h"
#include "SoundMgr.h"
#include "TileMgr.h"
#include "ObjMgr.h"
#include "BeatMgr.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"

CStage1::CStage1()
	:m_pPlayer(nullptr), m_bOpen(false)
{
}

CStage1::~CStage1()
{
	Release();
}

void CStage1::Initialize()
{
	BEAT_MGR->Release();
	SOUND_MGR->StopSound(SOUND_LOBBY);
	TILE_MGR->Load_Tiles(L"../Maps/StageOneTile.dat");
	OBJ_MGR->Load_Object(L"../Maps/StageOneObj.dat");

	m_pPlayer = OBJ_MGR->Get_Player();
	m_pPlayer->Set_Info_Pos(744.f, 792.f);
	TILE_MGR->Push_Object(m_pPlayer->Get_Info().fX, m_pPlayer->Get_Info().fY, m_pPlayer);
	m_pPlayer->Reset_Direction();

	TIME_MGR->Reset();
	TIME_MGR->Set_BPM(116 * g_iSpeed);
	TIME_MGR->Set_Delay(0.4f * float(g_iSpeed));
	BEAT_MGR->Set_Delay(-150);
	SOUND_MGR->PlayBGM(L"zone1_1.ogg", SOUND_STAGE_ONE, 0.15f);
	TIME_MGR->Rap_Start();
}

void CStage1::Update()
{
	if (TIME_MGR->Tick())
		BEAT_MGR->Make_Beat();

	if (!OBJ_MGR->Find_Monster(GREEN_DRAGON))
	{
		m_bOpen = true; 
		TILE_MGR->Set_ImgKey(L"stairs");
		TILE_MGR->Picking(POINT{ 1224, 1176 }, 0, 0);
	}

	BEAT_MGR->Update();
	TILE_MGR->Update();
	OBJ_MGR->Update();
}

void CStage1::Late_Update()
{
	if (m_bOpen && (int)OBJ_MGR->Get_Player()->Get_Info().fX == 1224 &&
		(int)OBJ_MGR->Get_Player()->Get_Info().fY == 1176)
	{
		SCENE_MGR->Scene_Change(SC_BOSS_STAGE);
		SOUND_MGR->StopSound(SOUND_STAGE_ONE);
		SOUND_MGR->StopSound(SOUND_BGM);
	}

	BEAT_MGR->Late_Update();
	TILE_MGR->Late_Update();
	OBJ_MGR->Late_Update();
}

void CStage1::Render(HDC _hDC)
{
	TCHAR buf[16] = {};

	int iMilli = int(TIME_MGR->Get_Cur_Rap() % 100);
	int iSec = int(TIME_MGR->Get_Cur_Rap() % 6000 / 100);
	int iMin = int(TIME_MGR->Get_Cur_Rap() / 6000);

	swprintf_s(buf, L"%d:%d:%d", iMin, iSec, iMilli);

	HDC hMemDC = BITMAP_MGR->Find_Img(L"Ground");

	BitBlt(_hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

	TILE_MGR->Render(_hDC);
	OBJ_MGR->Render(_hDC);
	BEAT_MGR->Render(_hDC);

	//높이,너비,?,ori,weight,이탤릭,밑줄,?,--------,?,?,퀄리티?
	HFONT hFont = CreateFont(36, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("necrosans")); // 폰트변경
	HFONT oldFont = (HFONT)SelectObject(_hDC, hFont); // DC관련
	SetTextAlign(_hDC, TA_CENTER); // 텍스트 정렬
	SetTextColor(_hDC, RGB(255, 255, 255)); // 텍스트 컬러 설정
	SetBkMode(_hDC, TRANSPARENT); // 매개변수 DC의 글자배경을 투명하게 한다.

	TextOut(_hDC, 400, 36, buf, lstrlen(buf));

	(HFONT)DeleteObject(oldFont); // 원래 있던폰트 제거
}

void CStage1::Release()
{
}
