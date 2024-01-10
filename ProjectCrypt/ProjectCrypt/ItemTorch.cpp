#include "stdafx.h"
#include "ItemTorch.h"
#include "ScrollMgr.h"
#include "BitmapMgr.h"
#include "TileMgr.h"

CItemTorch::CItemTorch()
{
}

CItemTorch::~CItemTorch()
{
	Release();
}

bool CItemTorch::Collision()
{
	return false;
}

void CItemTorch::Render_Text(HDC _hDC)
{
	if (m_fDistance > sqrtf(TILECX * TILECX + TILECY * TILECY))
		return;

	float fScrollX = SCROLL_MGR->Get_ScrollX();
	float fScrollY = SCROLL_MGR->Get_ScrollY();

	HFONT hFont = CreateFont(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("necrosans")); // 폰트변경
	HFONT oldFont = (HFONT)SelectObject(_hDC, hFont); // DC관련
	SetTextAlign(_hDC, TA_CENTER); // 텍스트 정렬
	SetTextColor(_hDC, RGB(255, 255, 255)); // 텍스트 컬러 설정
	SetBkMode(_hDC, TRANSPARENT); // 매개변수 DC의 글자배경을 투명하게 한다.


	TextOut(_hDC, int(m_tInfo.fX + fScrollX), int(m_tInfo.fY + fScrollY - 60.f), L"torch", 6);

	(HFONT)DeleteObject(oldFont); // 원래 있던폰트 제거
}

void CItemTorch::Initialize()
{
	m_eID = OBJ_ITEM;
	m_eItemID = ITEM_TORCH;
	m_tInfo.fCX = 48.f;
	m_tInfo.fCY = 48.f;
	lstrcpy(m_szImgKey, L"torch_2");
}

int CItemTorch::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CItemTorch::Late_Update()
{
	__super::Set_Distance();
}

void CItemTorch::Render(HDC _hDC)
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

void CItemTorch::Release()
{
}

void CItemTorch::Render_UI(HDC _hDC)
{
	HDC hMemDC = BITMAP_MGR->Find_Img(m_szImgKey);

	GdiTransparentBlt(_hDC,
		int(m_tInfo.fX - m_tInfo.fCX),
		int(m_tInfo.fY - m_tInfo.fCY),
		int(m_tInfo.fCX), int(m_tInfo.fCY), hMemDC,
		0, 0,
		int(m_tInfo.fCX), int(m_tInfo.fCY), RGB(255, 0, 144));
}
