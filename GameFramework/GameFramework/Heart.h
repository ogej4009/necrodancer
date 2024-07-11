#pragma once
#include "GameActor.h"
#include <vector>


class Heart :public GameActor
{
private:
	std::vector<AniRender*> m_Heart;
	std::vector<AniRender*>::iterator m_StartIter;
	std::vector<AniRender*>::iterator m_EndIter;
	int m_HeartCount;
	int m_HeartGroupCount;
	float m_Timer;
	int m_BeatCount;
	CVector m_Position;

	enum HeartState {
		FULL,
		HALF,
		NONE
	};
public:
	Heart(int _HeartCount);
	~Heart();

public:
	virtual void Update() override;
	void InitAni(CVector _Pos);

private:
	void CheckHeart();
	void HeartBeat();

};

