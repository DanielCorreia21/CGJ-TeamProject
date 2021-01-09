#pragma once
#include "GameSlidingPuzzle.h"

class SlidePuzzleGameFileHandler
{
public:
	static SlidePuzzleGameFileHandler* instance;
	static SlidePuzzleGameFileHandler* getInstance();
	void saveGame(GameSlidingPuzzle* game);
	void loadGame(GameSlidingPuzzle* game);
	vector<int> piecesPositions;
private:
	SlidePuzzleGameFileHandler();
	void parseLine(string line);
	vector<string> split(const string& s);
	int emptyPos = -1;
	GameSlidingPuzzle::MouseMode mouseMode = GameSlidingPuzzle::MouseMode::Drag;
};

