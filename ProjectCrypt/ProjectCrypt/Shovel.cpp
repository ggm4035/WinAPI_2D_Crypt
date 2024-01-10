#include "stdafx.h"
#include "Shovel.h"
#include "ScrollMgr.h"
#include "BitmapMgr.h"
#include "TileMgr.h"

CShovel::CShovel()
{
}

CShovel::~CShovel()
{
	Release();
}

bool CShovel::Collision()
{
	return false;
}

void CShovel::Render_Text(HDC _hDC)
{
	if (m_fDistance > sqrtf(TILECX * TILECX + TILECY * TILECY))
		return;

	float fScrollX = SCROLL_MGR->Get_ScrollX();
	float fScrollY = SCROLL_MGR->Get_ScrollY();

	HFONT hFont = CreateFont(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("necrosans")); // ��Ʈ����
	HFONT oldFont = (HFONT)SelectObject(_hDC, hFont); // DC����
	SetTextAlign(_hDC, TA_CENTER); // �ؽ�Ʈ ����
	SetTextColor(_hDC, RGB(255, 255, 255)); // �ؽ�Ʈ �÷� ����
	SetBkMode(_hDC, TRANSPARENT); // �Ű����� DC�� ���ڹ���� �����ϰ� �Ѵ�.

	if (!lstrcmp(m_szImgKey, L"shovel_basic"))
		TextOut(_hDC, int(m_tInfo.fX + fScrollX), int(m_tInfo.fY + fScrollY - 60.f), L"shovel", 7);

	else if (!lstrcmp(m_szImgKey, L"pickaxe"))
		TextOut(_hDC, int(m_tInfo.fX + fScrollX), int(m_tInfo.fY + fScrollY - 60.f), L"pickaxe", 8);

	(HFONT)DeleteObject(oldFont); // ���� �ִ���Ʈ ����
}

void CShovel::Initialize()
{
	m_eID = OBJ_ITEM;
	m_eItemID = ITEM_SHOVEL;
	m_tInfo.fCX = 48.f;
	m_tInfo.fCY = 48.f;
}

int CShovel::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CShovel::Late_Update()
{
	__super::Set_Distance();
}

void CShovel::Render(HDC _hDC)
{
	float fScrollX = SCROLL_MGR->Get_ScrollX();
	float fScrollY = SCROLL_MGR->Get_ScrollY();

	HDC hMemDC = BITMAP_MGR->Find_Img(m_szImgKey);

	GdiTransparentBlt(_hDC, int(m_tRect.left + fScrollX), int(m_tRect.top + fScrollY - 20),
		int(m_tInfo.fCX), int(m_tInfo.fCY), hMemDC,
		0, int((m_tInfo.fCY) * m_tFrame.iMotion),
		int(m_tInfo.fCX), int(m_tInfo.fCY), RGB(255, 0, 144));

	__super::Show_Money(_hDC);
	Render_Text(_hDC);
}

void CShovel::Render_UI(HDC _hDC)
{
	HDC hMemDC = BITMAP_MGR->Find_Img(m_szImgKey);

	GdiTransparentBlt(_hDC, 
		int(m_tInfo.fX - m_tInfo.fCX),
		int(m_tInfo.fY - m_tInfo.fCY),
		int(m_tInfo.fCX), int(m_tInfo.fCY), hMemDC,
		0, 0,
		int(m_tInfo.fCX), int(m_tInfo.fCY), RGB(255, 0, 144));
}

void CShovel::Release()
{
}