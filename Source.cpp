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

	bool board[10][20];

public:
	bool OnUserCreate() override
	{
		BoardWidth = 10 * 30;
		BoardHeight = 20 * 30;
		BorderX = (ScreenWidth() - BoardWidth) / 2;
		BorderY = (ScreenHeight() - BoardHeight) / 2;

		memset(board, false, sizeof(bool)*10*20);

		board[5][10] = true;

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		//-------------- Gameplay Logic --------------

		

		


		//----------------- Graphics -----------------

		// Draw the board
		FillRect({ 0, 0 }, { ScreenWidth(), ScreenHeight() }, olc::BLACK);
		FillRect({ BorderX - 3, BorderY - 3 }, { BoardWidth + 6, BoardHeight + 6 }, olc::BLUE);
		FillRect({ BorderX, BorderY }, { BoardWidth, BoardHeight }, olc::DARK_GREY);

		// Draw the pieces
		
		for (int x = 0; x < 10; x++) {
			for (int y = 0; y < 20; y++) {
				if (board[x][y]) {
					FillRect({ 10 * x + BorderX, 10 * y + BorderY }, { 10, 10 }, olc::GREEN);
				}
			}
		}

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