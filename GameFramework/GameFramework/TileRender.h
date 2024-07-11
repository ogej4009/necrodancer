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

	//Ÿ�� �̵����� ���ο� ��������Ʈ �ε��� ����� Ŭ����
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

	//��� Ÿ������ ����
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
	//�ε��� ��ġ �˾Ƴ���
	CVector::MapIndex PosToIndex(const CVector _Pos);
	//�ε��� Ÿ���� m_Len�� �������ֱ�
	int Len(CVector::MapIndex _Index);

	// Astar
private:
	class PathNode
	{
	public:
		// �밢�� �̵��� �ǳ� �ȵǳ�?
		CVector::MapIndex Index;
		float StartDis; // ���������� �������ؼ� �ɸ� �Ÿ�
		float EndDis; // �ִܰŸ�
		float AllDis; // StartDis + EndDis ���� ��ģ �Ÿ�
		PathNode* Parent; //�θ���, ó�� ������ ���� �׻� ����ִ�.

	public:
		// �ε����� ���õǾ��ִ� ��Ȳ�̶�� ��������.
		// ������ ��ǥ�� �θ��带 �޾ƿ´�.
		void CalDisData(CVector::MapIndex _End, PathNode* _Parent)
		{
			// Index = _This;
			// StartDis ���� ������� �ʴ´�.

			//ó�� ������ ���
			if (nullptr == _Parent)
			{
				StartDis = 0.0f;
			}
			//������ ����� �������� ���
			else
			{
				StartDis = _Parent->StartDis;
				Parent = _Parent;
				CVector ParentPos = { Parent->Index.X, Parent->Index.Y };
				CVector Start = { Index.X, Index.Y };
				CVector Dis = Start - ParentPos;
				StartDis += Dis.Len();
			}

			//��ǥ����(end)���� �Ÿ����ϱ�
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

	// PathNode* ����ȭ��
	// �����е鿡�� �����ٰ�

	// �޸� ����ȭ�� �������ؼ��� ���� �� ����.
	// ����� �ٰ��� �Ǵ� ����.
	// �޸� Ǯ ����� �ִµ�. new char[10000]

	// ������Ʈ Ǯ ����� �ִ��� ȿ���� ��������
	// ���� x���� �̻� ������ ���� �ſ��並 ��Ű�� ��.
	// ������Ʈ Ǯ ��İ� new�� �̸� ������ �Ѵ�.()
	// Pool ���.
	// ȥ���Ҽ� �ִ�.
	// �޸� Ǯ ����� �Ⱦ���

	std::vector<PathNode*>::iterator CurNode;
	std::vector<PathNode*> PathNodePool;

	// closeList���� Ž���� �ֺ��� Ÿ���� �߰��ϴ� ����Ʈ
	//            AllDis
	std::multimap<float, PathNode*> m_OpenList;
	// ���� �˻����� ���� ���
	// ������ Ž���� ��ġ�� �ִ� ����
	// �׷��� float(alldis)
	// AllDis�� ���� ª�� �༮�� ���� ���� ��� ���� ���ɼ��� ���ٰ� ����.
	std::set<__int64> m_OpenIndexSet; // ���� �˻����� ���� ���

	// �̰� ���� �ִ°��� �ƴϴ�.
	// ���� �ֱ�� �ִ�.
	// �Ѱ��� �� �����ִ°���
	// StartDis
	std::multimap<__int64, PathNode*> m_CloseList; // ���� �˻����� ���� ���
	// std::set<__int64> m_CloseIndexSet; // ���� �˻����� ���� ���

	//��ǥ���� ��ġ
	CVector::MapIndex EndIndex;
	//��ǥ����
	PathNode* m_End;


	// F�� ���� ���� ���
	// �˻����� ���� ��� �߿��� 

	std::list<CVector> PathFind(CVector _Start, CVector _End);


private:
	bool IsOpenList(CVector::MapIndex _Index);

	// Ž���� ���ؼ� ���°�.
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

