#include "stdafx.h"
#include "Lobby.h"
#include "SceneMgr.h"
#include "TileMgr.h"
#include "BitmapMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"
#include "ObjMgr.h"
#include "SoundMgr.h"
#include "TimeMgr.h"
#include "BeatMgr.h"
#include "ScrollMgr.h"

void CLobby::Move_Frame()
{
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;

		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			m_tFrame.iFrameStart = 0;

		m_tFrame.dwTime = GetTickCount();
	}
}

CLobby::CLobby()
	:m_pPlayer(nullptr)
{
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CLobby::~CLobby()
{
	Release();
}

void CLobby::Initialize()
{
	BEAT_MGR->Release();
	TILE_MGR->Load_Tiles(L"../Maps/LobbyTile.dat");
	OBJ_MGR->Load_Object(L"../Maps/LobbyObj.dat");

	m_pPlayer = OBJ_MGR->Get_Player();
	m_pPlayer->Set_Info_Pos(600.f, 600.f);
	TILE_MGR->Push_Object(m_pPlayer->Get_Info().fX, m_pPlayer->Get_Info().fY, m_pPlayer);
	m_pPlayer->Reset_Direction();

	TIME_MGR->Reset();
	TIME_MGR->Set_BPM(131 * g_iSpeed);
	TIME_MGR->Set_Delay(0.5f * float(g_iSpeed));
	BEAT_MGR->Set_Delay(-100);
	SOUND_MGR->PlayBGM(L"lobby.ogg", SOUND_LOBBY, 0.15f);

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.dwSpeed = 250;
	m_tFrame.iMotion = 0;
}

void CLobby::Update()
{
	if (TIME_MGR->Tick())
		BEAT_MGR->Make_Beat();

	BEAT_MGR->Update();
	TILE_MGR->Update();
	OBJ_MGR->Update();
}

void CLobby::Late_Update()
{
	if ((int)OBJ_MGR->Get_Player()->Get_Info().fX == 744 &&
		(int)OBJ_MGR->Get_Player()->Get_Info().fY == 984)
	{
		SCENE_MGR->Scene_Change(SC_EDIT);
		SOUND_MGR->StopSound(SOUND_LOBBY);
	}

	else if ((int)OBJ_MGR->Get_Player()->Get_Info().fX == 456 &&
		(int)OBJ_MGR->Get_Player()->Get_Info().fY == 984)
	{
		SCENE_MGR->Scene_Change(SC_STAGE1);
		SOUND_MGR->StopSound(SOUND_LOBBY);
	}

	else if ((int)OBJ_MGR->Get_Player()->Get_Info().fX == 600 &&
		(int)OBJ_MGR->Get_Player()->Get_Info().fY == 984)
	{
		SCENE_MGR->Scene_Change(SC_BOSS_STAGE);
		SOUND_MGR->StopSound(SOUND_LOBBY);
	}

	else if ((int)OBJ_MGR->Get_Player()->Get_Info().fX == 456 &&
		(int)OBJ_MGR->Get_Player()->Get_Info().fY == 504)
	{
		g_iSpeed = 2;
		TILE_MGR->Pop_Object(m_pPlayer->Get_Info().fX, m_pPlayer->Get_Info().fY);
		m_pPlayer->Set_Info_Pos(600.f, 600.f);
		TILE_MGR->Push_Object(m_pPlayer->Get_Info().fX, m_pPlayer->Get_Info().fY, m_pPlayer);
		m_pPlayer->Reset_Direction();
		TIME_MGR->Set_BPM(130 * g_iSpeed);
	}

	else if ((int)OBJ_MGR->Get_Player()->Get_Info().fX == 744 &&
		(int)OBJ_MGR->Get_Player()->Get_Info().fY == 504)
	{
		g_iSpeed = 1;
		TILE_MGR->Pop_Object(m_pPlayer->Get_Info().fX, m_pPlayer->Get_Info().fY);
		m_pPlayer->Set_Info_Pos(600.f, 600.f);
		TILE_MGR->Push_Object(m_pPlayer->Get_Info().fX, m_pPlayer->Get_Info().fY, m_pPlayer);
		m_pPlayer->Reset_Direction();
		TIME_MGR->Set_BPM(130 * g_iSpeed);
	}

	BEAT_MGR->Late_Update();
	TILE_MGR->Late_Update();
	OBJ_MGR->Late_Update();
	Move_Frame();
}

void CLobby::Render(HDC _hDC)
{
	float fScrollX = SCROLL_MGR->Get_ScrollX();
	float fScrollY = SCROLL_MGR->Get_ScrollY();

	HDC hMemDC = BITMAP_MGR->Find_Img(L"Ground");

	BitBlt(_hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
	TILE_MGR->Render(_hDC);

	hMemDC = BITMAP_MGR->Find_Img(L"dungeon_master");

	GdiTransparentBlt(_hDC, int(543 + fScrollX), int(163 + fScrollY - 20),
		114, 106, hMemDC,
		int(114 * m_tFrame.iFrameStart), int(106 * m_tFrame.iMotion),
		114, 106, RGB(255, 0, 144));
	OBJ_MGR->Render(_hDC);

	HFONT hFont = CreateFont(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("necrosans")); // 폰트변경
	HFONT oldFont = (HFONT)SelectObject(_hDC, hFont); // DC관련
	SetTextAlign(_hDC, TA_CENTER); // 텍스트 정렬
	SetTextColor(_hDC, RGB(255, 255, 255)); // 텍스트 컬러 설정
	SetBkMode(_hDC, TRANSPARENT); // 매개변수 DC의 글자배경을 투명하게 한다. 

	TextOut(_hDC, 744 + int(fScrollX), 934 + int(fScrollY), L"edit", 5);
	TextOut(_hDC, 456 + int(fScrollX), 934 + int(fScrollY), L"stage", 6);
	TextOut(_hDC, 600 + int(fScrollX), 934 + int(fScrollY), L"boss", 5);
	TextOut(_hDC, 456 + int(fScrollX), 454 + int(fScrollY), L"speed x2", 8);
	TextOut(_hDC, 744 + int(fScrollX), 454 + int(fScrollY), L"speed x1", 8);
	(HFONT)DeleteObject(oldFont); // 원래 있던폰트 제거

	BEAT_MGR->Render(_hDC);
}

void CLobby::Release()
{
}
