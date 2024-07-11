#pragma once
#include <map>
#include <set>
#include <vector>
#include <GameImage.h>
#include "GameRender.h"


class TileRender : public GameRender
{

private:
	enum eTileDirNum
	{
		TILEDIR_MAX = 4,
	};

	static CVector::MapIndex m_DirArr[TILEDIR_MAX];

	//타일 이동가능 여부와 스프라이트 인덱스 저장용 클래스
	class TileData
	{
	public:
		bool m_Move;
		int m_SpriteIndex;
		int m_Len;
	};

	int m_Mask;

public:
	std::map<__int64, TileData*>::iterator findIter;
	std::map<__int64, TileData*>::iterator TileStart;
	std::map<__int64, TileData*>::iterator TileEnd;
	std::map<__int64, TileData*> m_AllTile;

	//모든 타일정보 리턴
	std::map<__int64, TileData*>& AllTile()
	{
		return m_AllTile;
	}

public:
	void AddTile(CVector _Pos, int _SpriteIndex = 0);
	void AddTile(int _X, int _Y, int _SpriteIndex = 0);
	TileData* FindTile(int _X, int _Y);
	void Render() override;

public:
	//인덱스 위치 알아내기
	CVector::MapIndex PosToIndex(const CVector _Pos);
	//인덱스 타일의 m_Len값 리턴해주기
	int Len(CVector::MapIndex _Index);

	// Astar
private:
	class PathNode
	{
	public:
		// 대각선 이동이 되냐 안되냐?
		CVector::MapIndex Index;
		float StartDis; // 시작점까지 오기위해서 걸린 거리
		float EndDis; // 최단거리
		float AllDis; // StartDis + EndDis 둘을 합친 거리
		PathNode* Parent; //부모노드, 처음 시작인 노드는 항상 비어있다.

	public:
		// 인덱스는 세팅되어있는 상황이라고 생각하자.
		// 목적지 좌표와 부모노드를 받아온다.
		void CalDisData(CVector::MapIndex _End, PathNode* _Parent)
		{
			// Index = _This;
			// StartDis 지금 계산하지 않는다.

			//처음 시작인 노드
			if (nullptr == _Parent)
			{
				StartDis = 0.0f;
			}
			//나머지 노드의 시작지점 계산
			else
			{
				StartDis = _Parent->StartDis;
				Parent = _Parent;
				CVector ParentPos = { Parent->Index.X, Parent->Index.Y };
				CVector Start = { Index.X, Index.Y };
				CVector Dis = Start - ParentPos;
				StartDis += Dis.Len();
			}

			//목표지점(end)까지 거리구하기
			{
				CVector End = { _End.X, _End.Y };
				CVector Start = { Index.X, Index.Y };
				CVector Dis = Start - End;
				EndDis = Dis.Len();
				AllDis = StartDis + EndDis;
			}
		}
	};

public:

	bool FindWaterTile(CVector::MapIndex _Index)
	{
		findIter = m_AllTile.find(_Index.Index);
		TileEnd = m_AllTile.end();

		if (findIter != TileEnd)
		{
			return true;
		}

		return false;
	}


	bool DeleteWaterTile(CVector::MapIndex _Index)
	{
		findIter = m_AllTile.find(_Index.Index);
		TileEnd = m_AllTile.end();

		if (findIter != TileEnd)
		{
			delete findIter->second;
			findIter->second = nullptr;
			m_AllTile.erase(findIter);
			return true;
		}

		return false;
	}

	// PathNode* 최적화겸
	// 여러분들에게 말해줄건

	// 메모리 단편화를 막기위해서가 보통 주 목적.
	// 가장더 근간이 되는 생각.
	// 메모리 풀 방식이 있는데. new char[10000]

	// 오브젝트 풀 방식이 최대의 효과를 얻으려면
	// 몬스터 x마리 이상 나오지 않을 거에요를 지키는 것.
	// 오브젝트 풀 방식과 new를 미리 여러번 한다.()
	// Pool 방식.
	// 혼합할수 있다.
	// 메모리 풀 방식은 안쓰고

	std::vector<PathNode*>::iterator CurNode;
	std::vector<PathNode*> PathNodePool;

	// closeList에서 탐색한 주변의 타일을 추가하는 리스트
	//            AllDis
	std::multimap<float, PathNode*> m_OpenList;
	// 아직 검사하지 않은 노드
	// 주위를 탐색할 가치가 있는 노드들
	// 그런왜 float(alldis)
	// AllDis가 가장 짧은 녀석이 가장 빠른 길로 통할 가능성이 높다고 생각.
	std::set<__int64> m_OpenIndexSet; // 아직 검사하지 않은 노드

	// 이게 쓸모가 있는것은 아니다.
	// 쓸모가 있기는 있다.
	// 한가지 더 쓸모있는것은
	// StartDis
	std::multimap<__int64, PathNode*> m_CloseList; // 아직 검사하지 않은 노드
	// std::set<__int64> m_CloseIndexSet; // 아직 검사하지 않은 노드

	//목표지점 위치
	CVector::MapIndex EndIndex;
	//목표지점
	PathNode* m_End;


	// F가 가장 작은 노드
	// 검사하지 않은 노드 중에서 

	std::list<CVector> PathFind(CVector _Start, CVector _End);


private:
	bool IsOpenList(CVector::MapIndex _Index);

	// 탐색을 위해서 빼는것.
	PathNode* OpenListPop();
	PathNode* OpenPathNodeCreate(CVector _Value, PathNode* _Parent);
	PathNode* OpenPathNodeCreate(CVector::MapIndex _Value, PathNode* _Parent);
	PathNode* PathNodeCreate(CVector _Value);
	PathNode* PathNodeCreate(CVector::MapIndex _Value);
	bool PathNodePathFind(PathNode* _Node);

	void ResetPool();

public:
	TileRender();
	~TileRender();
};

