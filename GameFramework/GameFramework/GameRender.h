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
	//������ �Ҹ��� �θ� Actor�� ������ �ִ� ���
	friend GameActor;
	//��� �������� �����ϱ� ���� State�� ������ȯ
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
	//������ ���� -> Z order�� �������� �򸮰� ��
	int m_Order;

	std::wstring m_Name;
	std::map<__int64, GameActor*>::iterator findIter;
	std::map<__int64, GameActor*>::iterator endIter;

	//�÷��̾���� �Ÿ������� index
	CVector::MapIndex FindPlayer[9];
	std::map<__int64, GameActor*>::iterator EndIter;
	//X���� Ž���ߴ��� Ȯ���ϴ� �뵵
	bool m_bXSearch;

	//������ �������� �̹��� ����
	GameImage* m_Img;
	GameImage* m_ShadowImg;

	//���° �̹����ΰ�?
	int m_ImgIndex;
	int m_ShadowImgIndex;
	//���� Image Ŭ������ �����ϰ� �ִ� ��ġ������ ������ ���� ���
	GameRect m_Rect;
	GameRect m_ShadowRect;
	//���� ��
	int m_TransColor;
	BLENDFUNCTION BF;


protected:
	void SetQuardant();
	int CheckWall();
	CVector::MapIndex m_PlayerIndex;

public:
	void TransColor(int r, int g, int b)
	{
		//�ش� �Լ��� ���ؼ� �̹��� ������ ������
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
	//State�� �׸� �� �ֵ��� ȣ���ϴ� �Լ�
	//���� Actor�� Render�� ����������, Actor�� ��ġ�� ������ �ְ� ��
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

