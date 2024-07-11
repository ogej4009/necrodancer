#pragma once
#include "GameActor.h"
#include "Player.h"

class Text;
class ShopItem :
	public GameActor
{
private:
	friend Player;
	friend MapEditor;
	struct _ShopItem
	{
		GameActor* ITEM;
		int price;
	};

private:
	CVector m_InitPos;
	_ShopItem m_Item[3];
	CVector::MapIndex m_ItemIndex[3];
	Text* m_Text[3];
	bool Sell[3];

public:
	ShopItem(CVector _Pos);
	~ShopItem();


public:
	void Update() override;
};

