#pragma once


class GameGraphic
{
public:
	static int ColorToInt(int r, int g, int b, int a)
	{
		int Color;

		//int �ּҸ� char*�� �����鼭
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

