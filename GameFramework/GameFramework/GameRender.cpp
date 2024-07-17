//#include "stdafx.h"
#include "GameRender.h"
#include <GameTexMgr.h>
#include "GameActor.h"
#include "GameState.h"
#include <GameMacro.h>
//#include <cstring>
#include "Player.h"
#include <GameWin.h>

GameRender::GameRender()
	: m_TransColor(0)
	, m_Order(0)
	, m_ImgIndex(0)
	, m_ShadowImgIndex(0)
	, m_Img(nullptr)
	, m_ShadowImg(nullptr)
	, m_Rect({ CVector::ZERO, CVector::ZERO })
	, m_ShadowRect({ CVector::ZERO, CVector::ZERO })
	, BF({})
	, m_bXSearch(false)
	, m_Quardant(Quardant::One)
{

	TransColor(255, 0, 255);//안쓰는 묘한 보라색(진한핑크)
	BF.BlendOp = AC_SRC_OVER;
	BF.BlendFlags = 0;
	BF.SourceConstantAlpha = 255;
	BF.AlphaFormat = AC_SRC_ALPHA;
}

void GameRender::MoveFindPlayer()
{
	FindPlayer[0] = { FindPlayer[4].X - 1 , FindPlayer[4].Y - 1 };
	FindPlayer[1] = { FindPlayer[4].X , FindPlayer[4].Y - 1 };
	FindPlayer[2] = { FindPlayer[4].X + 1 , FindPlayer[4].Y - 1 };
	FindPlayer[3] = { FindPlayer[4].X - 1 , FindPlayer[4].Y };
	FindPlayer[5] = { FindPlayer[4].X + 1 , FindPlayer[4].Y };
	FindPlayer[6] = { FindPlayer[4].X - 1 , FindPlayer[4].Y + 1 };
	FindPlayer[7] = { FindPlayer[4].X , FindPlayer[4].Y + 1 };
	FindPlayer[8] = { FindPlayer[4].X + 1 , FindPlayer[4].Y + 1 };

}

//렌더러 생성시 호출해줘야한다. 
void GameRender::InitFindPlayer()
{
	FindPlayer[4] = m_pParent->MapIndex();
	FindPlayer[0] = { FindPlayer[4].X - 1 , FindPlayer[4].Y - 1 };
	FindPlayer[1] = { FindPlayer[4].X , FindPlayer[4].Y - 1 };
	FindPlayer[2] = { FindPlayer[4].X + 1 , FindPlayer[4].Y - 1 };
	FindPlayer[3] = { FindPlayer[4].X - 1 , FindPlayer[4].Y };
	FindPlayer[5] = { FindPlayer[4].X + 1 , FindPlayer[4].Y };
	FindPlayer[6] = { FindPlayer[4].X - 1 , FindPlayer[4].Y + 1 };
	FindPlayer[7] = { FindPlayer[4].X , FindPlayer[4].Y + 1 };
	FindPlayer[8] = { FindPlayer[4].X + 1 , FindPlayer[4].Y + 1 };

	EndIter = m_pParent->State()->m_WallActorMap.end();

	//플레이어기준 어디 사사분면에 있는지 확인
	SetQuardant();
}


void GameRender::SetQuardant()
{
	if (FindPlayer[4].X - m_PlayerIndex.X == 0
		&& FindPlayer[4].Y - m_PlayerIndex.Y == 0)
	{
		m_Quardant = Quardant::Zero;
		return;
	}

	if (FindPlayer[4].X - m_PlayerIndex.X == 0)
	{
		if (FindPlayer[4].Y - m_PlayerIndex.Y > 0)
		{
			m_Quardant = Quardant::YPositive;
		}
		else
		{
			m_Quardant = Quardant::YNegative;
		}
		return;
	}
	else if (FindPlayer[4].Y - m_PlayerIndex.Y == 0)
	{
		if (FindPlayer[4].X - m_PlayerIndex.X > 0)
		{
			m_Quardant = Quardant::XPositive;
		}
		else
		{
			m_Quardant = Quardant::XNegative;
		}
		return;
	}

	if (FindPlayer[4].X - m_PlayerIndex.X > 0)
	{
		if (FindPlayer[4].Y - m_PlayerIndex.Y > 0)
		{
			m_Quardant = Quardant::Four;
		}
		else
		{
			m_Quardant = Quardant::One;
		}
		return;
	}
	else //X < 0
	{
		if (FindPlayer[4].Y - m_PlayerIndex.Y > 0)
		{
			m_Quardant = Quardant::Three;
		}
		else
		{
			m_Quardant = Quardant::Two;
		}
	}
}


int GameRender::CheckWall()
{
	int adder = 9;
	int Len = 0;

	switch (m_Quardant)
	{
	case GameRender::Quardant::XPositive:
		if (m_pParent->State()->m_WallActorMap.find(FindPlayer[3].Index) != EndIter)
		{
			Len += adder;
		}
		break;

	case GameRender::Quardant::XNegative:
		if (m_pParent->State()->m_WallActorMap.find(FindPlayer[5].Index) != EndIter)
		{
			Len += adder;
		}
		break;

	case GameRender::Quardant::YPositive:
		if (m_pParent->State()->m_WallActorMap.find(FindPlayer[1].Index) != EndIter)
		{
			Len += adder;
		}
		break;

	case GameRender::Quardant::YNegative:
		if (m_pParent->State()->m_WallActorMap.find(FindPlayer[7].Index) != EndIter)
		{
			Len += adder;
		}
		break;

	case GameRender::Quardant::One:

		if (m_pParent->State()->m_WallActorMap.find(FindPlayer[6].Index) != EndIter)
		{
			Len += adder;
		}

		break;
	case GameRender::Quardant::Two:
		if (m_pParent->State()->m_WallActorMap.find(FindPlayer[8].Index) != EndIter)
		{
			Len += adder;
		}

		break;
	case GameRender::Quardant::Three:
		if (m_pParent->State()->m_WallActorMap.find(FindPlayer[2].Index) != EndIter)
		{
			Len += adder;
		}

		break;
	case GameRender::Quardant::Four:
		if (m_pParent->State()->m_WallActorMap.find(FindPlayer[0].Index) != EndIter)
		{
			Len += adder;
		}

		break;
	case GameRender::Quardant::Zero:
		if (m_pParent->State()->m_WallActorMap.find(FindPlayer[0].Index) != EndIter)
		{
			Len += adder;
		}

		break;
	default:
		ASSERTMSG;
		break;
	}

	return Len;
}

GameRender::~GameRender()
{
}


//추후 부모 Actor에게 이미지 로드할 경로를 받아야 할것
//지금은 그냥 되는지만 확인
void GameRender::Render()
{
	if (nullptr == m_pParent)
	{
		ASSERTMSG;
	}

	GameRect Rc;

	switch (m_pParent->ActorType())
	{
	case EActorTypeIndex::UI:
	case EActorTypeIndex::UIText:
	case EActorTypeIndex::Background:
	case EActorTypeIndex::UIBeatBar:
		Rc = { m_pParent->Pos() + m_SubPos , m_SubSize };
		break;
	default:
		Rc = CalRect();
		break;
	}


	TransparentBlt(GameTexMgr::BackDC()
		, Rc.IL()
		, Rc.IT()
		, Rc.Size.IX()
		, Rc.Size.IY()
		, m_Img->HDC()
		, m_Rect.Pos.IX() // 이미지의 x에서부터
		, m_Rect.Pos.IY() // 이미지의 y에서부터
		, m_Rect.Size.IX()  // 여기까지의 x 픽셀
		, m_Rect.Size.IY()
		, m_TransColor);

	int Len = 0;

	if (nullptr != m_ShadowImg)
	{
		if (m_pParent->ActorType() == EActorTypeIndex::Player)
		{
			//빨간화면 랜더링용
			BF.SourceConstantAlpha = 100;
			AlphaBlend(GameTexMgr::BackDC()
				, Rc.IL()
				, Rc.IT()
				, Rc.Size.IX()
				, Rc.Size.IY()
				, m_ShadowImg->HDC()
				, m_ShadowRect.Pos.IX() // 이미지의 x에서부터
				, m_ShadowRect.Pos.IY() // 이미지의 y에서부터
				, m_ShadowRect.Size.IX()  // 여기까지의 x 픽셀
				, m_ShadowRect.Size.IY()
				, BF);

			return;
		}
		else if (m_pParent->ActorType() == EActorTypeIndex::UIBeatBar)
		{
			CVector HeartPosition = { GameWin::MainWindow()->Size().X / 2 , GameWin::MainWindow()->Size().Y - 40.0f * 2 };

			if (SubPos().X > 480)
			{
				Len = (int)(SubPos().X - HeartPosition.X) - 125;

				// 0   10
				// 0 ~ 1.0f
				if (Len > 255)
				{
					Len = 255;
				}
				else if (Len < 1)
				{
					Len = 0;
				}
				BF.SourceConstantAlpha = Len;
			}
			else
			{

				Len = (int)(HeartPosition.X - SubPos().X) - 125;

				// 0   10
				// 0 ~ 1.0f
				if (Len > 255)
				{
					Len = 255;
				}
				else if (Len < 1)
				{
					Len = 0;
				}
				BF.SourceConstantAlpha = Len;
			}

		}
		else
		{

			m_PlayerIndex = Player::PlayerIndex;
			int adder = 3;
			int Len = (m_PlayerIndex - m_pParent->MapIndex()).IndexLen();
			FindPlayer[4] = m_pParent->MapIndex();
			MoveFindPlayer();
			SetQuardant();


			int chkLen = Len;

			m_PlayerIndex = Player::PlayerIndex;
			Len = (FindPlayer[4] - m_PlayerIndex).IndexLen();
			//플레이어가 움직이면 사분면도 움직여야됨
			SetQuardant();

			Len += CheckWall();

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


		}
		


		AlphaBlend(GameTexMgr::BackDC()
			, Rc.IL()
			, Rc.IT()
			, Rc.Size.IX()
			, Rc.Size.IY()
			, m_ShadowImg->HDC()
			, m_ShadowRect.Pos.IX() // 이미지의 x에서부터
			, m_ShadowRect.Pos.IY() // 이미지의 y에서부터
			, m_ShadowRect.Size.IX()  // 여기까지의 x 픽셀
			, m_ShadowRect.Size.IY()
			, BF);

	}
}


void GameRender::Image(const wchar_t* _Name, int _Index /*=0*/)
{
	//등록되어 있을 이미지 클래스 찾기
	m_Img = GameTexMgr::Inst().FindImg(_Name);
	m_Name = _Name;
	Index(_Index);

	return;
}

void GameRender::ShadowImage(const wchar_t* _Name, int _Index /*=0*/)
{
	//등록되어 있을 이미지 클래스 찾기
	m_ShadowImg = GameTexMgr::Inst().FindImg(_Name);
	ShadowIndex(_Index);
	return;
}

std::wstring*  GameRender::GetImageName()
{
	if (m_Img == nullptr)
	{
		ASSERTMSG;
		return nullptr;
	}

	return &m_Name;
}


void GameRender::Index(int _Index)
{
	m_ImgIndex = _Index;

	if (nullptr == m_Img)
	{
		ASSERTMSG;
	}

	// 같아도 안된다.
	if (_Index >= static_cast<int>(m_Img->CutCount()))
	{
		ASSERTMSG;
	}

	//여기서 잘라놓은 위치정보를 가져온다.
	m_Rect = m_Img->GetRect(_Index);
}

void GameRender::ShadowIndex(int _Index)
{
	m_ShadowImgIndex = _Index;

	if (nullptr == m_ShadowImg)
	{
		ASSERTMSG;
	}

	// 같아도 안된다.
	if (_Index >= static_cast<int>(m_ShadowImg->CutCount()))
	{
		ASSERTMSG;
	}

	//여기서 잘라놓은 위치정보를 가져온다.
	m_ShadowRect = m_ShadowImg->GetRect(_Index);
}

void  GameRender::SubPos(CVector _Pos)
{
	m_SubPos = _Pos;
	AddRenderPos(_Pos);
}

void GameRender::SubPosPure(CVector _Pos)
{
	m_SubPos = _Pos;
}



void GameRender::AddRenderPos(CVector _Pos)
{
	EActorTypeIndex type = m_pParent->CheckType(m_pParent->ActorType());

	m_SubIndex.X = static_cast<int>(_Pos.X / GameActor::TileSize());
	m_SubIndex.Y = static_cast<int>(_Pos.Y / GameActor::TileSize());

	m_SubIndex = { m_SubIndex.X, m_SubIndex.Y };
}