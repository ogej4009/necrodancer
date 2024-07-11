//#include "stdafx.h"
#include "TileRender.h"
#include "GameActor.h"
#include <GameTexMgr.h>
#include "GameState.h"
#include <GameMath.h>
#include "Player.h"
//
//std::map<__int64, TileRender::TileData*>::iterator TileRender::TileStart;
//std::map<__int64, TileRender::TileData*>::iterator TileRender::TileEnd;
//std::map<__int64, TileRender::TileData*> TileRender::m_AllTile;


TileRender::TileRender() : m_Mask(0)
{
	//PathNodePool �ʱ�ȭ
	//300���� ���� ��������.
	for (size_t i = 0; i < 300; i++)
	{
		// POP FRONT
		PathNodePool.push_back(new PathNode());
	}
	// PathNodePool.pop_front();
	CurNode = PathNodePool.begin();

}


TileRender::~TileRender()
{
	std::map<__int64, TileRender::TileData*>::iterator Start = m_AllTile.begin();
	std::map<__int64, TileRender::TileData*>::iterator End = m_AllTile.end();

	for (; Start != End; ++Start)
	{
		if (nullptr != Start->second)
		{
			delete Start->second;
			Start->second = nullptr;
		}
	}

	{
		std::vector<PathNode*>::iterator Start = PathNodePool.begin();
		std::vector<PathNode*>::iterator End = PathNodePool.end();

		for (; Start != End; ++Start)
		{
			delete (*Start);
		}
	}

}


CVector::MapIndex TileRender::m_DirArr[TILEDIR_MAX]
{
	{1, 0}, // R
	{0, -1}, // T
	{-1, 0}, // L
	{0, 1}, // B
};


TileRender::TileData* TileRender::FindTile(int _X, int _Y)
{
	CVector::MapIndex Index = { _X, _Y };

	std::map<__int64, TileData*>::iterator Iter = m_AllTile.find(Index.Index);

	if (Iter == m_AllTile.end())
	{
		return nullptr;
	}
	return Iter->second;
}

void TileRender::AddTile(int _X, int _Y, int _SpriteIndex /*= 0*/)
{
	TileData* pFindTile = FindTile(_X, _Y);

	if (nullptr == pFindTile)
	{
		CVector::MapIndex IndexKey = { _X, _Y };
		pFindTile = new TileData();
		m_AllTile.insert(std::map<__int64, TileRender::TileData*>::value_type(IndexKey.Index, pFindTile));
	}

	if (m_pParent->ActorType() != EActorTypeIndex::WaterTile)
	{
		m_Mask = 1;
		m_Mask &= _X + _Y;
		if (1 == m_Mask)
		{
			_SpriteIndex += 1;
		}
	
	}
	pFindTile->m_SpriteIndex = _SpriteIndex;
	
}

void TileRender::AddTile(CVector _Pos, int _SpriteIndex)
{
	int X = 0;
	int Y = 0;

	X = static_cast<int>(std::round((_Pos.X + GameState::CamPos().X) / GameActor::TileSize()));
	Y = static_cast<int>(std::round((_Pos.Y + GameState::CamPos().Y) / GameActor::TileSize()));

	AddTile(X, Y, _SpriteIndex);
}

int TileRender::Len(CVector::MapIndex _Index)
{
	findIter = m_AllTile.find(_Index.Index);
	if (findIter == m_AllTile.end())
	{
		ASSERTMSG;
	}
	return findIter->second->m_Len;
}

void TileRender::Render()
{

	TileStart = m_AllTile.begin();
	TileEnd = m_AllTile.end();

	GameRect Rc;
	Rc.Size = { GameActor::TileSize(),GameActor::TileSize() };

	if (m_ShadowImg != nullptr)
	{
		m_ShadowRect = m_ShadowImg->GetRect(m_ShadowImgIndex);
	}


	for (; TileStart != TileEnd; ++TileStart)
	{
		m_Rect = m_Img->GetRect(TileStart->second->m_SpriteIndex);

		CVector::MapIndex TI;
		TI.Index = TileStart->first;
		m_SubIndex.X = TI.X;
		m_SubIndex.Y = TI.Y;

		Rc.Pos.X = TI.X * GameActor::TileSize() - GameState::CamPos().X;
		Rc.Pos.Y = TI.Y * GameActor::TileSize() - GameState::CamPos().Y;

		// �÷��̾���� �Ÿ��� ���� ���� �󵵸� �ٲ��.


		TransparentBlt(GameTexMgr::BackDC()
			, Rc.IL()
			, Rc.IT()
			, Rc.Size.IX()
			, Rc.Size.IY()
			, m_Img->HDC()
			, m_Rect.Pos.IX() // �̹����� x��������
			, m_Rect.Pos.IY() // �̹����� y��������
			, m_Rect.Size.IX()  // ��������� x �ȼ�
			, m_Rect.Size.IY()
			, m_TransColor);


		if (nullptr != m_ShadowImg)
		{
			m_PlayerIndex = Player::PlayerIndex;
			int adder = 2;
			int Len = (m_PlayerIndex - TI).IndexLen();
			FindPlayer[4] = TI;
			MoveFindPlayer();
			SetQuardant();
			int chkLen = Len;

			while (true)
			{
				if (FindPlayer[4] == m_PlayerIndex || chkLen < 0 || Len > 10)
				{
					break;
				}

				SetQuardant();
				Len += CheckWall();

				switch (m_Quardant)
				{
				case GameRender::Quardant::XPositive:
				case GameRender::Quardant::One:
					if (FindPlayer[4].X != m_PlayerIndex.X)
					{
						FindPlayer[4].X -= 1;
						MoveFindPlayer();

						if (FindPlayer[4] == m_PlayerIndex || FindPlayer[4].X == m_PlayerIndex.X)
						{
							break;
						}
						Len += CheckWall();
					}

					if (FindPlayer[4].Y != m_PlayerIndex.Y)
					{

						FindPlayer[4].Y += 1;
						MoveFindPlayer();

						if (FindPlayer[4] == m_PlayerIndex || FindPlayer[4].Y == m_PlayerIndex.Y)
						{
							break;
						}
						Len += CheckWall();
					}
					--chkLen;
					break;
				case GameRender::Quardant::XNegative:
				case GameRender::Quardant::YNegative:
				case GameRender::Quardant::Two:
					if (FindPlayer[4].X != m_PlayerIndex.X)
					{
						FindPlayer[4].X += 1;
						MoveFindPlayer();

						if (FindPlayer[4] == m_PlayerIndex || FindPlayer[4].X == m_PlayerIndex.X)
						{
							break;
						}
						Len += CheckWall();
					}
					if (FindPlayer[4].Y != m_PlayerIndex.Y)
					{
						FindPlayer[4].Y += 1;
						MoveFindPlayer();

						if (FindPlayer[4] == m_PlayerIndex || FindPlayer[4].Y == m_PlayerIndex.Y)
						{
							break;
						}
						Len += CheckWall();
					}
					--chkLen;
					break;
				case GameRender::Quardant::YPositive:
				case GameRender::Quardant::Three:
					if (FindPlayer[4].X != m_PlayerIndex.X)
					{
						FindPlayer[4].X += 1;
						MoveFindPlayer();

						if (FindPlayer[4] == m_PlayerIndex || FindPlayer[4].X == m_PlayerIndex.X)
						{
							break;
						}
						Len += CheckWall();

					}
					if (FindPlayer[4].Y != m_PlayerIndex.Y)
					{
						FindPlayer[4].Y -= 1;
						MoveFindPlayer();

						if (FindPlayer[4] == m_PlayerIndex || FindPlayer[4].Y == m_PlayerIndex.Y)
						{
							break;
						}
						Len += CheckWall();
					}
					--chkLen;
					break;
				case GameRender::Quardant::Four:
					if (FindPlayer[4].X != m_PlayerIndex.X)
					{
						FindPlayer[4].X -= 1;
						MoveFindPlayer();

						if (FindPlayer[4] == m_PlayerIndex || FindPlayer[4].X == m_PlayerIndex.X)
						{
							break;
						}
						Len += CheckWall();
					}
					if (FindPlayer[4].Y != m_PlayerIndex.Y)
					{
						FindPlayer[4].Y -= 1;
						MoveFindPlayer();

						if (FindPlayer[4] == m_PlayerIndex || FindPlayer[4].Y == m_PlayerIndex.Y)
						{
							break;
						}
						Len += CheckWall();
					}
					--chkLen;
					break;
				default:
					break;
				}
			}



			if (Len >= 10)
			{
				BF.SourceConstantAlpha = 255;
			}
			else
			{
				// 0   10
				// 0 ~ 1.0f
				float Ratio = Len / 10.0f;
				BF.SourceConstantAlpha = (int)(255 * Ratio);
			}

			findIter = m_AllTile.find(TI.Index);
			if (findIter != m_AllTile.end())
			{
				findIter->second->m_Len = Len;
			}



			AlphaBlend(GameTexMgr::BackDC()
				, Rc.IL()
				, Rc.IT()
				, Rc.Size.IX()
				, Rc.Size.IY()
				, m_ShadowImg->HDC()
				, m_ShadowRect.Pos.IX() // �̹����� x��������
				, m_ShadowRect.Pos.IY() // �̹����� y��������
				, m_ShadowRect.Size.IX()  // ��������� x �ȼ�
				, m_ShadowRect.Size.IY()
				, BF);
		}
	}

	// ��� Ÿ���� ���� �ʴ°� ����.
	// ȭ��ȿ� ���ϸ��� Ÿ�ϵ鸸 �����°� ����.
	// �̼Ҵ� ���� ��ƴ�.
}


//OpenList�� �����ϴ��� ���θ� Ȯ����
bool TileRender::IsOpenList(CVector::MapIndex _Index)
{
	return m_OpenIndexSet.find(_Index.Index) != m_OpenIndexSet.end();
}

TileRender::PathNode* TileRender::PathNodeCreate(CVector _Value)
{
	return PathNodeCreate(PosToIndex(_Value));
}


TileRender::PathNode* TileRender::PathNodeCreate(CVector::MapIndex _Value)
{
	//pool���� �ϳ� �����´ٰ� ��
	TileRender::PathNode* NewNode = *CurNode;
	NewNode->Index = _Value;

	++CurNode;

	if (CurNode == PathNodePool.end())
	{
		ASSERTMSG;
		CurNode = PathNodePool.begin();
	}

	return NewNode;
}


TileRender::PathNode* TileRender::OpenPathNodeCreate(CVector _Value, PathNode* _Parent)
{
	return OpenPathNodeCreate(PosToIndex(_Value), _Parent);
}

TileRender::PathNode* TileRender::OpenPathNodeCreate(CVector::MapIndex _Value, PathNode* _Parent) {
	//�����Ǹ鼭 ��ġ �ε��� ���� �޾Ƽ� �ڱ� �ε����� �־���
	PathNode* StartNode = PathNodeCreate(_Value);
	StartNode->CalDisData(EndIndex, _Parent);
	//�ʿ� ��ΰ��� Ű�� �־��ֱ� ������ ª�� ������� ���ĵǰ� �ȴ�.
	//���� ª�������� ã�ƺ��� �ȴٴ� ��.
	m_OpenList.insert(std::multimap<float, PathNode*>::value_type(StartNode->AllDis, StartNode));
	m_OpenIndexSet.insert(StartNode->Index.Index);
	return StartNode;
}

CVector::MapIndex TileRender::PosToIndex(const CVector _Pos)
{
	CVector::MapIndex Index;

	CVector RealPos = _Pos;
	RealPos += GameState::CamPos();

	Index.X = static_cast<int>(std::roundf(RealPos.X / GameActor::TileSize()));
	Index.Y = static_cast<int>(std::roundf(RealPos.Y / GameActor::TileSize()));

	return Index;
}


//8�������� ��ġ ã���ִ� ���
bool TileRender::PathNodePathFind(PathNode* _Node)
{
	// 4�����̳�?
	// 8�����̳�?

	CVector::MapIndex Cur = _Node->Index;
	CVector::MapIndex CheckIndex;
	std::map<__int64, TileData*>::iterator TileDataIter;

	for (size_t i = 0; i < TILEDIR_MAX; i++)
	{
		CheckIndex = Cur + m_DirArr[i];

		//�ε��� �¿� �ִٸ� ã����
		if (m_OpenIndexSet.end() != m_OpenIndexSet.find(CheckIndex.Index))
		{
			continue;
		}

		//Ŭ���� ����Ʈ�� �ִٸ� ã����
		if (m_CloseList.end() != m_CloseList.find(CheckIndex.Index))
		{
			continue;
		}

		//Ÿ���� �ƴ϶�� ã����
		TileDataIter = m_AllTile.find(CheckIndex.Index);
		if (m_AllTile.end() == TileDataIter)
		{
			continue;
		}

		//if (false == TileDataIter->second->m_Move)
		//{
		//	continue;
		//}
		//���¸���Ʈ�� �߰�����
		m_End = OpenPathNodeCreate(CheckIndex, _Node);

		//������ ��ġ�� ã�Ҵٸ� ����.
		//���⿡ ������ ���Ѵٸ� m_End�� ��� ���ο� ��ΰ��� ��
		if (CheckIndex == EndIndex)
		{
			return true;
		}

	}

	m_End = nullptr;

	return false;

}


//���¸���Ʈ���� �ϳ� ������ closelist�� �ִ� �Լ�
TileRender::PathNode* TileRender::OpenListPop()
{
	if (0 >= m_OpenList.size())
	{
		return nullptr;
	}

	std::multimap<float, PathNode*>::iterator PopNode = m_OpenList.begin();
	PathNode* ReturnNode = PopNode->second;
	m_OpenList.erase(PopNode);
	m_OpenIndexSet.erase(ReturnNode->Index.Index);

	//Ŭ�����Ʈ�� �ְ�
	m_CloseList.insert(std::multimap<__int64, PathNode*>::value_type(ReturnNode->Index.Index, ReturnNode));
	//ã��
	PathNodePathFind(ReturnNode);

	// ���� ã�Ƶ� �Űڳ�????
	return ReturnNode;
}

//Astar ������ ����
//Campos�� �������� ���� �׳� ��ġ�� �־��ָ� �˾Ƽ� campos�� �����ش�.
std::list<CVector> TileRender::PathFind(CVector _Start, CVector _End)
{
	//����Ȯ�ο�
	m_OpenList;
	m_CloseList;

	std::list<CVector> Path;

	if (PosToIndex(_Start) == PosToIndex(_End))
	{
		return Path;
	}

	EndIndex = PosToIndex(_End);
	OpenPathNodeCreate(_Start, nullptr);

	// �����Ϸ��� ������
	// n�� �߷��Ҽ� ����.
	// nȸ ã�ƾ��Ѵ�
	// ������ n�� �˼� �־�?

	// ���� ������ ������.
	// �ʹ� �־ ��尡 ������ ������ ����. 
	while (nullptr == m_End && 0 < m_OpenList.size())
	{
		//�ϳ� ������ Ŭ��� �ְ�
		//pathfind�� �ؼ� ���¸���Ʈ�� �ֺ� 8���� ����
		//������ ��ġ�� ã�����ϸ� m_End�� nullptr�� ��
		OpenListPop();
	}

	while (nullptr != m_End)
	{
		CVector::MapIndex TI = m_End->Index;
		CVector Pos;

		Pos.X = (TI.X * GameActor::TileSize());
		Pos.Y = (TI.Y * GameActor::TileSize());
		Path.push_front(Pos);

		m_End = m_End->Parent;
	}

	m_End = nullptr;

	ResetPool();

	return Path;
}


//������ Ǯ�� �ǵ����� �Լ�
void TileRender::ResetPool()
{
	CurNode = PathNodePool.begin();
	m_OpenList.clear();
	m_OpenIndexSet.clear();
	m_CloseList.clear();
}
