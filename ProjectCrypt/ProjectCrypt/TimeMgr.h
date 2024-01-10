#pragma once
#include <chrono>

class CTimeMgr	// 약간 메트로놈 같은 기능 BPM을 입력하면 BPM만큼 true를 반환한다.
{
private:
	static CTimeMgr* m_pInstance;
	chrono::microseconds m_Tick;
	chrono::microseconds m_Delay;
	chrono::system_clock::time_point m_StartTime;
	chrono::system_clock::time_point m_EndTime;

	chrono::system_clock::time_point m_StartRap;
	chrono::system_clock::time_point m_endRap;

	float m_fDelay;

private:
	CTimeMgr();
	~CTimeMgr();

public:
	void Initialize();
	bool Tick();
	void Set_BPM(const int& _iBPM);
	DWORD Get_Milli_Sec() { return DWORD(m_Tick.count() % 1000); }
	void Rap_Start() { m_StartRap = chrono::system_clock::now(); }
	void Rap_end() { m_endRap = chrono::system_clock::now(); }
	long long Get_Cur_Rap() { return (chrono::system_clock::now() - m_StartRap).count() / 100000; }
	long long Get_End_Rap() { return (m_endRap - m_StartRap).count() / 100000; }

public:
	static CTimeMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CTimeMgr;

		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
	void Set_Delay(const float& _fDelay) { m_fDelay = _fDelay; }
	void Reset()
	{
		m_StartTime = chrono::system_clock::now();
		m_EndTime = chrono::system_clock::now();
	}
};

#define TIME_MGR	CTimeMgr::Get_Instance()