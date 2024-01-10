#include "stdafx.h"
#include "MainGame.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "TileMgr.h"
#include "TimeMgr.h"
#include "ObjMgr.h"
#include "SoundMgr.h"
#include "BeatMgr.h"

CMainGame::CMainGame()
	:m_dwTime(GetTickCount()), m_iFPS(0)
{
	ZeroMemory(m_szFPS, sizeof(m_szFPS));
}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	m_hDC = GetDC(g_hWnd);

	BEAT_MGR->Initialize();
	TILE_MGR->Initialize();
	OBJ_MGR->Initialize();
	SCENE_MGR->Initialize();
	SCENE_MGR->Scene_Change(SC_LOGO);
	SOUND_MGR->Initialize();
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/Back.bmp", L"Back");

	OBJ_MGR->Make_Player();

#ifdef _DEBUG
/*
	if (::AllocConsole() == TRUE)
	{
		FILE* nfp[3];
		freopen_s(nfp + 0, "CONOUT$", "rb", stdin);
		freopen_s(nfp + 1, "CONOUT$", "wb", stdout);
		freopen_s(nfp + 2, "CONOUT$", "wb", stderr);
		std::ios::sync_with_stdio();
	}*/

#endif // _DEBUG
}

void CMainGame::Update()
{
	SCENE_MGR->Update();
}

void CMainGame::Late_UPdate()
{
	SCENE_MGR->Late_Update();
}

void CMainGame::Render()
{
	++m_iFPS;

	if (m_dwTime + 1000 < GetTickCount())
	{
		swprintf_s(m_szFPS, L"FPS : %d", m_iFPS);
		SetWindowText(g_hWnd, m_szFPS);

		m_iFPS = 0;
		m_dwTime = GetTickCount();
	}

	HDC hBackDC = BITMAP_MGR->Find_Img(L"Back");

	BitBlt(m_hDC, 0, 0, WINCX, WINCY, hBackDC, 0, 0, SRCCOPY);

	SCENE_MGR->Render(hBackDC);
}

void CMainGame::Release()
{
	TILE_MGR->Destroy_Instance();
	SCENE_MGR->Destroy_Instance();
	OBJ_MGR->Destroy_Instance();
	BITMAP_MGR->Destroy_Instance();
	INPUT_MGR->Destroy_Instance();
	SCROLL_MGR->Destroy_Instance();
	TIME_MGR->Destroy_Instance();
	SOUND_MGR->Destroy_Instance();
	BEAT_MGR->Destroy_Instance();
	ReleaseDC(g_hWnd, m_hDC);
}
