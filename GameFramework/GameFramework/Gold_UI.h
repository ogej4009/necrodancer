#pragma once
#include "GameActor.h"


class UI_Text;
class Gold_UI : public GameActor
{
private:
	GameRender* m_Gold;
	UI_Text* m_Text;
	UI_Text* m_TextCount;
	CVector m_Position;
	int m_PlayerGold;

public:
	Gold_UI();
	~Gold_UI();

public:
	void ChangeGold();
	virtual void Update() override;
	void InitRender(CVector _Pos);
	void InitText(CVector _Pos);

};

