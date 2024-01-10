#include "stdafx.h"
#include "Item.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"
#include "BitmapMgr.h"

void CItem::Show_Money(HDC _hDC)
{
	TCHAR buf[100] = {};
	//높이,너비,?,ori,weight,이탤릭,밑줄,?,--------,?,?,퀄리티?
	HFONT hFont = CreateFont(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("necrosans")); // 폰트변경
	HFONT oldFont = (HFONT)SelectObject(_hDC, hFont); // DC관련
	SetTextAlign(_hDC, TA_LEFT); // 텍스트 정렬
	SetTextColor(_hDC, RGB(255, 255, 255)); // 텍스트 컬러 설정
	SetBkMode(_hDC, TRANSPARENT); // 매개변수 DC의 글자배경을 투명하게 한다.

	float fScrollX = SCROLL_MGR->Get_ScrollX();
	float fScrollY = SCROLL_MGR->Get_ScrollY();

	if (m_tState.iMoney > 0)
	{
		_itow_s(m_tState.iMoney, buf, 10); // hp값 int -> wstring
		TextOut(_hDC, int(m_tInfo.fX + 10 + fScrollX), int(m_tInfo.fY + 10 + fScrollY), buf, lstrlen(buf));
	}

	(HFONT)DeleteObject(oldFont); // 원래 있던폰트 제거
}

void CItem::Show_Dia(HDC _hDC)
{
	HDC hDia = BITMAP_MGR->Find_Img(L"resource_diamond");

	TCHAR buf[100] = {};
	//높이,너비,?,ori,weight,이탤릭,밑줄,?,--------,?,?,퀄리티?
	HFONT hFont = CreateFont(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("necrosans")); // 폰트변경
	HFONT oldFont = (HFONT)SelectObject(_hDC, hFont); // DC관련
	SetTextAlign(_hDC, TA_LEFT); // 텍스트 정렬
	SetTextColor(_hDC, RGB(255, 255, 255)); // 텍스트 컬러 설정
	SetBkMode(_hDC, TRANSPARENT); // 매개변수 DC의 글자배경을 투명하게 한다.

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

	(HFONT)DeleteObject(oldFont); // 원래 있던폰트 제거
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
