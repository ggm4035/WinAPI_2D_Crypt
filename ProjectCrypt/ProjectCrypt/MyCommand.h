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

// �� �ൿ���� ���� Ŭ������ �����.
class CJumpCommand : public CMyCommand
{
public:
	virtual void execute(CObj* _pObj) override
	{
		// CObj�� ���� ���� ��� ����
	}
};

class CAttackCommand : public CMyCommand
{
public:
	virtual void execute(CObj* _pObj) override
	{
		// CObj�� ���� ���� ��� ����
	}

};

// ��� �ϳ��� �θ�Ŭ���� �������̽��� ��ӹ޾Ƽ� �پ��� ����� �����ϴ� ��� => command(���) ����