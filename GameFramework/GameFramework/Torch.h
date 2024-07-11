#pragma once
#include "GameActor.h"

class MapEditor;
class Torch : public GameActor
{
public:
	friend MapEditor;

private:
	AniRender* m_Torch;

	enum TorchState {
		Torchi = 0,
	};
	int m_Len;

public:
	Torch(CVector _TargetPos);
	~Torch();

public:
	void Update() override;

private:
	void InitAni(CVector _TargetPos);


};

