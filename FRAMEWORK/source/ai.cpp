#include "../includes/ai.h"
#include "../includes/ai_maths.h"

#include <fstream>
#include <queue>
#include <map>
#include <unordered_map>
#include <iostream>
#include <algorithm>

double aiScoreMove(const Board& board, BoardTiles tile, unsigned int col) {
	MoveResult move = boardTestMove(board, col);
	if (!move.first) {
		return 0;
	}

	WinCheck right = boardCheckRightWin(board, tile, move.second, col);
	WinCheck left = boardCheckLeftWin(board, tile, move.second, col);
	WinCheck down = boardCheckDownWin(board, tile, move.second, col);
	WinCheck up = boardCheckUpWin(board, tile, move.second, col);
	WinCheck downRight = boardCheckDownRightWin(board, tile, move.second, col);
	WinCheck downLeft = boardCheckDownLeftWin(board, tile, move.second, col);
	WinCheck upRight = boardCheckUpRightWin(board, tile, move.second, col);
	WinCheck upLeft = boardCheckUpLeftWin(board, tile, move.second, col);

	if (right.first || down.first || downRight.first || upRight.first ||
		left.first || up.first || downLeft.first || upLeft.first) {
		return AI_MAX_SCORE;
	}

	BoardTiles otherPlayer = tile == PLAYER_ONE ? PLAYER_TWO : PLAYER_ONE;
	WinCheck rightBlock = boardCheckRightBlock(board, otherPlayer, move.second, col);
	WinCheck leftBlock = boardCheckLeftBlock(board, otherPlayer, move.second, col);
	WinCheck downBlock = boardCheckDownBlock(board, otherPlayer, move.second, col);
	WinCheck upBlock = boardCheckUpBlock(board, otherPlayer, move.second, col);
	WinCheck downRightBlock = boardCheckDownRightBlock(board, otherPlayer, move.second, col);
	WinCheck downLeftBlock = boardCheckDownLeftBlock(board, otherPlayer, move.second, col);
	WinCheck upRightBlock = boardCheckUpRightBlock(board, otherPlayer, move.second, col);
	WinCheck upLeftBlock = boardCheckUpLeftBlock(board, otherPlayer, move.second, col);

	if (rightBlock.first || downBlock.first || downRightBlock.first || upRightBlock.first ||
		leftBlock.first || upBlock.first || downLeftBlock.first || upLeftBlock.first) {
		return AI_MAX_SCORE - 1;
	}

	double score = 0;
	score += right.second * AI_COUNT_WEIGHT;
	score += left.second * AI_COUNT_WEIGHT;
	score += down.second * AI_COUNT_WEIGHT;
	score += up.second * AI_COUNT_WEIGHT;
	score += downRight.second * AI_COUNT_WEIGHT;
	score += downLeft.second * AI_COUNT_WEIGHT;
	score += upRight.second * AI_COUNT_WEIGHT;
	score += upLeft.second * AI_COUNT_WEIGHT;
	score += rightBlock.second * AI_BLOCK_WEIGHT;
	score += leftBlock.second * AI_BLOCK_WEIGHT;
	score += downBlock.second * AI_BLOCK_WEIGHT;
	score += upBlock.second * AI_BLOCK_WEIGHT;
	score += downRightBlock.second * AI_BLOCK_WEIGHT;
	score += downLeftBlock.second * AI_BLOCK_WEIGHT;
	score += upRightBlock.second * AI_BLOCK_WEIGHT;
	score += upLeftBlock.second * AI_BLOCK_WEIGHT;

	return score;
}

Node::NodePtr aiNodeCreate(const Board& board, BoardTiles tile, unsigned int col) {
	Node::NodePtr node = std::make_shared<Node>();
	node->tile = tile;
	node->board = board;
	boardMakeMove(node->board, col, tile);
	node->col = col;
	node->row = boardTestMove(board, col).second;
	node->score = aiScoreMove(board, tile, col);
	return node;
}

std::string aiHashBoard(const Board& board, BoardTiles tile) {
	std::string hash;
	for (auto const& row : board) {
		for (auto const& tile : row) {
			switch (tile) {
			case PLAYER_ONE:
				hash.push_back(BOARD_PLAYER_ONE);
			break;
			case PLAYER_TWO:
				hash.push_back(BOARD_PLAYER_TWO);
			break;
			case EMPTY:
				hash.push_back(BOARD_EMPTY_TILE);
			break;
			}
		}
	}
	hash.push_back((char)tile);
	return hash;
}

Node::NodePtr aiGetMoveGraph(const Board& board, BoardTiles tile, unsigned int col, unsigned int depth) {
	std::queue<Node::NodePtr> nodeQueue;
	Node::NodePtr root = aiNodeCreate(board, tile, col);
	nodeQueue.push(root);

	unsigned int currentDepth = 1;
	bool setDepthIncreaseNode = true;
	Node::NodePtr nodeToIncreaseDepthOn = nullptr;

	std::unordered_map<std::string, bool> searchedPaths;
	while (!nodeQueue.empty() && currentDepth < depth) {
		Node::NodePtr currentNode = nodeQueue.front();
		if (currentNode->score == -1) {
			return root;
		}

		nodeQueue.pop();
		if (nodeToIncreaseDepthOn == currentNode) {
			setDepthIncreaseNode = true;
		}

		Moves moves = boardGetValidMoves(currentNode->board);
		Moves filteredMoves;
		BoardTiles nextTile = currentNode->tile == PLAYER_ONE ? PLAYER_TWO : PLAYER_ONE;
		if (nextTile == PLAYER_TWO) {
			std::vector<double> scores;
			for (auto const& move : moves) {
				scores.push_back(aiScoreMove(currentNode->board, nextTile, move.second));
			}

			double bestScore = *std::max_element(scores.begin(), scores.end());
			double deviation = standardDeviation(scores);

			for (unsigned int i = 0; i < moves.size(); i++) {
				if (withinRange(scores[i], bestScore - deviation, bestScore + deviation)) {
					filteredMoves.push_back(moves[i]);
				}
			}
		} else {
			filteredMoves = moves;
		}
		
		for (unsigned int i = 0; std::pair<unsigned int, unsigned int> const move : filteredMoves) {
			Board tmpBoard = currentNode->board;
			Node::NodePtr nextNode = aiNodeCreate(tmpBoard, nextTile, move.first);
			std::string nextNodeHash = aiHashBoard(nextNode->board, nextTile);
			if (!searchedPaths.contains(nextNodeHash)) {
				searchedPaths[nextNodeHash] = true;
			} else {
				continue;
			}
		
			if (nextNode->score != AI_MAX_SCORE) {
				nodeQueue.push(nextNode);
			}
			currentNode->children.push_back(nextNode);
		
			if (i == filteredMoves.size() - 1 && setDepthIncreaseNode) {
				currentDepth++;
				nodeToIncreaseDepthOn = nextNode;
				setDepthIncreaseNode = false;
			}
			i++;
		}
	}
	return root;
}

void aiSegmentGraphByDepth(std::map<unsigned int, std::vector<Board>>& depths, const Node::NodePtr node, unsigned int depth, std::ostream& file) {
	depths[depth].push_back(node->board);
	depth++;
	for (auto const& child : node->children) {
		aiSegmentGraphByDepth(depths, child, depth, file);
	}
}

void aiPrintMoveGraphToFile(const Node::NodePtr graph, const std::string& fileName) {
	std::ofstream file(fileName);
	if (!file.is_open()) {
		return;
	}

	std::map<unsigned int, std::vector<Board>> segmentedGraph;
	aiSegmentGraphByDepth(segmentedGraph, graph, 1, file);

	for (unsigned int depth = 1; auto const& vector : segmentedGraph) {
		for (auto const& board : vector.second) {
			file << "depth: " << depth << '\n';
			for (auto const& row : board) {
				for (auto const& tile : row) {
					switch (tile) {
					case PLAYER_ONE:
						file << BOARD_PLAYER_ONE << ' ';
						break;
					case PLAYER_TWO:
						file << BOARD_PLAYER_TWO << ' ';
						break;
					case EMPTY:
						file << BOARD_EMPTY_TILE << ' ';
						break;
					}
				}
				file << '\n';
			}
			file << '\n';
		}
		depth++;
	}
	file.close();
}