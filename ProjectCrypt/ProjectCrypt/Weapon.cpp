#include "stdafx.h"
#include "Weapon.h"
#include "BitmapMgr.h"
#include "ScrollMgr.h"
#include "TileMgr.h"

void CWeapon::Move_Frame(FRAME& _rFrame, FRAME_OPTION _eOption)
{
	switch (_eOption)
	{
	case CWeapon::LTOR:
		if (_rFrame.dwTime + _rFrame.dwSpeed < GetTickCount())
		{
			++_rFrame.iFrameStart;

			if (_rFrame.iFrameStart > _rFrame.iFrameEnd)
				_rFrame.iFrameStart = 0;

			_rFrame.dwTime = GetTickCount();
		}
		break;

	case CWeapon::RTOL:
		if (_rFrame.dwTime + _rFrame.dwSpeed < GetTickCount())
		{
			--_rFrame.iFrameStart;

			if (_rFrame.iFrameStart < 0)
				_rFrame.iFrameStart = _rFrame.iFrameEnd;

			_rFrame.dwTime = GetTickCount();
		}
		break;

	case CWeapon::TTOB:
		if (_rFrame.dwTime + _rFrame.dwSpeed < GetTickCount())
		{
			++_rFrame.iMotion;

			if (_rFrame.iMotion > _rFrame.iMotionEnd)
				_rFrame.iFrameStart = 0;

			_rFrame.dwTime = GetTickCount();
		}
		break;

	case CWeapon::BTOT:
		if (_rFrame.dwTime + _rFrame.dwSpeed < GetTickCount())
		{
			--_rFrame.iMotion;

			if (_rFrame.iMotion < 0)
				_rFrame.iFrameStart = _rFrame.iMotionEnd;

			_rFrame.dwTime = GetTickCount();
		}
		break;

	case CWeapon::OP_END:
		break;

	default:
		break;
	}
}

CWeapon::CWeapon()
	:m_bEffect(false), m_eOption(OP_END)
{
	ZeroMemory(&m_tTarget, sizeof(INFO));
}

CWeapon::~CWeapon()
{
}

bool CWeapon::Collision()
{
	return false;
}

void CWeapon::Render_UI(HDC _hDC)
{
	HDC hMemDC = BITMAP_MGR->Find_Img(m_szImgKey);

	GdiTransparentBlt(_hDC,
		int(m_tInfo.fX - m_tInfo.fCX),
		int(m_tInfo.fY - m_tInfo.fCY),
		int(m_tInfo.fCX), int(m_tInfo.fCY), hMemDC,
		0, 0,
		int(m_tInfo.fCX), int(m_tInfo.fCY), RGB(255, 0, 144));
}
