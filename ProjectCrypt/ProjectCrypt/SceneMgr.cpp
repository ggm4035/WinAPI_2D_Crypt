#include "stdafx.h"
#include "SceneMgr.h"
#include "BitmapMgr.h"
#include "Logo.h"
#include "MainMenu.h"
#include "Lobby.h"
#include "MyEdit.h"
#include "Stage1.h"
#include "BossStage.h"
#include "ObjMgr.h"
#include "TileMgr.h"
#include "BeatMgr.h"

CSceneMgr* CSceneMgr::m_pInstance = nullptr;

CSceneMgr::CSceneMgr()
	:m_pScene(nullptr), m_eCurScene(SC_LOGO), m_ePreScene(SC_END), m_bPause(false)
{
}

CSceneMgr::~CSceneMgr()
{
	Release();
}

void CSceneMgr::Scene_Change(SCENEID _eSceneID)
{
	m_eCurScene = _eSceneID;

	if (_eSceneID == SC_LOBBY || m_eCurScene != m_ePreScene)
	{
		SAFE_DELETE(CScene*)(m_pScene);

		switch (m_eCurScene)
		{
		case SC_LOGO:
			m_pScene = new CLogo;
			break;

		case SC_MENU:
			m_pScene = new CMainMenu;
			break;

		case SC_LOBBY:
			m_pScene = new CLobby;
			break;

		case SC_EDIT:
			m_pScene = new CMyEdit;
			break;

		case SC_STAGE1:
			m_pScene = new CStage1;
			break;

		case SC_BOSS_STAGE:
			m_pScene = new CBossStage;
			break;
		}

		m_pScene->Initialize();
		m_ePreScene = m_eCurScene;
	}
}

void CSceneMgr::Initialize()
{
	//Ground
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/Ground.bmp", L"Ground");

	// 로고 저장
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/Logo.bmp", L"Logo");

	// 게임화면
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/mainmenu/mainmenu.bmp", L"MainMenu");
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/mainmenu/continue.bmp", L"Continue");

	// 보스 그림
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/gui/bg_bottomblade.bmp", L"bg_bottomblade");
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/gui/bg_gradient.bmp", L"bg_gradient");
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/gui/bg_topblade.bmp", L"bg_topblade");
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/gui/bosscreen_kingconga.bmp", L"bosscreen_kingconga");
}

void CSceneMgr::Update()
{
	if (m_bPause)
	{
		OBJ_MGR->Update();
	}
	else
		m_pScene->Update();
}

void CSceneMgr::Late_Update()
{
	if (m_bPause)
	{
		OBJ_MGR->Late_Update();
	}
	else
		m_pScene->Late_Update();
}

void CSceneMgr::Render(HDC _hDC)
{
	m_pScene->Render(_hDC);
}

void CSceneMgr::Release()
{
	SAFE_DELETE(CScene*)(m_pScene);
}