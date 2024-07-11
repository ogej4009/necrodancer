#pragma once
#include <GameMath.h>
#include <list>
#include <map>
#include <set>

//������ ����� ����
//Ÿ��Ʋ, �÷��� ȭ�� ���
//����Ƽ Scene�� ����
//Actor �����ͷ� ��ĳ�����Ͽ� Actor���� ������
class MapEditor;
class Player;
class GameCol;
class GameActor; //���漱��
class GameRender;
class Monster;
class TileRender;
class Gold;
class GameState
{
	//�����ε�, ������Ʈ�� �ڵ� �Ҹ��� ���, 
	// �ٵ� �� State�� �Ҹ��ڿ��� ó���� ���Ѱ���?
	class STATICCONDES 
	{
	public:
		~STATICCONDES();
	};
	friend STATICCONDES;
	static STATICCONDES m_Inst;

	// Ÿ��Ʋ������ Ư���� �ϰ� ���� ���� �ִٸ�
	// GAMESTATE::CreateState(L"PlayerState");

private:
	//100������ ȭ���� �����־ �̰ɷ� ������
	static std::map<std::wstring, GameState* > m_AllState;

private:
	friend GameActor;
	static GameState* m_LoadingState; //ó�� �ε��ɶ��� ������Ʈ�� ����Ų
	static GameState* m_CurState;  //�Ϲݻ��¿��� ����Ű�� ������Ʈ�� ����Ų��.

private:
	static GameState* FindState(const wchar_t* _Name);

public:
	static void ChangeState(const wchar_t* _Name);

	template<typename T>
	static void CreateState(const wchar_t* _Name)
	{
		//State�� �ڽĵ��� ��ĳ�������� ����
		T* NewState = new T(); //state ��ü�� �������� ������
		//GameState* ptrNewState = (GameState*)NewState;
		// �ٽɱ���.
		GameState* ptrNewState = (GameState*)NewState;
		m_LoadingState = ptrNewState;
		ptrNewState->Loading(); //ù ���� �� �ѹ� �ε�, ���� curState�� ����
		m_LoadingState = nullptr;
		// ���� ��ü�� �������� �̿��ϴ� ��찡 ���ƿ�.
		m_AllState.insert(std::map<std::wstring, GameState*>::value_type(_Name, ptrNewState));

	}

	static void Progress();

protected:
	static CVector m_DefaultCam;

private:
	//������ġ ����� Pos��� ��������
	static CVector m_Cam;
	//���� �Լ� ������
	static CVector m_CurCamPos;
	//���� �Լ� ����
	static CVector m_NextCamPos;

protected:
	//�̴Ϻ��� �׾����� ����
	bool m_bMiniDie;

public:
	static float m_CamTimer;

	bool MiniDie()
	{
		return m_bMiniDie;
	}

	void MiniDie(bool _IsDie)
	{
		m_bMiniDie = _IsDie;
	}


public:
	static CVector CamPos()
	{
		return m_Cam;
	}

	void CamPos(CVector _Pos)
	{
		m_Cam = _Pos;
	}

private:
	static bool m_bIsDebug;
	static void ChangeDebug()
	{
		m_bIsDebug = !m_bIsDebug;
	}

private:
	// ��ĳ����
	// �̾ȿ��� ���Ӿ����� ����� ���� ��� ����ü�� �ʿ��ϴ�.
	// HGAMEACTOR�� ���� ��ҵ��� �θ�Ŭ���� �̰�
	// ���� �����ϱ� ���ؼ�
	// ���ϳ��� �ڷᱸ���� ���ؼ� ���� �����ϱ� ���ؼ�

	std::list<GameActor*>::iterator ProgressStart;
	std::list<GameActor*>::iterator ProgressEnd;
	std::list<GameActor*> m_AllActor;

private:
	Player* m_Player;
	MapEditor* m_MapEditor;
	Gold* m_Gold[20];
	int m_GoldIndex;


public:
	void CreateGold(CVector::MapIndex _MapIndex, int GoldCount);

	void SetPlayer(Player* _Player)
	{
		m_Player = _Player;
	}

	Player* GetPlayer()
	{
		return m_Player;
	}

	MapEditor* SetEditor(MapEditor* _Editor)
	{
		return m_MapEditor;
	}

public:
	TileRender* m_TileRender;
	TileRender* m_WaterTileRender;
	std::map<__int64, GameActor*> m_WallActorMap;
	std::map<__int64, GameActor*> m_BackWallActorMap;
	std::map<__int64, GameActor*> m_InterObjMap;
	std::map<__int64, GameActor*> m_MonsterMap;
	std::list<GameActor*> m_UIList;

public:
	bool m_StateChanged;

private:
	static void InActor(GameActor* _Actor);
	void MoveInterface();
	void MoveDebugRender();

public:
	virtual void Loading();
	// ���� ���͸� �����ʿ� ���µ� �� �����Ӹ���
	// �����ϰ� ���� �༮�� ��������  StateUpdate�� ������ �����.
	virtual void StateUpdate();
	virtual void SoundOn();
	virtual void SoundPause();

	void InitUI();
	void TurnOffUI();

	// ������ ������Ʈ�� �����ִ� �༮.
	// �ڽĵ��� �̰� ������ �ʿ䰡 ����. 
	// ��ĳ�������� �����ϴ°� �����̹Ƿ� �̰� ��Ʋ�� �� �κе� �����ؾ��Ѵ�.
	// ��ĳ�����̶� �θ�Ŭ������ ������ �ִ� ���
	void ActorUpdate();
	void ActorRender();
	void ActorDebugRender();
	void Collision();
	//void ColDebugRender();
	void Release();
	// void CreateActor(HGAMEACTOR* _Actor);
	GameActor* FindPlayerOrNull();

private:
	// delete GAMERENDERER* ������ �Ҽ� �ֳ�?
	// => ���͸� �����ϴ�.

	// ������ ���� ������ ���Ͱ� ������
	// ��뿡 ���� ������ STATE�� ������. 
	// �÷��̾ �����°� �ƴ϶� ���⿡�� �߰��ȴ�.
	//       -10,  background�� �������� ����.
	//       10, �÷��̾ 
	//       500000 UIIMAGE 
	std::map<int, std::list<GameRender*>> m_AllRenderLeague;

private:
	void AddRenderer(GameRender* _Render);

private:
	// PLAYER, PLAYERBULLET, MONSTER, MONSTERBULLET, MOUSE, UI
	// PLAYER = MONSTERBULLET
	// MONSTER = PLAYERBULLET, MOUSE
	// �ڱ��ڽŰ��� �浹�Ҽ� �־�� �Ѵ�.
	// ���������̳� �ڵ����� ������ ����ȴ�. 
	std::map<int, std::set<int>> m_LinkData;
	std::map<int, std::set<int>>::iterator LinkStart;
	std::map<int, std::set<int>>::iterator LinkEnd;
	std::set<int>::iterator OtherLinkStart;
	std::set<int>::iterator OtherLinkEnd;

	std::map<int, std::list<GameCol*>> m_AllColLeague;

	std::map<int, std::list<GameCol*>>::iterator DebugStart;
	std::map<int, std::list<GameCol*>>::iterator DebugEnd;

	std::map<int, std::list<GameCol*>>::iterator Src;
	std::map<int, std::list<GameCol*>>::iterator Other;

	std::list<GameCol*>::iterator DebugListStart;
	std::list<GameCol*>::iterator DebugListEnd;

	std::list<GameCol*>::iterator SrcStart;
	std::list<GameCol*>::iterator SrcEnd;
	std::list<GameCol*>::iterator OtherStart;
	std::list<GameCol*>::iterator OtherEnd;

	//���� �׷쳢�� �浹 üũ�� �ؾ��ϴ� ���
	void CheckLeagueCol(
		std::map<int, std::list<GameCol*>>::iterator& _Src) {};

	//�ٸ� �׷쳢�� �浹�� üũ�ϴ� ���
	void CheckLeagueCol(
		std::map<int, std::list<GameCol*>>::iterator& _Src
		, std::map<int, std::list<GameCol*>>::iterator& _Other);

	//public:
	//	void AddCol(GameCol* _Render);

public:
	void Link(int _Src, int _Dest);

protected:
	GameState();
	//���� �Ҹ��ڴ� �ʼ�
	virtual ~GameState();
};

