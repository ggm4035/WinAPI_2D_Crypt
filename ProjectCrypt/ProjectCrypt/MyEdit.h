#pragma once
#include "Scene.h"
#include "Define.h"
#include "Player.h"

class CMyEdit :
	public CScene
{
private:
	CObj* m_pPlayer;
	int m_iOption;
	int m_iDrawX;
	int m_iDrawY;

	bool m_bObjectMode;
	bool m_bEraseMode;

	TCHAR m_szModeObj[16];
	TCHAR m_szModeErase[16];
	TCHAR m_szCurBlock[16];

public:
	CMyEdit();
	virtual ~CMyEdit();

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hDC) override;
	virtual void Release() override;

public:
	void Key_Input();
};

