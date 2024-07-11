#pragma once
#include "ActorSub.h"
#include <set>


class GameState;
class GameCol : public ActorSub
{
public:
	enum class COLTYPE 
	{
		RECT2D,
		CIRCLE2D,
		POINT2D,
		COLTYPE_MAX,
	};


public:
	//�浹 ������ �ޱ����� �Լ�������
	//COLTYPE�� enum���� �����ؼ� ���� ����ġ������ �Ǻ��ϴ� ������ �ٿ���
	static bool(__stdcall *ColCheckFunc[static_cast<int>(COLTYPE::COLTYPE_MAX)][static_cast<int>(COLTYPE::COLTYPE_MAX)])(const GameRect& _Left, const GameRect& _Right);

public:
	friend GameState;
	friend GameActor;

private:
	int m_League;
	COLTYPE m_Type;
	std::set<GameCol*> m_ColSet;

private:
	void Init();


private:
	void DebugRender();
	//�Լ�������
	void (GameCol::*DebugRenderFunc)();
	void RectRender();
	void CirCleRender();
	void PointRender();

private:
	void ColCheck(GameCol* _Other);

public:
	GameCol();
	~GameCol();
};

