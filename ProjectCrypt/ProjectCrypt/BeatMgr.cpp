#include "stdafx.h"
#include "BeatMgr.h"
#include "BitmapMgr.h"
#include "TileMgr.h"
#include "ObjMgr.h"

CBeatMgr* CBeatMgr::m_pInstance = nullptr;

CBeatMgr::CBeatMgr()
	:m_iDrawX(0), m_dwTime(GetTickCount()), m_fDelay(0.f)
{
}

CBeatMgr::~CBeatMgr()
{
	Release();
}

void CBeatMgr::Initialize()
{
	m_fDelay = -100.f;
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/gui/TEMP_beat_heart.bmp", L"TEMP_beat_heart");
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/gui/TEMP_beat_marker.bmp", L"TEMP_beat_marker");
}

void CBeatMgr::Update()
{
	int iResult = 0;
	for (auto& iter = m_BeatList.begin(); iter != m_BeatList.end(); )
	{
		iResult = (*iter)->Update();

		if (BEAT_HITTING == iResult)
		{
			OBJ_MGR->Get_Player()->Set_MoveTime(true);
			++iter;
		}

		else if (BEAT_DEAD == iResult)
		{
			SAFE_DELETE(CBeat*)(*iter);
			iter = m_BeatList.erase(iter);
			OBJ_MGR->Get_Player()->Set_MoveTime(false);
			OBJ_MGR->Set_Hitting_Obj(OBJ_MONSTER);
			OBJ_MGR->Set_Hitting_Obj(OBJ_ITEM);
			TILE_MGR->Add_Count();
			m_iDrawX = 1;
			m_dwTime = GetTickCount();
		}

		else
			++iter;
	}
}

void CBeatMgr::Late_Update()
{
	if (m_dwTime + 100 < GetTickCount())
		m_iDrawX = 0;

	for (auto& iter : m_BeatList)
		iter->Late_Update();
}

void CBeatMgr::Render(HDC _hDC)
{
	HDC hMemDC = BITMAP_MGR->Find_Img(L"TEMP_beat_heart");

	for (auto& iter : m_BeatList)
		iter->Render(_hDC);

	GdiTransparentBlt(_hDC, 363, 475,
		82, 104, hMemDC,
		82 * m_iDrawX, 0,
		82, 104, RGB(255, 0, 144));
}

void CBeatMgr::Release()
{
	for_each(m_BeatList.begin(), m_BeatList.end(), SAFE_DELETE(CBeat*));
	m_BeatList.clear();
}

void CBeatMgr::Make_Beat()
{
	CBeat* pBeat = new CBeat;
	pBeat->Initialize();
	pBeat->Set_Pos(m_fDelay);
	m_BeatList.push_back(pBeat);
}
