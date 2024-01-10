#pragma once
#include "Define.h"

class CMainGame
{
private:
	HDC m_hDC;
	int m_iFPS;
	TCHAR m_szFPS[32];
	DWORD m_dwTime;

public:
	CMainGame();
	~CMainGame();

public:
	void Initialize();
	void Update();
	void Late_UPdate();
	void Render();
	void Release();
};

