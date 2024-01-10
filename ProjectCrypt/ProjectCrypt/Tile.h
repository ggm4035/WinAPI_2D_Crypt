#pragma once
#include "Define.h"
#include "Obj.h"

class CTile
{
private:
	INFO m_tInfo;
	RECT m_tRect;
	FRAME m_tFrame;

	int m_iDrawX;
	int m_iDrawY;

	TCHAR m_szImgKey[32];

private:
	void Update_Rect();
	void Move_Frame();

public:
	CTile();
	~CTile();

public:
	void Initialize();
	int Update();
	void Late_Update();
	void Render(HDC _hDC);
	void Release();

public:
	const INFO& Get_Info() const { return m_tInfo; }
	const RECT& Get_Rect() const { return m_tRect; }
	const int& Get_DrawX() const { return m_iDrawX; }
	const int& Get_DrawY() const { return m_iDrawY; }
	const TCHAR* Get_ImgKey() const { return m_szImgKey; }

	void Set_Info_Pos(const float& _fX, const float& _fY)
	{
		m_tInfo.fX = _fX;
		m_tInfo.fY = _fY;
	}
	void Set_DrawID(const int& _iX, const int& _iY)
	{
		m_iDrawX = _iX;
		m_iDrawY = _iY;
	}
	void Set_ImgKey(TCHAR* _pKey) { lstrcpy(m_szImgKey, _pKey); }
};

