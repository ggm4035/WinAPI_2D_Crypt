#include "stdafx.h"
#include "TileMgr.h"
#include "ScrollMgr.h"
#include "BitmapMgr.h"
#include "ObjMgr.h"

CTileMgr* CTileMgr::m_pInstance = nullptr;

CTileMgr::CTileMgr()
	:m_pImgKey(nullptr), m_iChangeTile(0)
{
}

CTileMgr::~CTileMgr()
{
	Release();
}

void CTileMgr::Initialize()
{
	Read_Resource();

	m_pImgKey = L"floor_dirt1";

	for (size_t i = 0; i < TILEY; ++i)
	{
		for (size_t j = 0; j < TILEX; ++j)
		{
			float fX = float((TILECX * 0.5f) + (j * TILECX  ));
			float fY = float((TILECY * 0.5f) + (i * TILECY  ));

			int idx = i * TILEX + j;

			CTile* pTile = new CTile;
			pTile->Set_Info_Pos(fX, fY);
			pTile->Set_ImgKey(m_pImgKey);
			pTile->Initialize();
			m_arrTile[idx] = pTile;
		}
	}
}

void CTileMgr::Update()
{
	for (auto& Tile : m_arrTile)
		Tile->Update();
}

void CTileMgr::Late_Update()
{
	for (auto& Tile : m_arrTile)
		Tile->Late_Update();
}

void CTileMgr::Render(HDC _hDC)
{
	if(OBJ_MGR->IsQuake())
	{
		SCROLL_MGR->Quake();
		OBJ_MGR->QuakeOff();
	}
	int iCullX = (int)abs(SCROLL_MGR->Get_ScrollX() / (TILECX ));				// 컬링 시작 값 x
	int iCullY = (int)abs(SCROLL_MGR->Get_ScrollY() / (TILECY ));

	if (SCROLL_MGR->Get_ScrollX() / (TILECX ) > 0) iCullX = 0;
	if (SCROLL_MGR->Get_ScrollY() / (TILECY ) > 0) iCullY = 0;

	int iMaxX = iCullX + WINCX / int(TILECX ) + 2;
	int iMaxY = iCullY + WINCY / int(TILECY ) + 2;

	for (int i = iCullY; i < iMaxY; ++i)
	{
		for (int j = iCullX; j < iMaxX; ++j)
		{
			int		iIndex = i * TILEX + j;

			if (0 > iIndex || m_arrTile.size() <= (size_t)iIndex)
				return;

			if (!lstrcmp(m_arrTile[iIndex]->Get_ImgKey(), L"floor_dirt1"))
				m_arrTile[iIndex]->Set_DrawID((iIndex + m_iChangeTile) % 2 + (m_iChangeTile + 1) % 2, (iIndex + m_iChangeTile) % 2);

			if (!lstrcmp(m_arrTile[iIndex]->Get_ImgKey(), L"boss_floor_A"))
				m_arrTile[iIndex]->Set_DrawID((iIndex + m_iChangeTile) % 2 + (m_iChangeTile + 1) % 2, (iIndex + m_iChangeTile) % 2);

			m_arrTile[iIndex]->Render(_hDC);
		}
	}
}

void CTileMgr::Release()
{
	Clear_Vector();
	for (auto& iter = m_arrTile.begin(); iter != m_arrTile.end(); ++iter)
	{
		if (*iter)
		{
			delete *iter;
			*iter = nullptr;
		}
	}
}

void CTileMgr::Picking(const POINT & _pt, int _iDrawX, int _iDrawY)
{
	int iX = _pt.x / int(TILECX  );
	int iY = _pt.y / int(TILECY  );

	int idx = iY * TILEX + iX;

	if (0 > idx || m_arrTile.size() <= (size_t)idx)
		return;

	m_arrTile[idx]->Set_DrawID(_iDrawX, _iDrawY);
	m_arrTile[idx]->Set_ImgKey(m_pImgKey);
}

void CTileMgr::Save_Tiles(const TCHAR* _pFilePath)
{
	HANDLE hFile = CreateFile(_pFilePath,		// 파일 경로
		GENERIC_WRITE,							// 파일 접근 모드(쓰기 전용), GENERIC_READ(읽기 전용)
		NULL,									// 공유 방식, 파일이 열려 있는 상태에서 다른 프로세스가 오픈 할 때 허가하는 것에 대한 설정, NULL을 지정하면 공유하지 않겠다는 의미
		NULL,									// 보안 속성, NULL인 경우 기본값으로 보안 상태를 설정
		CREATE_ALWAYS,							// 생성 방식, 해당 파일을 열어서 작업을 할 것인지, 새로 만들 것인지 설정(CREATE_ALWAYS : 파일이 없다면 생성, 있다면 덮어쓰기, OPEN_EXISTING : 파일이 있을 경우에만 연다)
		FILE_ATTRIBUTE_NORMAL,					// 파일 속성, FILE_ATTRIBUTE_NORMAL 아무런 속싱 없는 일반적인 파일 생성
		NULL);									// 생성될 파일의 속성을 제공할 템플릿 파일, 안쓰니까 NULL)

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, _T("Save File"), L"Fail", MB_OK);
		return;
	}

	TCHAR szImgKey[32];
	DWORD dwByte = 0;

	for (auto& iter : m_arrTile)
	{
		WriteFile(hFile, &(iter->Get_Info()), sizeof(INFO), &dwByte, nullptr);
		lstrcpy(szImgKey, iter->Get_ImgKey());
		WriteFile(hFile, &szImgKey, sizeof(szImgKey), &dwByte, nullptr);
	}

	CloseHandle(hFile);
}

void CTileMgr::Load_Tiles(const TCHAR* _pFilePath)
{
	Release();
	Clear_Vector();

	HANDLE hFile = CreateFile(_pFilePath,		// 파일 경로
		GENERIC_READ,							// 파일 접근 모드(쓰기 전용), GENERIC_READ(읽기 전용)
		NULL,									// 공유 방식, 파일이 열려 있는 상태에서 다른 프로세스가 오픈 할 때 허가하는 것에 대한 설정, NULL을 지정하면 공유하지 않겠다는 의미
		NULL,									// 보안 속성, NULL인 경우 기본값으로 보안 상태를 설정
		OPEN_EXISTING,							// 생성 방식, 해당 파일을 열어서 작업을 할 것인지, 새로 만들 것인지 설정(CREATE_ALWAYS : 파일이 없다면 생성, 있다면 덮어쓰기, OPEN_EXISTING : 파일이 있을 경우에만 연다)
		FILE_ATTRIBUTE_NORMAL,					// 파일 속성, FILE_ATTRIBUTE_NORMAL 아무런 속싱 없는 일반적인 파일 생성
		NULL);									// 생성될 파일의 속성을 제공할 템플릿 파일, 안쓰니까 NULL)

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, _T("Load File"), L"Fail", MB_OK);
		return;
	}

	int idx = 0;

	TCHAR szTemp[32];
	INFO tInfo{};

	DWORD dwByte = 0;

	while(true)
	{
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);
		ReadFile(hFile, &szTemp, sizeof(szTemp), &dwByte, nullptr);

		if (dwByte == 0)
			break;

		CTile* pTile = new CTile;
		pTile->Set_Info_Pos(tInfo.fX, tInfo.fY);
		pTile->Initialize();
		pTile->Set_ImgKey(szTemp);

		m_arrTile[idx++] = pTile;
	}

	CloseHandle(hFile);
}

void CTileMgr::Clear_Vector()
{
	for (auto& iter = m_arrVector.begin(); iter != m_arrVector.end(); ++iter)
	{
		iter->clear();
	}
}

void CTileMgr::Read_Resource()
{
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/level/floor_dirt1.bmp", L"floor_dirt1");								// 바닥
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/level/boss_floor_A.bmp", L"boss_floor_A");							// 보스 바닥
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/level/TEMP_shop_floor.bmp", L"TEMP_shop_floor");						// 상점
	//BITMAP_MGR->Insert_Bmp(L"../Datas/Data/level/TEMP_floor_hotcoal.bmp", L"TEMP_floor_hotcoal");				// 뜨거운 바닥
	//BITMAP_MGR->Insert_Bmp(L"../Datas/Data/level/TEMP_floor_ice.bmp", L"TEMP_floor_ice");						// 아이스 바닥
	//BITMAP_MGR->Insert_Bmp(L"../Datas/Data/level/TEMP_floor_water.bmp", L"TEMP_floor_water");					// 물웅덩이
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/level/floor_erase.bmp", L"floor_erase");								// 지우개
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/level/stairs.bmp", L"stairs");										// 계단(다음 스테이지로가는)
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/level/stairs_locked_miniboss.bmp", L"stairs_locked_miniboss");		// 미니보스 안잡아서 못내려가는 계단
}