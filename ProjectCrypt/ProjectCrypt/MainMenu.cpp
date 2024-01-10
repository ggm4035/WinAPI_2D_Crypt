#include "stdafx.h"
#include "MainMenu.h"
#include "BitmapMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"

CMainMenu::CMainMenu()
	:m_dwTime(0), m_bBlink(true)
{
}

CMainMenu::~CMainMenu()
{
	Release();
}

void CMainMenu::Initialize()
{
	m_dwTime = GetTickCount();
	SOUND_MGR->PlayBGM(L"zone1_3.ogg", SOUND_MAINMENU, 0.15f);
}

void CMainMenu::Update()
{
	if (m_dwTime + 466 < GetTickCount())
	{
		m_bBlink = !m_bBlink;
		m_dwTime = GetTickCount();
	}
}

void CMainMenu::Late_Update()
{
	if (INPUT_MGR->Key_Down(VK_SPACE))
	{
		SCENE_MGR->Scene_Change(SC_LOBBY);
		SOUND_MGR->StopSound(SOUND_MAINMENU);
		return;
	}
}

void CMainMenu::Render(HDC _hDC)
{
	HDC hGroundDC = BITMAP_MGR->Find_Img(L"Ground");
	HDC hMemDC = BITMAP_MGR->Find_Img(L"MainMenu");
	HDC hConDC = BITMAP_MGR->Find_Img(L"Continue");

	if (m_bBlink)
	{
		BitBlt(_hDC, 0, 0, WINCX, WINCY, hGroundDC, 0, 0, SRCCOPY);
		BitBlt(_hDC, -80, 0, WINCX + 80, WINCY, hMemDC, 0, 0, SRCCOPY);
		GdiTransparentBlt(_hDC, 180, 502, 440, 36, hConDC, 0, 0, 440, 36, RGB(255, 0, 144));
	}
	else
	{
		BitBlt(_hDC, 0, 0, WINCX, WINCY, hGroundDC, 0, 0, SRCCOPY);
		BitBlt(_hDC, -80, 0, WINCX + 80, WINCY, hMemDC, 0, 0, SRCCOPY);
	}
}

void CMainMenu::Release()
{
}
