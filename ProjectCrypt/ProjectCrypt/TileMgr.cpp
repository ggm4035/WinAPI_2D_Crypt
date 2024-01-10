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
	int iCullX = (int)abs(SCROLL_MGR->Get_ScrollX() / (TILECX ));				// �ø� ���� �� x
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
	HANDLE hFile = CreateFile(_pFilePath,		// ���� ���
		GENERIC_WRITE,							// ���� ���� ���(���� ����), GENERIC_READ(�б� ����)
		NULL,									// ���� ���, ������ ���� �ִ� ���¿��� �ٸ� ���μ����� ���� �� �� �㰡�ϴ� �Ϳ� ���� ����, NULL�� �����ϸ� �������� �ʰڴٴ� �ǹ�
		NULL,									// ���� �Ӽ�, NULL�� ��� �⺻������ ���� ���¸� ����
		CREATE_ALWAYS,							// ���� ���, �ش� ������ ��� �۾��� �� ������, ���� ���� ������ ����(CREATE_ALWAYS : ������ ���ٸ� ����, �ִٸ� �����, OPEN_EXISTING : ������ ���� ��쿡�� ����)
		FILE_ATTRIBUTE_NORMAL,					// ���� �Ӽ�, FILE_ATTRIBUTE_NORMAL �ƹ��� �ӽ� ���� �Ϲ����� ���� ����
		NULL);									// ������ ������ �Ӽ��� ������ ���ø� ����, �Ⱦ��ϱ� NULL)

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

	HANDLE hFile = CreateFile(_pFilePath,		// ���� ���
		GENERIC_READ,							// ���� ���� ���(���� ����), GENERIC_READ(�б� ����)
		NULL,									// ���� ���, ������ ���� �ִ� ���¿��� �ٸ� ���μ����� ���� �� �� �㰡�ϴ� �Ϳ� ���� ����, NULL�� �����ϸ� �������� �ʰڴٴ� �ǹ�
		NULL,									// ���� �Ӽ�, NULL�� ��� �⺻������ ���� ���¸� ����
		OPEN_EXISTING,							// ���� ���, �ش� ������ ��� �۾��� �� ������, ���� ���� ������ ����(CREATE_ALWAYS : ������ ���ٸ� ����, �ִٸ� �����, OPEN_EXISTING : ������ ���� ��쿡�� ����)
		FILE_ATTRIBUTE_NORMAL,					// ���� �Ӽ�, FILE_ATTRIBUTE_NORMAL �ƹ��� �ӽ� ���� �Ϲ����� ���� ����
		NULL);									// ������ ������ �Ӽ��� ������ ���ø� ����, �Ⱦ��ϱ� NULL)

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
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/level/floor_dirt1.bmp", L"floor_dirt1");								// �ٴ�
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/level/boss_floor_A.bmp", L"boss_floor_A");							// ���� �ٴ�
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/level/TEMP_shop_floor.bmp", L"TEMP_shop_floor");						// ����
	//BITMAP_MGR->Insert_Bmp(L"../Datas/Data/level/TEMP_floor_hotcoal.bmp", L"TEMP_floor_hotcoal");				// �߰ſ� �ٴ�
	//BITMAP_MGR->Insert_Bmp(L"../Datas/Data/level/TEMP_floor_ice.bmp", L"TEMP_floor_ice");						// ���̽� �ٴ�
	//BITMAP_MGR->Insert_Bmp(L"../Datas/Data/level/TEMP_floor_water.bmp", L"TEMP_floor_water");					// ��������
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/level/floor_erase.bmp", L"floor_erase");								// ���찳
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/level/stairs.bmp", L"stairs");										// ���(���� ���������ΰ���)
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/level/stairs_locked_miniboss.bmp", L"stairs_locked_miniboss");		// �̴Ϻ��� ����Ƽ� ���������� ���
}