#pragma once
#include "Scene.h"
#include "Player.h"

class CBossStage :
	public CScene
{
private:
	CObj* m_pPlayer;

	int m_iBeatCount;
	int m_iMoveLen;
	float m_fVolume;

	bool m_bClose;
	bool m_bEnd;
	bool m_bStart;
	bool m_bTurn;
	bool m_bEnding;

	DWORD m_dwTime;

private:
	void Render_CutScene(HDC _hDC);

public:
	CBossStage();
	virtual ~CBossStage();

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hDC) override;
	virtual void Release() override;
};

