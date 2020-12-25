#pragma once
#include "SceneNode.h"


class GameSlidingPuzzle
{

public:
	GameSlidingPuzzle(SceneNode* pieces, int emptyPos);
	void handleKeyboardInput(int key, int action);
	void handleMouseDrag(float xpos, float ypos, int action);
	void handleMouseClick(float xpos, float ypos, int index);
private:
	enum class MouseMoveDirection {
		None,
		Up,
		Down,
		Left,
		Right
	};
	SceneNode* getMouseMoveDirection(int pieceIndex);
	vector<SceneNode*> pieces;
	int totalSlots;
	int emptyPos;
	SceneNode* getRightPiece();
	SceneNode* getLeftPiece();
	SceneNode* getUpPiece();
	SceneNode* getDownPiece();
	void movePiece(Vector3d translation,SceneNode* piece);
};

