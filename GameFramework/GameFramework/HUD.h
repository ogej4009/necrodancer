#pragma once
#include "GameActor.h"

class UI_Text;
class HUD : public GameActor
{
private:
	enum HudType
	{
		Shovel,
		Weapon,
		Head,
		Body,
		Canthrow,
		Magic,
		Bomb,


	};


private:
	std::map<int, GameRender*> m_Hud;
	std::map<int, GameRender*> m_HudItem;
	std::map<int, UI_Text*> m_HudText;
	CVector m_Position;
	CVector m_SecondPosition;
	CVector m_ThirdPosition;
	float m_Size;
	float m_HudPadding;
	std::wstring m_HudName[14];
	std::wstring m_WeaponName[4];
	int m_PlayerWeaponType;
	int m_PlayerHeadType;
	int m_PlayerArmorType;


	std::map<int, GameRender*>::iterator findIter;
	std::map<int, GameRender*>::iterator findIter2;
	std::map<int, UI_Text*>::iterator findTextIter;


public:
	HUD();
	~HUD();

public:
	virtual void Update() override;
	void InitRender(CVector _Pos);

private:

	void ChagneWeaponImg(int _WeaponType);
	void TurnOnHud(int _HudNum);

};

