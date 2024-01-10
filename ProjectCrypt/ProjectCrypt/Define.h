#pragma once

#define		WINCX	800
#define		WINCY	600

#define		VK_MAX	0xff

#define		TILECX		48
#define		TILECY		48
#define		TILEX		29
#define		TILEY		29

#define		PURE		= 0

#define		PLAYER_DEAD	-1
#define		OBJ_DEAD	0
#define		OBJ_NOEVENT	1

#define		BEAT_DEAD		2
#define		BEAT_NOEVENT	3
#define		BEAT_HITTING	4

#define		UI_DEAD			5
#define		UI_NOEVENT		6

#define		PI	3.141592f
#define		RADIAN	PI / 180.f

#define		SAFE_DELETE(TYPE)	[&](TYPE& T){if(T){delete T; T = nullptr;}}

extern HWND g_hWnd;
extern int g_iSpeed;

typedef struct tag_Status
{
	int iMaxHP;
	int iHP;
	int iAttack;
	int iDefence;
	int iMoney;
	int iDiamond;
}STATE;

typedef struct tag_Info
{
	float fX;
	float fY;
	float fCX;
	float fCY;

	bool operator==(const tag_Info& _rhs)
	{
		return (fX == _rhs.fX) && (fY == _rhs.fY);
	}
}INFO;

typedef struct tagFrame
{
	int		iFrameStart;
	int		iFrameEnd;
	int		iMotion;
	int		iMotionEnd;

	DWORD	dwSpeed;
	DWORD	dwTime;

}FRAME;

typedef struct tag_Imagine
{
	INFO tInfo;
	FRAME tFrame;
	TCHAR* pHead;
	TCHAR* pBody;
}IMAGINE;

class cTag_Finder
{
private:
	const TCHAR* m_pKey;

public:
	cTag_Finder(const TCHAR* _pKey)
		:m_pKey(_pKey) { }

	template<typename T>
	bool operator()(T& MyPair)
	{
		if (!lstrcmp(MyPair.first, m_pKey))
			return true;

		else
			return false;
	}
};

class CDeleteMap
{
public:
	template<typename T>
	void operator()(T& Temp)
	{
		if (Temp.second)
		{
			delete Temp.second;
			Temp.second = nullptr;
		}
	}
};

enum Armor { NORMAL, LEATHER, CHAINMAIL, PLATEMAIL, HEAVYPLATE, GI };
enum OBJID {OBJ_PLAYER, OBJ_WALL, OBJ_TRAP, OBJ_TORCH, OBJ_CHEST, OBJ_NPC, OBJ_ITEM, OBJ_MONEY, OBJ_MONSTER, OBJ_END};
enum ITEMID { ITEM_SHOVEL, ITEM_WEAPON, ITEM_ARMOR, ITEM_TORCH, ITEM_KEY, ITEM_BOMB, ITEM_FOOD, ITEM_SPELL, ITEM_HEART, ITEM_END };
enum WALLID {WALL_BREAK, WALL_BOOM_BREAK, WALL_UNBREAK, WALL_DOOR, WALL_END};
enum MONSTERID { GREEN_SLIME, ICE_SLIME, SKELETON, BAT, ZOMBIE, GREEN_DRAGON, BOSS, MONSTER_END };
enum SCENEID { SC_LOGO, SC_MENU, SC_LOBBY, SC_EDIT, SC_STAGE1, SC_BOSS_STAGE, SC_END };
enum CHANNELID { SOUND_MAINMENU, SOUND_LOBBY, SOUND_STAGE_ONE, SOUND_BOSS_STAGE,
				SOUND_PLAYER_VOICE, SOUND_MONSTER_VOICE, 
				SOUND_BGM,
				SOUND_EFFECT, SOUND_MONSTER,
				MAXCHANNEL };