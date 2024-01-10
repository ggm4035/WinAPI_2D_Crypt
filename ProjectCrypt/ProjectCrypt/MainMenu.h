#pragma once
#include "Scene.h"
class CMainMenu :
	public CScene
{
private:
	DWORD m_dwTime;
	bool m_bBlink;

public:
	CMainMenu();
	virtual ~CMainMenu();

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hDC) override;
	virtual void Release() override;
};

