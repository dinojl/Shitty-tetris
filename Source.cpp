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

		void solidify(bool board[10][20], olc::vi2d offset) {
			board[pos.x + offset.x][pos.y + offset.y] = true;
		}

		olc::vi2d pos;

	};

	struct tetromino {
		tetromino() {
			switch (shape) {
			case(1):
				blocks[0]->pos = { 0, 2 };
				blocks[1]->pos = { 0, 1 };
				blocks[2]->pos = { 0, 0 };
				blocks[3]->pos = { -1, 0 };
				break;
			case(2):
				blocks[0]->pos = { -1, 2 };
				blocks[1]->pos = { -1, 1 };
				blocks[2]->pos = { -1, 0 };
				blocks[3]->pos = { 0, 0 };
				break;
			case(3):
				blocks[0]->pos = { 0, 1 };
				blocks[1]->pos = { 0, 0 };
				blocks[2]->pos = { -1, 1 };
				blocks[3]->pos = { -1, 0 };
				break;
			case(4):
				blocks[0]->pos = { -1, 1 };
				blocks[1]->pos = { 0, 1 };
				blocks[2]->pos = { 0, 0 };
				blocks[3]->pos = { 1, 0 };
				break;
			case(5):
				blocks[0]->pos = { 1, 1 };
				blocks[1]->pos = { 0, 1 };
				blocks[2]->pos = { 0, 0 };
				blocks[3]->pos = { -1, 0 };
				break;
			case(6):
				blocks[0]->pos = { -1, 0 };
				blocks[1]->pos = { 0, 1 };
				blocks[2]->pos = { 0, 0 };
				blocks[3]->pos = { 1, 0 };
			}
		}
		int shape = rand() % 7;
		block* blocks[4] = { new block(0, 2), new block(0, 1), new block(0, 0), new block(0, -1) };
		olc::vi2d pos = { 5, 17 };

		int GetPosX(int block) {
			return blocks[block]->pos.x + pos.x;
		}

		int GetPosY(int block) {
			return blocks[block]->pos.y + pos.y;
		}

		void Advance() {
			pos.y--;
		}

		void Right() {
			pos.x++;
		}

		void Left() {
			pos.x--;
		}

		void Solidify(bool board[10][20]) {
			for (int i = 0; i < 4; i++) {
				blocks[i]->solidify(board, pos);
			}
		}

		void Rotate(bool board[10][20]) {
			bool blocked = false;
			for (int i = 0; i < 4; i++) {
				int x = blocks[i]->pos.x;
				int y = blocks[i]->pos.y;

				blocks[i]->pos = {-y, x};
			}

			for (int i = 0; i < 4; i++) {
				blocked = board[GetPosX(i)][GetPosY(i)];
				if (GetPosX(i) < 0 || GetPosX(i) > 9)
					blocked = true;
				if (GetPosY(i) < 0 || GetPosY(i) > 19)
					blocked = true;
			}

			if (blocked) {
				for (int i = 0; i < 4; i++) {
					int x = blocks[i]->pos.x;
					int y = blocks[i]->pos.y;

					blocks[i]->pos = { y, -x };
				}
			}
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
				if (Active->GetPosY(i) == 0 || board[Active->GetPosX(i)][Active->GetPosY(i) - 1])
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
					if (Active->GetPosX(i) == 19)
						blocked = true;
					else if (board[Active->GetPosX(i) + 1][Active->GetPosY(i)])
						blocked = true;
				}
				if (!blocked)
					Active->Right();
			}
			if (GetKey(olc::A).bPressed || GetKey(olc::LEFT).bPressed) {
				bool blocked = false;
				for (int i = 0; i < 4; i++) {
					if (Active->GetPosX(i) == 0)
						blocked = true;
					else if (board[Active->GetPosX(i) - 1][Active->GetPosY(i)])
						blocked = true;
				}
				if (!blocked)
					Active->Left();
			}
			if (GetKey(olc::S).bPressed || GetKey(olc::DOWN).bPressed) {
				bool solidify = false;
				for (int i = 0; i < 4; i++) {
					if (Active->GetPosY(i) == 0 || board[Active->GetPosX(i)][Active->GetPosY(i) - 1])
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
				FillRect({ 30 * Active->GetPosX(i) + BorderX, 30 * -Active->GetPosY(i) + BorderY + BoardHeight - 30 }, { 30, 30 }, olc::RED);
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