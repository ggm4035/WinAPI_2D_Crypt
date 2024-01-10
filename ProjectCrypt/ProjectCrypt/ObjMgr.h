#pragma once
#include "Obj.h"
#include "Define.h"

class CObjMgr
{
private:
	static CObjMgr* m_pInstance;
	list<CObj*> m_ObjList[OBJ_END];
	list<CObj*> m_RenderList;
	TCHAR* m_pImgKey;
	bool m_bQuake;

	OBJID m_eObjID;
	ITEMID m_eItemID;
	MONSTERID m_eMonID;
	WALLID m_eWallID;
	int m_iMoney;
	int m_iDia;

private:
	CObjMgr();
	~CObjMgr();

private:
	void Read_Resource();

public:
	void Make_Player();
	void AddObject(OBJID _eID, CObj* _pObj);
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _hDC);
	void Release();
	void Release_Player();

public:
	static CObjMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CObjMgr;

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

public:
	list<CObj*>* Get_ObjList(OBJID _eID) { return &m_ObjList[_eID]; }
	void Set_ImgKey(TCHAR* _pImgKey) { m_pImgKey = _pImgKey; }
	void Picking(const POINT& pt);
	CObj* Get_Player() { return m_ObjList[OBJ_PLAYER].front(); }
	void Set_Hitting_Obj(OBJID _eID);
	void Set_Stop_Monster(bool _bool);

	CObj* Find_Obj(const OBJID& _eID, const float _fX, const float _fY);
	CObj* Find_Obj(const float _fX, const float _fY);
	CObj* Find_Monster(MONSTERID _eMonID);
	void Delete_Object(const float& _fX, const float& _fY);
	void Erase_Object(const CObj* _pObj);

	bool IsQuake() { return m_bQuake; }
	void QuakeOn() { m_bQuake = true; }
	void QuakeOff() { m_bQuake = false; }

	void Set_ObjID(const OBJID& _eID) { m_eObjID = _eID; }
	void Set_ItemID(const ITEMID& _eID) { m_eItemID = _eID; }
	void Set_MonsterID(const MONSTERID& _eID) { m_eMonID = _eID; }
	void Set_WallID(const WALLID& _eID) { m_eWallID = _eID; }
	void Set_Obj_Money(const int& _iValue) { m_iMoney = _iValue; }
	void Set_Obj_Dia(const int& _iValue) { m_iDia = _iValue; }

	void Save_Object(const TCHAR* _pFilePath);
	void Load_Object(const TCHAR* _pFilePath);
};

#define OBJ_MGR	CObjMgr::Get_Instance()