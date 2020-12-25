#pragma once
#include "SlidePuzzleSceneNode.h"


class GameSlidingPuzzle
{

public:
	enum class MouseMode {Click,Drag};
	GameSlidingPuzzle(SceneNode* pieces, int emptyPos);
	void handleKeyboardInput(int key, int action);
	void handleMouseDrag(float xpos, float ypos, int action);
	void handleMouseClick(float xpos, float ypos, int index);
	void setMouseMode(MouseMode);
	void releasePiece();
private:
	MouseMode mouseMode;
	enum class MouseMoveDir {Up,Down,Left,Right,None};
	MouseMoveDir mouseMoveDir;
	map<int, int> stencilToGameIndex; //First is the stencilIndex, second is the game index
	SlidePuzzleSceneNode* selectedPiece;
	int selectedPieceGameIndex;
	int selectedPieceStencilIndex;
	bool was_pressed = false;
	bool firstMouse = true;
	float lastX = 0;
	float lastY = 0;
	vector<SlidePuzzleSceneNode*> pieces;
	int totalSlots;
	int emptyPos;
	SlidePuzzleSceneNode* getRightPiece();
	SlidePuzzleSceneNode* getLeftPiece();
	SlidePuzzleSceneNode* getUpPiece();
	SlidePuzzleSceneNode* getDownPiece();
	void movePiece(Vector3d translation,SlidePuzzleSceneNode* piece);
	void setNewMouseMoveDir(int gamePieceIndex);
};

