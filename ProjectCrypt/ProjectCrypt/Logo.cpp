#include "stdafx.h"
#include "Logo.h"
#include "BitmapMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"

CLogo::CLogo()
{
}

CLogo::~CLogo()
{
	Release();
}

void CLogo::Initialize()
{
}

void CLogo::Update()
{
}

void CLogo::Late_Update()
{
	if (INPUT_MGR->Key_Down(VK_RETURN))
	{
		SCENE_MGR->Scene_Change(SC_MENU);
		return;
	}
}

void CLogo::Render(HDC _hDC)
{
	HDC hMemDC = BITMAP_MGR->Find_Img(L"Logo");

	BitBlt(_hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
}

void CLogo::Release()
{
}
