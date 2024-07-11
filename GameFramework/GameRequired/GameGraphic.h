#pragma once


class GameGraphic
{
public:
	static int ColorToInt(int r, int g, int b, int a)
	{
		int Color;

		//int 주소를 char*로 읽으면서
		char* ArrColor = (char*)&Color;

		ArrColor[0] = r;
		ArrColor[1] = g;
		ArrColor[2] = b;
		ArrColor[3] = a;
	
		return Color;
	}

private:
	GameGraphic();
	virtual ~GameGraphic() = 0;
};

