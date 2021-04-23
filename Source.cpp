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
	int Score = 0;

	// Struct to hold info about blocks
	struct block {
		block(int x = 5, int y = 19) {
			pos = { x, y };
		}

		void solidify(bool board[10][20]) {
			board[pos.x][pos.y] = true;
		}

		olc::vi2d pos;

	};

	struct tetromino {
		tetromino() {
			switch (shape) {
			case(1):
				blocks[0]->pos = {5, 19};
				blocks[1]->pos = {5, 18};
				blocks[2]->pos = {5, 17};
				blocks[3]->pos = {4, 17};
				break;
			case(2):
				blocks[0]->pos = { 4, 19 };
				blocks[1]->pos = { 4, 18 };
				blocks[2]->pos = { 4, 17 };
				blocks[3]->pos = { 5, 17 };
				break;
			case(3):
				blocks[0]->pos = { 5, 19 };
				blocks[1]->pos = { 5, 18 };
				blocks[2]->pos = { 4, 19 };
				blocks[3]->pos = { 4, 18 };
				break;
			case(4):
				blocks[0]->pos = { 4, 19 };
				blocks[1]->pos = { 5, 19 };
				blocks[2]->pos = { 5, 18 };
				blocks[3]->pos = { 6, 18 };
				break;
			case(5):
				blocks[0]->pos = { 6, 19 };
				blocks[1]->pos = { 5, 19 };
				blocks[2]->pos = { 5, 18 };
				blocks[3]->pos = { 4, 18 };
				break;
			case(6):
				blocks[0]->pos = { 4, 18 };
				blocks[1]->pos = { 5, 19 };
				blocks[2]->pos = { 5, 18 };
				blocks[3]->pos = { 6, 18 };
			}
		}
		int shape = rand() % 7;
		block* blocks[4] = { new block(5, 19), new block(5, 18), new block(5, 17), new block(5, 16) };

		void Advance() {
			for (int i = 0; i < 4; i++) {
				blocks[i]->pos.y--;
			}
		}

		void Right() {
			for (int i = 0; i < 4; i++) {
				blocks[i]->pos.x++;
			}
		}

		void Left() {
			for (int i = 0; i < 4; i++) {
				blocks[i]->pos.x--;
			}
		}

		void Solidify(bool board[10][20]) {
			for (int i = 0; i < 4; i++) {
				blocks[i]->solidify(board);
			}
		}

		void Rotate(bool board[10][20]) {
			
		}

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

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		//-------------- Gameplay Logic --------------

		if (GameTimer >= 1 && !GameOver) {
			GameTimer = 0.0f;

			// Lower active tetromino & do collision
			bool solidify = false;
			for (int i = 0; i < 4; i++) {
				if (Active->blocks[i]->pos.y == 0 || board[Active->blocks[i]->pos.x][Active->blocks[i]->pos.y - 1])
					solidify = true;
			}
			if (solidify) {
				Active->Solidify(board);
				Active = new tetromino;
			}
			else
				Active->Advance();


			BoardTimer++;
		}

		// User input
		if (!GameOver) {
			if (GetKey(olc::D).bPressed || GetKey(olc::RIGHT).bPressed) {
				bool blocked = false;
				for (int i = 0; i < 4; i++) {
					if (Active->blocks[i]->pos.x == 19)
						blocked = true;
					else if (board[Active->blocks[i]->pos.x + 1][Active->blocks[i]->pos.y])
						blocked = true;
				}
				if (!blocked)
					Active->Right();
			}
			if (GetKey(olc::A).bPressed || GetKey(olc::LEFT).bPressed) {
				bool blocked = false;
				for (int i = 0; i < 4; i++) {
					if (Active->blocks[i]->pos.x == 0)
						blocked = true;
					else if (board[Active->blocks[i]->pos.x - 1][Active->blocks[i]->pos.y])
						blocked = true;
				}
				if (!blocked)
					Active->Left();
			}
			if (GetKey(olc::S).bPressed || GetKey(olc::DOWN).bPressed) {
				bool solidify = false;
				for (int i = 0; i < 4; i++) {
					if (Active->blocks[i]->pos.y == 0 || board[Active->blocks[i]->pos.x][Active->blocks[i]->pos.y - 1])
						solidify = true;
				}
				if (solidify) {
					Active->Solidify(board);
					Active = new tetromino;
				}
				else
					Active->Advance();
			}
			if (GetKey(olc::W).bPressed || GetKey(olc::UP).bPressed) {
				Active->Rotate(board);
			}
		}

		// Game Over debug
		if (GetKey(olc::SPACE).bPressed)
			GameOver = true;

		// Clear filled rows
		for (int y = 0; y < 20; y++) {
			bool filled = true;
			for (int x = 0; x < 10; x++) {
				if (!board[x][y])
					filled = false;
			}
			if (filled) {
				Score += 10;

				for (int y2 = y; y2 < 20; y2++) {
					if (y2 == 19) {
						for (int x = 0; x < 10; x++)
							board[x][19] = false;
					}
					else {
						for (int x = 0; x < 10; x++) {
							board[x][y2] = board[x][y2 + 1];
						}
					}
				}
			}
		}

		// Game over when blocks reach top
		for (int x = 0; x < 10; x++) {
			if (board[x][19])
				GameOver = true;
		}

		//----------------- Graphics -----------------

		// Draw the board
		FillRect({ 0, 0 }, { ScreenWidth(), ScreenHeight() }, olc::BLACK);
		FillRect({ BorderX - 3, BorderY - 3 }, { BoardWidth + 6, BoardHeight + 6 }, olc::BLUE);
		FillRect({ BorderX, BorderY }, { BoardWidth, BoardHeight }, olc::DARK_GREY);

		// Draw the pieces & active tetromino
		
		for (int x = 0; x < 10; x++) {
			for (int y = 0; y < 20; y++) {
				if (board[x][y]) {
					FillRect({ 30 * x + BorderX, 30 * -y + BorderY + BoardHeight - 30 }, { 30, 30 }, olc::GREEN);
				}
			}
		}
		if (Active != nullptr) {
			for (int i = 0; i < 4; i++) {
				FillRect({ 30 * Active->blocks[i]->pos.x + BorderX, 30 * -Active->blocks[i]->pos.y + BorderY + BoardHeight - 30 }, { 30, 30 }, olc::RED);
			}
		}

		// Draw game timer & score
		DrawString({ 30, 15 }, std::to_string(BoardTimer), olc::GREY, 2);
		DrawString({ 30, 35 }, std::to_string(Score), olc::GREY, 2);

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