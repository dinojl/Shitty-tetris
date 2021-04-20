#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		// Window name
		sAppName = "Shitty Tetris";
	}

	int BoardWidth;
	int BoardHeight;
	int BorderX;
	int BorderY;


public:
	bool OnUserCreate() override
	{
		BoardWidth = 10 * 30;
		BoardHeight = 20 * 30;
		BorderX = (ScreenWidth() - BoardWidth) / 2;
		BorderY = (ScreenHeight() - BoardHeight) / 2;


		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{



		


		//----------------- Graphics -----------------
		FillRect({ 0, 0 }, { ScreenWidth(), ScreenHeight() }, olc::BLACK);
		FillRect({ BorderX - 3, BorderY - 3 }, { BoardWidth + 6, BoardHeight + 6 }, olc::BLUE);
		FillRect({ BorderX, BorderY }, { BoardWidth, BoardHeight }, olc::DARK_GREY);
		
		
		return true;
	}
};

int main()
{
	Example demo;
	if (demo.Construct(480, 720, 1, 1))
		demo.Start();
	return 0;
}