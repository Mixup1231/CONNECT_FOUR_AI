#pragma once
#include <array>
#include <memory>
#include <vector>

constexpr int BOARD_GOAL = 4;
constexpr int BOARD_ROWS = 6;
constexpr int BOARD_COLS = 7;

const char BOARD_PLAYER_ONE = 'X';
const char BOARD_PLAYER_TWO = 'O';
const char BOARD_EMPTY_TILE = '\'';

enum BoardTiles {
	PLAYER_ONE = 0,
	PLAYER_TWO,
	EMPTY
};

using Board = std::array<std::array<BoardTiles, BOARD_COLS>, BOARD_ROWS>;
using MoveResult = std::pair<bool, unsigned int>;
using WinCheck = std::pair<bool, unsigned int>;
using Moves = std::vector<std::pair<unsigned int, unsigned int>>;

Board boardCreate(void);
void boardPrint(const Board& board);
MoveResult boardTestMove(const Board& board, unsigned int col);
bool boardMakeMove(Board& board, unsigned int col, BoardTiles tile);
Moves boardGetValidMoves(const Board& board);
WinCheck boardCheckRightWin(const Board& board, BoardTiles tile, unsigned int row, unsigned col);
WinCheck boardCheckLeftWin(const Board& board, BoardTiles tile, unsigned int row, unsigned col);
WinCheck boardCheckDownWin(const Board& board, BoardTiles tile, unsigned int row, unsigned col);
WinCheck boardCheckUpWin(const Board& board, BoardTiles tile, unsigned int row, unsigned col);
WinCheck boardCheckDownRightWin(const Board& board, BoardTiles tile, unsigned int row, unsigned col);
WinCheck boardCheckDownLeftWin(const Board& board, BoardTiles tile, unsigned int row, unsigned col);
WinCheck boardCheckUpRightWin(const Board& board, BoardTiles tile, unsigned int row, unsigned col);
WinCheck boardCheckUpLeftWin(const Board& board, BoardTiles tile, unsigned int row, unsigned col);
WinCheck boardCheckRightBlock(const Board& board, BoardTiles tile, unsigned int row, unsigned col);
WinCheck boardCheckLeftBlock(const Board& board, BoardTiles tile, unsigned int row, unsigned col);
WinCheck boardCheckDownBlock(const Board& board, BoardTiles tile, unsigned int row, unsigned col);
WinCheck boardCheckUpBlock(const Board& board, BoardTiles tile, unsigned int row, unsigned col);
WinCheck boardCheckDownRightBlock(const Board& board, BoardTiles tile, unsigned int row, unsigned col);
WinCheck boardCheckDownLeftBlock(const Board& board, BoardTiles tile, unsigned int row, unsigned col);
WinCheck boardCheckUpRightBlock(const Board& board, BoardTiles tile, unsigned int row, unsigned col);
WinCheck boardCheckUpLeftBlock(const Board& board, BoardTiles tile, unsigned int row, unsigned col);
BoardTiles boardCheckWin(const Board& board);