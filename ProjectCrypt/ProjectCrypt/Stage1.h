#pragma once
#include "Scene.h"
#include "Player.h"

class CStage1 :
	public CScene
{
private:
	CObj* m_pPlayer;
	bool m_bOpen;

public:
	CStage1();
	virtual ~CStage1();

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hDC) override;
	virtual void Release() override;
};

