#include "stdafx.h"
#include "Armor.h"
#include "ScrollMgr.h"
#include "BitmapMgr.h"
#include "TileMgr.h"
#include "ObjMgr.h"

CArmor::CArmor()
	:m_eArmor(NORMAL)
{
}

CArmor::~CArmor()
{
	Release();
}

bool CArmor::Collision()
{
	return false;
}

void CArmor::Initialize()
{
	m_eID = OBJ_ITEM;
	m_eItemID = ITEM_ARMOR;
	m_tInfo.fCX = 48.f;
	m_tInfo.fCY = 48.f;
}

int CArmor::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CArmor::Late_Update()
{
	__super::Set_Distance();
}

void CArmor::Render(HDC _hDC)
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

void CArmor::Release()
{
}

void CArmor::Render_UI(HDC _hDC)
{
	HDC hMemDC = BITMAP_MGR->Find_Img(m_szImgKey);

	GdiTransparentBlt(_hDC,
		int(m_tInfo.fX - m_tInfo.fCX),
		int(m_tInfo.fY - m_tInfo.fCY),
		int(m_tInfo.fCX), int(m_tInfo.fCY), hMemDC,
		0, 0,
		int(m_tInfo.fCX), int(m_tInfo.fCY), RGB(255, 0, 144));
}

const int & CArmor::Get_Defence()
{
	switch (m_eArmor)
	{
	case NORMAL:
		return 0;

	case LEATHER:
		return 1;

	case CHAINMAIL:
		return 1;

	case PLATEMAIL:
		return 2;

	case HEAVYPLATE:
		return 2;

	case GI:
		return 0;

	default:
		return 0;
	}
}

void CArmor::Render_Text(HDC _hDC)
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

	switch (m_eArmor)
	{
	case LEATHER:
		TextOut(_hDC, int(m_tInfo.fX + fScrollX), int(m_tInfo.fY + fScrollY - 60.f), L"leather", 8);
		break;

	case CHAINMAIL:
		TextOut(_hDC, int(m_tInfo.fX + fScrollX), int(m_tInfo.fY + fScrollY - 60.f), L"chainmail", 10);
		break;

	case PLATEMAIL:
		TextOut(_hDC, int(m_tInfo.fX + fScrollX), int(m_tInfo.fY + fScrollY - 60.f), L"platemail", 10);
		break;

	case HEAVYPLATE:
		TextOut(_hDC, int(m_tInfo.fX + fScrollX), int(m_tInfo.fY + fScrollY - 60.f), L"heavyplate", 11);
		break;

	case GI:
		TextOut(_hDC, int(m_tInfo.fX + fScrollX), int(m_tInfo.fY + fScrollY - 60.f), L"gi", 3);
		break;

	default:
		break;
	}
	(HFONT)DeleteObject(oldFont); // ���� �ִ���Ʈ ����
}
