#pragma once
#include <map>
#include <list>
#include "ActorSub.h"
#include <GameTex.h>
#include <GameImage.h>
#include <GameGraphic.h>


class GameActor;
class GameState;
class GameRender : public ActorSub
{
	//생성과 소멸은 부모 Actor만 가지고 있는 기능
	friend GameActor;
	//모든 랜더러를 관리하기 위해 State와 정보교환
	friend GameState;

protected:
	enum class Quardant
	{
		Zero,
		One,
		Two,
		Three,
		Four,
		XPositive,
		YPositive,
		XNegative,
		YNegative,
	};

	Quardant m_Quardant;

protected:
	//랜더링 순서 -> Z order로 낮을수록 깔리게 됨
	int m_Order;

	std::wstring m_Name;
	std::map<__int64, GameActor*>::iterator findIter;
	std::map<__int64, GameActor*>::iterator endIter;

	//플레이어와의 거리측정용 index
	CVector::MapIndex FindPlayer[9];
	std::map<__int64, GameActor*>::iterator EndIter;
	//X축을 탐색했는지 확인하는 용도
	bool m_bXSearch;

	//실제로 랜더링할 이미지 정보
	GameImage* m_Img;
	GameImage* m_ShadowImg;

	//몇번째 이미지인가?
	int m_ImgIndex;
	int m_ShadowImgIndex;
	//실제 Image 클래스가 보관하고 있는 위치정보를 가지고 있을 멤버
	GameRect m_Rect;
	GameRect m_ShadowRect;
	//지울 색
	int m_TransColor;
	BLENDFUNCTION BF;


protected:
	void SetQuardant();
	int CheckWall();
	CVector::MapIndex m_PlayerIndex;

public:
	void TransColor(int r, int g, int b)
	{
		//해당 함수를 통해서 이미지 정보를 가져옴
		m_TransColor = GameGraphic::ColorToInt(r, g, b, 0);
	}

	void ChangeOrder(int _Order)
	{
		m_Order = _Order;
	}

public:
	int Order() 
	{
		return m_Order;
	}

	int Index()
	{
		return m_ImgIndex;
	}

	void InitFindPlayer();
	void MoveFindPlayer();

	void Index(int _Index);
	void ShadowIndex(int _Index);

	void Image(const wchar_t* _Name, int _Index = 0);
	void ShadowImage(const wchar_t* _Name, int _Index =0);

	std::wstring* GetImageName();
private:
	//State가 그릴 수 있도록 호출하는 함수
	//이제 Actor의 Render는 유명무실해짐, Actor는 위치만 가지고 있게 됨
	virtual void Render();
	void AddRenderPos(CVector _Pos);

public:
	virtual void SubPos(CVector _Pos) override;
	virtual CVector SubPos() override
	{
		return m_SubPos;
	}
	virtual void SubPosPure(CVector _Pos) override;


public:
	GameRender();
	virtual ~GameRender();
};

