#pragma once
#include "Scene.h"
#include "Player.h"

class CLobby :
	public CScene
{
private:
	CObj* m_pPlayer;
	FRAME m_tFrame;
	
private:
	void Move_Frame();

public:
	CLobby();
	virtual ~CLobby();

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hDC) override;
	virtual void Release() override;
};

