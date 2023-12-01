#include <iostream>
#include <time.h>
#include "includes/board.h"
#include "includes/ai.h"

int main(int argc, char* argv[]) {
	Board test = boardCreate();

	clock_t start = clock();
	Node::NodePtr testMoveGraph = aiGetMoveGraph(test, PLAYER_ONE, 0, 10);
	std::cout << ((float)(clock() - start) / CLOCKS_PER_SEC) << "ms\n";

	aiPrintMoveGraphToFile(testMoveGraph, "test.txt");
}