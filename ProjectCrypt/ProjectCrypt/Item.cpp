#include "stdafx.h"
#include "Item.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"
#include "BitmapMgr.h"

void CItem::Show_Money(HDC _hDC)
{
	TCHAR buf[100] = {};
	//����,�ʺ�,?,ori,weight,���Ÿ�,����,?,--------,?,?,����Ƽ?
	HFONT hFont = CreateFont(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("necrosans")); // ��Ʈ����
	HFONT oldFont = (HFONT)SelectObject(_hDC, hFont); // DC����
	SetTextAlign(_hDC, TA_LEFT); // �ؽ�Ʈ ����
	SetTextColor(_hDC, RGB(255, 255, 255)); // �ؽ�Ʈ �÷� ����
	SetBkMode(_hDC, TRANSPARENT); // �Ű����� DC�� ���ڹ���� �����ϰ� �Ѵ�.

	float fScrollX = SCROLL_MGR->Get_ScrollX();
	float fScrollY = SCROLL_MGR->Get_ScrollY();

	if (m_tState.iMoney > 0)
	{
		_itow_s(m_tState.iMoney, buf, 10); // hp�� int -> wstring
		TextOut(_hDC, int(m_tInfo.fX + 10 + fScrollX), int(m_tInfo.fY + 10 + fScrollY), buf, lstrlen(buf));
	}

	(HFONT)DeleteObject(oldFont); // ���� �ִ���Ʈ ����
}

void CItem::Show_Dia(HDC _hDC)
{
	HDC hDia = BITMAP_MGR->Find_Img(L"resource_diamond");

	TCHAR buf[100] = {};
	//����,�ʺ�,?,ori,weight,���Ÿ�,����,?,--------,?,?,����Ƽ?
	HFONT hFont = CreateFont(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("necrosans")); // ��Ʈ����
	HFONT oldFont = (HFONT)SelectObject(_hDC, hFont); // DC����
	SetTextAlign(_hDC, TA_LEFT); // �ؽ�Ʈ ����
	SetTextColor(_hDC, RGB(255, 255, 255)); // �ؽ�Ʈ �÷� ����
	SetBkMode(_hDC, TRANSPARENT); // �Ű����� DC�� ���ڹ���� �����ϰ� �Ѵ�.

	float fScrollX = SCROLL_MGR->Get_ScrollX();
	float fScrollY = SCROLL_MGR->Get_ScrollY();

	if (m_tState.iDiamond > 0)
	{
		swprintf_s(buf, L"x %d", m_tState.iDiamond);

		GdiTransparentBlt(_hDC,
			int(m_tInfo.fX - 14 + fScrollX),
			int(m_tInfo.fY + 4 + fScrollY),
			24, 24, hDia,
			0, 0,
			TILECX, TILECY, RGB(255, 0, 144));
		TextOut(_hDC, int(m_tInfo.fX + 10 + fScrollX), int(m_tInfo.fY + 10 + fScrollY), buf, lstrlen(buf));
	}

	(HFONT)DeleteObject(oldFont); // ���� �ִ���Ʈ ����
}

void CItem::Set_Distance()
{
	float fX = m_tInfo.fX - OBJ_MGR->Get_Player()->Get_Info().fX;
	float fY = m_tInfo.fY - OBJ_MGR->Get_Player()->Get_Info().fY;
	m_fDistance = sqrtf(fX * fX + fY * fY);
}

CItem::CItem()
	:m_eItemID(ITEM_END), m_fDistance(0.f)
{
}

CItem::~CItem()
{
}
