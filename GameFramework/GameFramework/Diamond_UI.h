#pragma once
#include "GameActor.h"


class UI_Text;
class Diamond_UI : public GameActor
{
private:
	GameRender* m_Diamond;
	UI_Text* m_Text;
	UI_Text* m_TextCount;
	CVector m_Position;

public:
	Diamond_UI();
	~Diamond_UI();


public:
	virtual void Update() override;
	void InitRender(CVector _Pos);
	void InitText(CVector _Pos);
};

