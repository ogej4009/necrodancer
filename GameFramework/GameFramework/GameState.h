#pragma once
#include <GameMath.h>
#include <list>
#include <map>
#include <set>

//게임의 장면을 랩핑
//타이틀, 플레이 화면 등등
//유니티 Scene과 동일
//Actor 포인터로 업캐스팅하여 Actor들을 관리함
class MapEditor;
class Player;
class GameCol;
class GameActor; //전방선언
class GameRender;
class Monster;
class TileRender;
class Gold;
class GameState
{
	//프리로딩, 스테이트의 자동 소멸을 담당, 
	// 근데 왜 State의 소멸자에서 처리를 안한거지?
	class STATICCONDES 
	{
	public:
		~STATICCONDES();
	};
	friend STATICCONDES;
	static STATICCONDES m_Inst;

	// 타이틀씬에서 특별히 하고 싶은 일이 있다면
	// GAMESTATE::CreateState(L"PlayerState");

private:
	//100종류의 화면이 들어와있어도 이걸로 관리함
	static std::map<std::wstring, GameState* > m_AllState;

private:
	friend GameActor;
	static GameState* m_LoadingState; //처음 로딩될때의 스테이트를 가리킨
	static GameState* m_CurState;  //일반상태에서 가리키는 스테이트를 가리킨다.

private:
	static GameState* FindState(const wchar_t* _Name);

public:
	static void ChangeState(const wchar_t* _Name);

	template<typename T>
	static void CreateState(const wchar_t* _Name)
	{
		//State의 자식들을 업캐스팅으로 관리
		T* NewState = new T(); //state 자체의 다형성을 지원함
		//GameState* ptrNewState = (GameState*)NewState;
		// 핵심구조.
		GameState* ptrNewState = (GameState*)NewState;
		m_LoadingState = ptrNewState;
		ptrNewState->Loading(); //첫 생성 후 한번 로딩, 이후 curState가 동작
		m_LoadingState = nullptr;
		// 관리 자체는 다형성을 이용하는 경우가 많아요.
		m_AllState.insert(std::map<std::wstring, GameState*>::value_type(_Name, ptrNewState));

	}

	static void Progress();

protected:
	static CVector m_DefaultCam;

private:
	//현재위치 저장용 Pos라고 생각하자
	static CVector m_Cam;
	//러프 함수 시작점
	static CVector m_CurCamPos;
	//러프 함수 끝점
	static CVector m_NextCamPos;

protected:
	//미니보스 죽었는지 여부
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
	// 업캐스팅
	// 이안에서 게임액터의 기능을 쓰면 헤더 그자체가 필요하다.
	// HGAMEACTOR는 게임 요소들의 부모클래스 이고
	// 쉽게 관리하기 위해서
	// 단하나의 자료구조를 통해서 쉽게 관리하기 위해서

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
	// 굳이 액터를 만들필요 없는데 매 프레임마다
	// 실행하고 싶은 녀석이 있을때는  StateUpdate에 내용을 적어라.
	virtual void StateUpdate();
	virtual void SoundOn();
	virtual void SoundPause();

	void InitUI();
	void TurnOffUI();

	// 액터의 업데이트를 돌려주는 녀석.
	// 자식들은 이걸 구현할 필요가 없다. 
	// 업캐스팅으로 관리하는게 전제이므로 이걸 비틀면 이 부분도 수정해야한다.
	// 업캐스팅이라 부모클래스만 가지고 있는 기능
	void ActorUpdate();
	void ActorRender();
	void ActorDebugRender();
	void Collision();
	//void ColDebugRender();
	void Release();
	// void CreateActor(HGAMEACTOR* _Actor);
	GameActor* FindPlayerOrNull();

private:
	// delete GAMERENDERER* 누구만 할수 있냐?
	// => 액터만 가능하다.

	// 삭제에 대한 권한은 액터가 가지고
	// 사용에 대한 권한은 STATE가 가진다. 
	// 플레이어만 가지는게 아니라 여기에도 추가된다.
	//       -10,  background의 랜더러가 들어간다.
	//       10, 플레이어가 
	//       500000 UIIMAGE 
	std::map<int, std::list<GameRender*>> m_AllRenderLeague;

private:
	void AddRenderer(GameRender* _Render);

private:
	// PLAYER, PLAYERBULLET, MONSTER, MONSTERBULLET, MOUSE, UI
	// PLAYER = MONSTERBULLET
	// MONSTER = PLAYERBULLET, MOUSE
	// 자기자신과도 충돌할수 있어야 한다.
	// 연관컨테이너 자동으로 순서가 변경된다. 
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

	//같은 그룹끼리 충돌 체크를 해야하는 경우
	void CheckLeagueCol(
		std::map<int, std::list<GameCol*>>::iterator& _Src) {};

	//다른 그룹끼리 충돌을 체크하는 경우
	void CheckLeagueCol(
		std::map<int, std::list<GameCol*>>::iterator& _Src
		, std::map<int, std::list<GameCol*>>::iterator& _Other);

	//public:
	//	void AddCol(GameCol* _Render);

public:
	void Link(int _Src, int _Dest);

protected:
	GameState();
	//가상 소멸자는 필수
	virtual ~GameState();
};

