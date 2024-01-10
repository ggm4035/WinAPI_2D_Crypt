#include "stdafx.h"
#include "Obj.h"
#include "ScrollMgr.h"
#include "TileMgr.h"

void CObj::Update_Rect()
{
	m_tRect.left = long(m_tInfo.fX - m_tInfo.fCX * 0.5f);
	m_tRect.top = long(m_tInfo.fY - m_tInfo.fCY * 0.5f);
	m_tRect.right = long(m_tInfo.fX + m_tInfo.fCX * 0.5f);
	m_tRect.bottom = long(m_tInfo.fY + m_tInfo.fCY * 0.5f);
}

void CObj::Move_Frame(FRAME& _rFrame)
{
	if (_rFrame.dwTime + _rFrame.dwSpeed < GetTickCount())
	{
		++_rFrame.iFrameStart;

		if (_rFrame.iFrameStart > _rFrame.iFrameEnd)
			_rFrame.iFrameStart = 0;

		_rFrame.dwTime = GetTickCount();
	}
}

void CObj::Jumping()
{
	float fParabolaF = m_fPower * m_fTime - 9.8f * 0.5f * m_fTime * m_fTime;
	m_tInfo.fY -= fParabolaF;
	m_fTime += 0.3f;

	switch (m_eDir)
	{
	case CObj::LEFT:
		m_tInfo.fX -= 7.5f;

		if (m_tInfo.fX < m_tNextPos.fX)
		{
			m_tInfo.fX = m_tNextPos.fX;
			m_tInfo.fY = m_tNextPos.fY;
			m_fTime = 0.f;
			m_bJump = false;
		}
		break;

	case CObj::RIGHT:
		m_tInfo.fX += 7.5f;

		if (m_tInfo.fX > m_tNextPos.fX)
		{
			m_tInfo.fX = m_tNextPos.fX;
			m_tInfo.fY = m_tNextPos.fY;
			m_fTime = 0.f;
			m_bJump = false;
		}
		break;

	case CObj::UP:
		m_tInfo.fY -= 7.5f;

		if (m_tInfo.fY < m_tNextPos.fY)
		{
			m_tInfo.fX = m_tNextPos.fX;
			m_tInfo.fY = m_tNextPos.fY;
			m_fTime = 0.f;
			m_bJump = false;
		}
		break;

	case CObj::DOWN:
		m_tInfo.fY += 7.5f;

		if (m_tInfo.fY > m_tNextPos.fY)
		{
			m_tInfo.fX = m_tNextPos.fX;
			m_tInfo.fY = m_tNextPos.fY;
			m_fTime = 0.f;
			m_bJump = false;
		}
		break;

	default:

		if (m_tInfo.fY > m_tNextPos.fY)
		{
			m_tInfo.fX = m_tNextPos.fX;
			m_tInfo.fY = m_tNextPos.fY;
			m_fTime = 0.f;
			m_bJump = false;
		}
		break;
	}
}

CObj::CObj()
	:m_bDead(false), m_bJump(false), m_eDir(DIR_END), m_fPower(0.f), m_fTime(0.f), m_eID(OBJ_END), 
	m_bMoveTime(false), m_pTarget(nullptr), m_bWrongBeat(false)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tNextPos, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_tFrame, sizeof(FRAME));
	ZeroMemory(&m_tEffect, sizeof(FRAME));
	ZeroMemory(&m_tState, sizeof(STATE));
	ZeroMemory(m_szImgKey, sizeof(m_szImgKey));
}

CObj::~CObj()
{
}

void CObj::Set_Info_Pos(const float & _fX, const float & _fY)
{
	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
	m_tNextPos = m_tInfo;
}