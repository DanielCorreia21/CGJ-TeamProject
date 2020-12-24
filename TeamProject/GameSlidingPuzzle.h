#pragma once
#include "SceneNode.h"
class GameSlidingPuzzle
{

public:
	GameSlidingPuzzle(SceneNode* pieces, int emptyPos);
	void handleInput(int key, int action);
private:
	vector<SceneNode*> pieces;
	int totalSlots;
	int emptyPos;
	SceneNode* getRightPiece();
	SceneNode* getLeftPiece();
	SceneNode* getUpPiece();
	SceneNode* getDownPiece();
	void movePiece(Vector3d translation,SceneNode* piece);
};

