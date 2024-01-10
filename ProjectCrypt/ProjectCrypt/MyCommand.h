#pragma once
#include "Define.h"
#include "Obj.h"

class CMyCommand
{
public:
	CMyCommand() {}
	virtual ~CMyCommand() {}
	virtual void execute(CObj* _pObj) PURE;
};

// 각 행동별로 하위 클래스를 만든다.
class CJumpCommand : public CMyCommand
{
public:
	virtual void execute(CObj* _pObj) override
	{
		// CObj의 점프 관련 기능 구현
	}
};

class CAttackCommand : public CMyCommand
{
public:
	virtual void execute(CObj* _pObj) override
	{
		// CObj의 공격 관련 기능 구현
	}

};

// 등등 하나의 부모클래스 인터페이스를 상속받아서 다양한 기능을 구현하는 방법 => command(명령) 패턴