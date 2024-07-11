#pragma once
#include <math.h>
#include <time.h>
#include <Windows.h>
#include <assert.h>

//union TestUni 
//{
//	// 내부에 있는 각 변수들이 메모리 주소를 공유하게 된다.
//	// 00000000 01000000 00000000 00000000
//	// float FX;
//	// int IX;
//	struct 
//	{
//		int X;
//		int Y;
//	};
//
//	__int64 i64;
//};

enum class eDir
{
	DIRR,
	DIRT,
	DIRL,
	DIRB,
	DIRNONE,
	DIRMAX,
};


class GameRandom
{
private:
	int m_Data[32];

private:
	int GetRandom32bit()
	{
		int retValue = 0;
		for (int i = 0; i < 32; ++i)
		{
			m_Data[i] = rand() % 2;
			retValue |= m_Data[i];
			retValue <<= 1;
		}
		return retValue;
	}

public:
	int RandomMinMax(int _Min, int _Max)
	{
		if (_Min == _Max)
		{
			assert(false);
			return _Max;
		}

		if (_Min > _Max)
		{
			assert(false);
			return -1;
		}

		int value = GetRandom32bit();

		if (_Min > value && value < 0)
		{
			value *= -1;
		}

		return value % (_Max + 1 - _Min) + _Min;
	}

public:
	GameRandom() : m_Data()
	{
		srand((unsigned int)time(nullptr));
	}

	~GameRandom()
	{

	}

};



class CVector
{
public:
	static GameRandom RANDOM;

public:
	//전체 맵 인덱스에 접근하기 위한 용도.
	//64비트 자료형으로 X,Y 뭉친값을 의미, 이걸 가져다가 식별코드로 사용함
	union MapIndex
	{
		struct
		{
			int X;
			int Y;
		};
		__int64 Index;

	public:
		bool operator==(const MapIndex& _Other) {
			return X == _Other.X && Y == _Other.Y;
		}

		int IndexLen()
		{
			return abs(X) + abs(Y);
		}

		MapIndex operator-(const MapIndex& _Other)
		{
			MapIndex retValue;
			retValue.X = X - _Other.X;
			retValue.Y = Y - _Other.Y;
			return retValue;
		}

		MapIndex operator+(const MapIndex& _Other)
		{
			MapIndex retValue;
			retValue.X = X + _Other.X;
			retValue.Y = Y + _Other.Y;
			return retValue;
		}
	};


private:
	static CVector DLEFT;
	static CVector DRIGHT;
	static CVector DUP;
	static CVector DDOWN;
	static CVector DZERO;
	static CVector st_DELTADIR[static_cast<int>(eDir::DIRMAX)];

public:
	static const CVector& DELTALEFT() { return DLEFT; }
	static const CVector& DELTARIGHT() { return DRIGHT; }
	static const CVector& DELTAUP() { return DUP; }
	static const CVector& DELTADOWN() { return DDOWN; }

public:
	static CVector Lerp(CVector _Start, CVector _End, float _DeltaTime);

public:
	//GameWin의 메세지 데드타임에 들어간다.
	//델타타임을 단위벡터에 곱한 정보를 매 프레임마다 업데이트 함
	static void DIRUPDATE();

public:
	static const CVector LEFT;
	static const CVector RIGHT;
	static const CVector UP;
	static const CVector DOWN;
	static const CVector ZERO;

public:
	float X;
	float Y;

public:
	// 컴파일러가 
	// 헤더쪽에 구현해 놓으면 일반적으로 여러분들이 설정(컴파일 옵션)을
	// 바꾸지 않은한은 그냥 inline이 자동으로 붙었다고 보면 된다.
	inline int IX() const
	{
		return (int)X;
	}

	inline int IY() const
	{
		return (int)Y;
	}

	inline int IHX() const
	{
		return (int)HX();
	}

	inline int IHY() const
	{
		return (int)HY();
	}

	float HX()  const
	{
		return X * 0.5f;
	}

	float HY() const
	{
		return Y * 0.5f;
	}

	inline float Len() const
	{
		return sqrtf((X * X) + (Y * Y));
	}

public:
	static CVector UnitVectorAB(CVector _VectorB, CVector _VectorA);
	static CVector UnitVectorA(CVector _VectorA);
	static float GetVectorLength(CVector _VectorA);

public:
	CVector operator *(const float _Value) const
	{
		return { X * _Value, Y * _Value };
	}


	CVector& operator *=(const float _Value)
	{
		X *= _Value;
		Y *= _Value;
		return *this;
	}

	CVector operator +(const CVector& _Other) const
	{
		return { X + _Other.X, Y + _Other.Y };
	}


	CVector& operator +=(const CVector& _Other)
	{
		X += _Other.X;
		Y += _Other.Y;
		return *this;
	}

	CVector operator -(const CVector& _Other) const
	{
		return { X - _Other.X, Y - _Other.Y };
	}

	CVector& operator -=(const CVector& _Other)
	{
		X -= _Other.X;
		Y -= _Other.Y;
		return *this;
	}


	bool operator ==(const CVector& _Other) const
	{
		return (X == _Other.X) && (Y == _Other.Y);
	}


	bool operator !=(const CVector& _Other) const
	{
		return (X != _Other.X) || (Y != _Other.Y);
	}

public:
	CVector() : X(0.0f), Y(0.0f)
	{

	}

	CVector(float _X, float _Y) : X(_X), Y(_Y)
	{

	}

	CVector(int _X, int _Y) : X(static_cast<float>(_X)), Y(static_cast<float>(_Y))
	{

	}
};

class GameRect
{
public:
	CVector Pos;
	CVector Size;

public:
	float L() const
	{
		return Pos.X - (Size.HX());
	}
	float R() const
	{
		return Pos.X + (Size.HX());
	}
	float T() const
	{
		return Pos.Y - (Size.HY());
	}
	float B() const
	{
		return Pos.Y + (Size.HY());
	}

	int IL() const
	{
		return (int)L();
	}
	int IR()const
	{
		return (int)R();
	}
	int IT() const
	{
		return (int)T();
	}
	int IB() const
	{
		return (int)B();
	}
};


class GameMath
{
public:
	static bool RECTTORECT(CVector _LeftPos, CVector _LeftSize, CVector _RightPos, CVector _RightSize)
	{
		return RECTTORECT({ _LeftPos , _LeftSize }, { _RightPos , _RightSize });
	}
	static bool RECTTOCIRCLE(CVector _LeftPos, CVector _LeftSize, CVector _RightPos, float _RightSize)
	{
		return false;
	}
	static bool RECTTOPOINT(CVector _LeftPos, CVector _LeftSize, CVector _RightPos)
	{
		return false;
	}

	static bool CIRCLETOCIRCLE(CVector _LeftPos, float _LeftSize, CVector _RightPos, float _RightSize)
	{
		return false;
	}
	static bool CIRCLETORECT(CVector _LeftPos, float _LeftSize, CVector _RightPos, CVector _RightSize)
	{
		return false;
	}
	static bool CIRCLETOPOINT(CVector _LeftPos, float _LeftSize, CVector _RightPos)
	{
		return false;
	}

	static bool POINTTOPOINT(CVector _LeftPos, CVector _RightPos) {
		return false;
	}
	static bool POINTTORECT(CVector _LeftPos, CVector _RightPos, CVector _RightSize) {
		return false;
	}
	static bool POINTTOCIRCLE(CVector _LeftPos, CVector _RightPos, float _RightSize) {
		return false;
	}

	
	static bool RECTTORECT(const GameRect& _Left, const GameRect& _Right)
	{
		if (_Left.R() < _Right.L())
		{
			return false;
		}

		if (_Left.L() > _Right.R())
		{
			return false;
		}

		if (_Left.T() > _Right.B())
		{
			return false;
		}

		if (_Left.B() < _Right.T())
		{
			return false;
		}

		return true;
	}
	static bool RECTTOCIRCLE(const GameRect& _Left, const GameRect& _Right)
	{
		return false;
	}
	static bool RECTTOPOINT(const GameRect& _Left, const GameRect& _Right)
	{
		return false;
	}

	static bool CIRCLETOCIRCLE(const GameRect& _Left, const GameRect& _Right)
	{
		return false;
	}
	static bool CIRCLETORECT(const GameRect& _Left, const GameRect& _Right)
	{
		return false;
	}
	static bool CIRCLETOPOINT(const GameRect& _Left, const GameRect& _Right)
	{
		return false;
	}
	static bool POINTTOPOINT(const GameRect& _Left, const GameRect& _Right)
	{
		return false;
	}
	static bool POINTTORECT(const GameRect& _Left, const GameRect& _Right)
	{
		return false;
	}
	static bool POINTTOCIRCLE(const GameRect& _Left, const GameRect& _Right)
	{
		return false;
	}

public:
	GameMath();
	~GameMath();
};

