//#include "stdafx.h"

#include "AniRender.h"
#include <GameMacro.h>
#include <GameTime.h>
#include <GameTexMgr.h>
#include "GameActor.h"


AniRender::AniRender() : m_CurAni(nullptr), bIsShadow(false)
{
}

AniRender::~AniRender()
{
	auto StartIter = m_AniMap.begin();
	auto EndIter = m_AniMap.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (nullptr != StartIter->second)
		{
			delete StartIter->second;
			StartIter->second = nullptr;
		}
	}
}

AniRender::GameAni* AniRender::FindAni(int _EnumName)
{
	FindIter = m_AniMap.find(_EnumName);

	if (FindIter == m_AniMap.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

bool AniRender::CreateAni(int _EnumName
	, const wchar_t* _SpriteName
	,unsigned int _StartFrame
	, unsigned int _EndFrame
	,bool _IsLoop
	, float _FrameTime /*= 0.1f*/)
{
	GameAni* pAni = FindAni(_EnumName);

	if (nullptr != pAni)
	{
		ASSERTMSG;
		return false;
	}

	GameImage* pImg = GameTexMgr::Inst().FindImg(_SpriteName);

	if (nullptr == pImg)
	{
		ASSERTMSG;
		return false;
	}

	pAni = new GameAni();
	pAni->m_Image = pImg;
	pAni->m_bLoop = _IsLoop;
	pAni->m_StartFrame = _StartFrame;
	pAni->m_EndFrame = _EndFrame;
	pAni->m_fFrameTime = _FrameTime;
	m_AniMap.insert(std::map<int, GameAni*>::value_type(_EnumName, pAni));
	return true;
}

bool AniRender::ChangeAni(int _EnumName)
{
	m_CurAni = FindAni(_EnumName);

	if (nullptr == m_CurAni)
	{
		ASSERTMSG;
		return false;
	}

	m_Img = m_CurAni->m_Image;
	m_CurAni->m_bEnd = false;
	m_CurAni->m_CurFrame = m_CurAni->m_StartFrame;
	m_CurAni->m_fCurFrameTime = m_CurAni->m_fFrameTime;

	return true;
}

void AniRender::Render()
{
	//부모 액터가 없으면 펑
	if (nullptr == m_pParent)
	{
		ASSERTMSG;
	}

	GameRect Rc;

	switch (m_pParent->ActorType())
	{
	case EActorTypeIndex::UI:
		Rc = { m_pParent->Pos() + m_SubPos , m_SubSize };
		break;
	default:
		Rc = CalRect();
		break;
	}

	m_Rect = m_Img->GetRect(m_CurAni->m_CurFrame);

	//주어진 프레임 시간에서 빼는 형태로 구현
	m_CurAni->m_fCurFrameTime -= GameTime::DeltaTime();

	if (0.0f >= m_CurAni->m_fCurFrameTime)
	{
		++m_CurAni->m_CurFrame;
		m_CurAni->m_fCurFrameTime = m_CurAni->m_fFrameTime;

		if (m_CurAni->m_CurFrame > m_CurAni->m_EndFrame)
		{
			m_CurAni->m_bEnd = true;
			
			if (true == m_CurAni->m_bLoop)
			{
				m_CurAni->m_CurFrame = m_CurAni->m_StartFrame;
			}
			else {
				m_CurAni->m_CurFrame = m_CurAni->m_EndFrame;
			}
		}
	}

	TransparentBlt(GameTexMgr::BackDC()
		, Rc.IL()
		, Rc.IT()
		, Rc.Size.IX()
		, Rc.Size.IY()
		, m_Img->HDC()
		, m_Rect.Pos.IX() 
		, m_Rect.Pos.IY() 
		, m_Rect.Size.IX()
		, m_Rect.Size.IY()
		, m_TransColor);

}

void AniRender::ChangeLoopOn(int _EnumName)
{
	GameAni* tempAni = FindAni(_EnumName);
	if (tempAni == nullptr)
	{
		ASSERTMSG;
	}
	tempAni->LoopOn();
}

void AniRender::ChangeLoopOff(int _EnumName)
{
	GameAni* tempAni = FindAni(_EnumName);
	if (tempAni == nullptr)
	{
		ASSERTMSG;
	}
	tempAni->LoopOff();
}
