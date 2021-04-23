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

	float GameTimer = 0.0f;
	int BoardTimer = 0;

	bool GameOver = false;
	float EndScreenTimer = 0.0f;

	bool board[10][20];

	// Struct to hold info about the active tetromino
	struct tetromino {
		tetromino() {
			rotation = 0;
			shape = rand() % 7;
			pos = { 5, 19 };
		}

		int rotation;
		int shape;
		olc::vi2d pos;

	};

	tetromino* Active;

public:
	bool OnUserCreate() override
	{
		// Precomputed board values, saves frame time & complexness
		BoardWidth = 10 * 30;
		BoardHeight = 20 * 30;
		BorderX = (ScreenWidth() - BoardWidth) / 2;
		BorderY = (ScreenHeight() - BoardHeight) / 2;

		memset(board, false, sizeof(bool)*10*20);

		Active = new tetromino;

		// Graphics debug blocks
		board[0][0] = true;
		board[2][0] = true;
		board[1][1] = true;
		board[9][0] = true;
		board[9][19] = true;

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		//-------------- Gameplay Logic --------------

		if (GameTimer >= 1 && !GameOver) {
			GameTimer = 0.0f;

			// Lower active tetromino
			if (Active->pos.y > 0)
				Active->pos.y--;
			else {
				board[Active->pos.x][Active->pos.y] = true;
				Active = new tetromino;
			}

			BoardTimer++;
		}

		if (GetKey(olc::SPACE).bPressed)
			GameOver = true;


		//----------------- Graphics -----------------

		// Draw the board
		FillRect({ 0, 0 }, { ScreenWidth(), ScreenHeight() }, olc::BLACK);
		FillRect({ BorderX - 3, BorderY - 3 }, { BoardWidth + 6, BoardHeight + 6 }, olc::BLUE);
		FillRect({ BorderX, BorderY }, { BoardWidth, BoardHeight }, olc::DARK_GREY);

		// Draw the pieses & active tetromino
		
		for (int x = 0; x < 10; x++) {
			for (int y = 0; y < 20; y++) {
				if (board[x][y]) {
					FillRect({ 30 * x + BorderX, 30 * -y + BorderY + BoardHeight - 30 }, { 30, 30 }, olc::GREEN);
				}
			}
		}
		if (Active != nullptr) {
			FillRect({ 30 * Active->pos.x + BorderX, 30 * -Active->pos.y + BorderY + BoardHeight - 30 }, { 30, 30 }, olc::RED);
		}

		// Draw game timer
		DrawString({ 30, 30 }, std::to_string(BoardTimer), olc::GREY, 2);

		// Game Over screen
		if (GameOver) {
			FillRect({ ScreenWidth() / 10 - 3, ScreenHeight() / 2 - 13 }, { ScreenWidth() / 10 * 8 + 6, 26 }, olc::BLUE);
			FillRect({ ScreenWidth() / 10, ScreenHeight() / 2 - 10 }, { ScreenWidth() / 10 * 8, 20 }, olc::DARK_GREY);
			DrawString({ ScreenWidth() / 2 - 9 * 8, ScreenHeight() / 2 - 8 }, "Game Over", olc::BLACK, 2);
			
			EndScreenTimer += fElapsedTime;
			if(EndScreenTimer >= 3)
				return false;
		}

		GameTimer += fElapsedTime;
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