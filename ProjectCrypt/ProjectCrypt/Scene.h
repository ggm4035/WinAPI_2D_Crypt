#pragma once
#include "Define.h"

class CScene
{
protected:
	bool m_bTick;

public:
	CScene();
	virtual ~CScene();

public:
	virtual void Initialize() PURE;
	virtual void Update() PURE;
	virtual void Late_Update() PURE;
	virtual void Render(HDC _hDC) PURE;
	virtual void Release() PURE;
};

