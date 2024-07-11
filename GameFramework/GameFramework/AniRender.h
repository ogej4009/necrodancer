#pragma once
#include "GameRender.h"
#include <map>


// 필요한 랜더러를 만들자.
class AniRender : public GameRender
{
	class GameAni	//animation 정보를 가지고 있는 이너클래스
	{
	public:
		GameImage* m_Image;
		unsigned int m_StartFrame;
		unsigned int m_EndFrame;
		float m_fFrameTime;
		bool m_bLoop;
		float m_fCurFrameTime;
		unsigned int m_CurFrame;
		bool m_bEnd;

		void LoopOn()
		{
			m_bLoop = true;
		}

		void LoopOff()
		{
			m_bLoop = false;
		}

	};

private:
	std::map<int, GameAni*>::iterator FindIter;
	std::map<int, GameAni*>::iterator CurIter;
	std::map<int, GameAni*> m_AniMap;
	GameAni* m_CurAni;
	bool bIsShadow;

public:
	bool IsCurAniEnd()
	{
		return m_CurAni->m_bEnd;
	}

	void SetShadow()
	{
		bIsShadow = true;
	}

	void Restart()
	{
		m_CurAni->m_CurFrame = m_CurAni->m_StartFrame;
		m_CurAni->m_fCurFrameTime = m_CurAni->m_fFrameTime;
		m_CurAni->m_bEnd = false;
	}

private:
	GameAni* FindAni(int _EnumName);

public:
	bool CreateAni(int _EnumName
		, const wchar_t* _SpriteName
		, unsigned int _StartFrame
		, unsigned int _EndFrame
		,bool _IsLoop
		, float _FrameTime = 0.1f
	);
	bool ChangeAni(int _EnumName);
	void ChangeLoopOn(int _EnumName);
	void ChangeLoopOff(int _EnumName);

private:
	void Render();

public:
	AniRender();
	~AniRender();
};
