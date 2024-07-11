#pragma once
#include "GameActor.h"
#include <vector>

class MapEditor;
class Text : public GameActor
{
public:
	friend MapEditor;

private:
	std::vector<GameRender *> m_Text;
	std::vector<GameRender *>::iterator m_StartIter;
	std::vector<GameRender *>::iterator m_EndIter;
	std::wstring m_TextData;
	CVector::MapIndex m_TextPos;
	int m_Len;

public:
	Text(CVector _TargetPos, 
		CVector _TextSize ={ 12.0f, 12.0f },
		std::wstring _TextData = L"Null");
	Text(CVector _TargetPos
		, CVector _TextSize
		, std::wstring _TextData
		, EActorTypeIndex _Type);
	~Text();

public:
	void Update() override;
	void ChangeSize(CVector _Size);
	std::wstring GetText()
	{
		return m_TextData;
	}
};

