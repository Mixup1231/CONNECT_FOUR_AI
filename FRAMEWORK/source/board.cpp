#include "board/board.h"

#include <iostream>
#include <cassert>
#include <vector>
#include <queue>

Board boardCreate(void) {
	Board board{};

	for (auto& row : board) {
		for (auto& tile : row) {
			tile = EMPTY;
		}
	}
	return board;
}

void boardPrint(const Board& board) {
	for (auto const& row : board) {
		for (auto const& tile : row) {
			switch (tile) {
			case PLAYER_ONE:
				std::cout << BOARD_PLAYER_ONE << ' ';
				break;
			case PLAYER_TWO:
				std::cout << BOARD_PLAYER_TWO << ' ';
				break;
			case EMPTY:
				std::cout << BOARD_EMPTY_TILE << ' ';
				break;
			}
		}
		std::cout << '\n';
	}
}

MoveResult boardTestMove(const Board& board, unsigned int col) {
	assert(0 <= col && col < BOARD_COLS);

	for (int i = BOARD_ROWS - 1; i >= 0; i--) {
		if (board[i][col] == EMPTY) {
			return { true, i };
		}
	}
	return { false, 0 };
}

bool boardMakeMove(Board& board, unsigned int col, BoardTiles tile) {
	MoveResult move = boardTestMove(board, col);
	if (move.first) {
		board[move.second][col] = tile;
	}
	return move.first;
}

WinCheck boardCheckRightWin(const Board& board, BoardTiles tile, unsigned int row, unsigned col) {
	assert(0 <= row && row < BOARD_ROWS && 0 <= col && col < BOARD_COLS);

	unsigned int count = 1;

	for (int j = col + 1; j < BOARD_COLS; j++) {
		if (count == BOARD_GOAL) {
			return { true, count };
		} else if (board[row][j] == tile) {
			count++;
		} else {
			return { false, count };
		}
	}
	return { false, count };
}

WinCheck boardCheckRightBlock(const Board& board, BoardTiles tile, unsigned int row, unsigned int col) {
	WinCheck tileCount = boardCheckRightWin(board, tile == PLAYER_ONE ? PLAYER_TWO : PLAYER_ONE, row, col);
	tileCount.second--;
	return tileCount;
}

WinCheck boardCheckLeftWin(const Board& board, BoardTiles tile, unsigned int row, unsigned col) {
	assert(0 <= row && row < BOARD_ROWS && 0 <= col && col < BOARD_COLS);

	unsigned int count = 1;

	for (int j = col - 1; j >= 0; j--) {
		if (count == BOARD_GOAL) {
			return { true, count };
		}
		else if (board[row][j] == tile) {
			count++;
		}
		else {
			return { false, count };
		}
	}
	return { false, count };
}

WinCheck boardCheckLeftBlock(const Board& board, BoardTiles tile, unsigned int row, unsigned int col) {
	WinCheck tileCount = boardCheckLeftWin(board, tile == PLAYER_ONE ? PLAYER_TWO : PLAYER_ONE, row, col);
	tileCount.second--;
	return tileCount;
}

WinCheck boardCheckDownWin(const Board& board, BoardTiles tile, unsigned int row, unsigned col) {
	assert(0 <= row && row < BOARD_ROWS && 0 <= col && col < BOARD_COLS);

	unsigned int count = 1;
	
	for (int i = row + 1; i < BOARD_ROWS; i++) {
		if (count == BOARD_GOAL) {
			return { true, count };
		} else if (board[i][col] == tile) {
			count++;
		} else {
			return { false, count };
		}
	}
	return { false, count };
}

WinCheck boardCheckDownBlock(const Board& board, BoardTiles tile, unsigned int row, unsigned int col) {
	WinCheck tileCount = boardCheckDownWin(board, tile == PLAYER_ONE ? PLAYER_TWO : PLAYER_ONE, row, col);
	tileCount.second--;
	return tileCount;
}

WinCheck boardCheckUpWin(const Board& board, BoardTiles tile, unsigned int row, unsigned col) {
	assert(0 <= row && row < BOARD_ROWS && 0 <= col && col < BOARD_COLS);

	unsigned int count = 1;

	for (int i = row - 1; i >= 0; i--) {
		if (count == BOARD_GOAL) {
			return { true, count };
		} else if (board[i][col] == tile) {
			count++;
		} else {
			return { false, count };
		}
	}
	return { false, count };
}

WinCheck boardCheckUpBlock(const Board& board, BoardTiles tile, unsigned int row, unsigned int col) {
	WinCheck tileCount = boardCheckUpWin(board, tile == PLAYER_ONE ? PLAYER_TWO : PLAYER_ONE, row, col);
	tileCount.second--;
	return tileCount;
}

WinCheck boardCheckDownRightWin(const Board& board, BoardTiles tile, unsigned int row, unsigned col) {
	assert(0 <= row && row < BOARD_ROWS && 0 <= col && col < BOARD_COLS);

	unsigned int count = 1;
	int i = row + 1;
	int j = col + 1;

	for (unsigned int k = 0; k < BOARD_GOAL; k++) {
		if (i <= 0 || j <= 0 || i >= BOARD_ROWS || j >= BOARD_COLS) {
			return { false, count };
		} else if (count == BOARD_GOAL) {
			return { true, count };
		} else if (board[i][j] == tile) {
			count++;
		} else {
			return { false, count };
		}

		i++;
		j++;
	}
	return { false, count };
}

WinCheck boardCheckDownRightBlock(const Board& board, BoardTiles tile, unsigned int row, unsigned int col) {
	WinCheck tileCount = boardCheckDownRightWin(board, tile == PLAYER_ONE ? PLAYER_TWO : PLAYER_ONE, row, col);
	tileCount.second--;
	return tileCount;
}

WinCheck boardCheckDownLeftWin(const Board& board, BoardTiles tile, unsigned int row, unsigned col) {
	assert(0 <= row && row < BOARD_ROWS && 0 <= col && col < BOARD_COLS);

	unsigned int count = 1;
	int i = row + 1;
	int j = col - 1;

	for (unsigned int k = 0; k < BOARD_GOAL; k++) {
		if (i <= 0 || j <= 0 || i >= BOARD_ROWS || j >= BOARD_COLS) {
			return { false, count };
		} else if (count == BOARD_GOAL) {
			return { true, count };
		} else if (board[i][j] == tile) {
			count++;
		} else {
			return { false, count };
		}

		i++;
		j--;
	}
	return { false, count };
}

WinCheck boardCheckDownLeftBlock(const Board& board, BoardTiles tile, unsigned int row, unsigned int col) {
	WinCheck tileCount = boardCheckDownLeftWin(board, tile == PLAYER_ONE ? PLAYER_TWO : PLAYER_ONE, row, col);
	tileCount.second--;
	return tileCount;
}

WinCheck boardCheckUpRightWin(const Board& board, BoardTiles tile, unsigned int row, unsigned col) {
	assert(0 <= row && row < BOARD_ROWS && 0 <= col && col < BOARD_COLS);

	unsigned int count = 1;
	int i = row - 1;
	int j = col + 1;

	for (unsigned int k = 0; k < BOARD_GOAL; k++) {
		if (i <= 0 || j <= 0 || i >= BOARD_ROWS || j >= BOARD_COLS) {
			return { false, count };
		} else if (count == BOARD_GOAL) {
			return { true, count };
		} else if (board[i][j] == tile) {
			count++;
		} else {
			return { false, count };
		}

		i--;
		j++;
	}
	return { false, count };
}

WinCheck boardCheckUpRightBlock(const Board& board, BoardTiles tile, unsigned int row, unsigned int col) {
	WinCheck tileCount = boardCheckUpRightWin(board, tile == PLAYER_ONE ? PLAYER_TWO : PLAYER_ONE, row, col);
	tileCount.second--;
	return tileCount;
}

WinCheck boardCheckUpLeftWin(const Board& board, BoardTiles tile, unsigned int row, unsigned col) {
	assert(0 <= row && row < BOARD_ROWS && 0 <= col && col < BOARD_COLS);

	unsigned int count = 1;
	int i = row - 1;
	int j = col - 1;

	for (unsigned int k = 0; k < BOARD_GOAL; k++) {
		if (i <= 0 || j <= 0 || i >= BOARD_ROWS || j >= BOARD_COLS) {
			return { false, count };
		} else if (count == BOARD_GOAL) {
			return { true, count };
		} else if (board[i][j] == tile) {
			count++;
		} else {
			return { false, count };
		}

		i--;
		j--;
	}
	return { false, count };
}

WinCheck boardCheckUpLeftBlock(const Board& board, BoardTiles tile, unsigned int row, unsigned int col) {
	WinCheck tileCount = boardCheckUpLeftWin(board, tile == PLAYER_ONE ? PLAYER_TWO : PLAYER_ONE, row, col);
	tileCount.second--;
	return tileCount;
}

BoardTiles boardCheckWin(const Board& board) {
	for (int i = 0; i < BOARD_ROWS; i++) {
		for (int j = 0; j < BOARD_COLS; j++) {
			BoardTiles currentTile = board[i][j];
			if (currentTile == EMPTY) {
				continue;
			} else if (boardCheckRightWin(board, currentTile, i, j).first) {
				return currentTile;
			} else if (boardCheckDownWin(board, currentTile, i, j).first) {
				return currentTile;
			} else if (boardCheckDownRightWin(board, currentTile, i, j).first) {
				return currentTile;
			} else if (boardCheckUpRightWin(board, currentTile, i, j).first) {
				return currentTile;
			}
		}
	}
	return EMPTY;
}

Moves boardGetValidMoves(const Board& board) {
	Moves moves;

	for (int i = 0; i < BOARD_COLS; i++) {
		MoveResult move = boardTestMove(board, i);
		if (move.first) {
			moves.push_back({ i, move.second });
		}
	}
	return moves;
}