#include "stdafx.h"
#include "TimeMgr.h"
#include "BeatMgr.h"

CTimeMgr* CTimeMgr::m_pInstance = nullptr;

CTimeMgr::CTimeMgr()
	:m_Tick(0), m_StartTime(chrono::system_clock::now()), m_EndTime(chrono::system_clock::now()), m_Delay(0), 
	m_StartRap(chrono::system_clock::now()), m_endRap(chrono::system_clock::now()), m_fDelay(0.f)
{
}

CTimeMgr::~CTimeMgr()
{
}

void CTimeMgr::Initialize()
{
	Set_BPM(115);
}

bool CTimeMgr::Tick()
{
	m_EndTime = chrono::system_clock::now();

	if ((m_EndTime - m_StartTime) > m_Tick - m_Delay)
	{
		m_Delay = chrono::duration_cast<chrono::microseconds>(m_EndTime - m_StartTime - m_Tick);
		BEAT_MGR->Add_Delay(m_fDelay);

		if (m_Delay.count() < 0)
			m_Delay = chrono::microseconds::zero();

		else if(m_Delay.count() > 5000000)
			m_Delay = chrono::microseconds::zero();

		m_StartTime = chrono::system_clock::now();
		return true;
	}
	return false;
}

void CTimeMgr::Set_BPM(const int & _iBPM)
{
	double dSec = 60.0 / double(_iBPM);

	m_Tick = chrono::duration_cast<chrono::microseconds>(chrono::duration<double>(dSec));
}
