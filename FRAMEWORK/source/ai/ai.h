#pragma once
#include "../board/board.h"

#include <string>

constexpr double AI_COUNT_WEIGHT = 2;
constexpr double AI_BLOCK_WEIGHT = 1;
constexpr double AI_MAX_SCORE = AI_COUNT_WEIGHT * ((double)BOARD_GOAL - 1) * 8 + AI_BLOCK_WEIGHT * ((double)BOARD_GOAL - 1) * 8 + 1;

struct Node {
	using NodePtr = std::shared_ptr<Node>;

	BoardTiles tile;
	Board board;
	unsigned int col;
	unsigned int row;
	double score;
	std::vector<NodePtr> children;
};

Node::NodePtr aiGetMoveGraph(const Board& board, BoardTiles tile, unsigned int col, unsigned int depth);
void aiPrintMoveGraphToFile(Node::NodePtr graph, const std::string& fileName);