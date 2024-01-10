#pragma once
#include "Scene.h"

class CSceneMgr
{
private:
	static CSceneMgr* m_pInstance;
	CScene* m_pScene;

	SCENEID m_eCurScene;
	SCENEID m_ePreScene;

	bool m_bPause;

private:
	CSceneMgr();
	~CSceneMgr();

public:
	void Scene_Change(SCENEID _eSceneID);
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _hDC);
	void Release();

public:
	static CSceneMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CSceneMgr;

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
	const SCENEID& Get_Cur_Scene() { return m_eCurScene; }
	void Set_Pause_EveryOne() { m_bPause = true; }
	void Set_Restart_EveryOne() { m_bPause = false; }
};

#define SCENE_MGR	CSceneMgr::Get_Instance()