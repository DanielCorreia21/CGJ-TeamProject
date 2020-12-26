#pragma once
#include "SlidePuzzleSceneNode.h"


class GameSlidingPuzzle
{

public:
	GameSlidingPuzzle(SceneNode* pieces, int emptyPos);
	enum class MouseMode {Click,Drag};
	void setMouseMode(MouseMode);
	void handleKeyboardInput(int key, int action);
	void handleMouseDrag(float xpos, float ypos, int action);
	void handleMouseClick(float xpos, float ypos, int index);
	void releasePiece();
private:
	#pragma region mouseVariables
	map<int, int> stencilToGameIndex; //First is the stencilIndex, second is the game index
	enum class MouseMoveDir {Up,Down,Left,Right,None};
	MouseMode mouseMode;
	MouseMoveDir mouseMoveDir;
	Matrix4d savedMatrix;
	SlidePuzzleSceneNode* selectedPiece;
	int selectedPieceGameIndex;
	int selectedPieceStencilIndex;
	float baseMovementValue = 0.008f;
	float totalMoved = 0;
	float lastX = 0;
	float lastY = 0;
	#pragma endregion
	vector<SlidePuzzleSceneNode*> pieces;
	int emptyPos;
	void simpleMouseMove(int pieceIndex, int gamePieceIndex);
	void initMouseDrag(int xpos, int ypos, int pieceIndex, int gamePieceIndex);
	void setNewMouseMoveDir(int gamePieceIndex);
	SlidePuzzleSceneNode* getRightPiece();
	SlidePuzzleSceneNode* getLeftPiece();
	SlidePuzzleSceneNode* getUpPiece();
	SlidePuzzleSceneNode* getDownPiece();
	void movePiece(Vector3d translation,SlidePuzzleSceneNode* piece);
};

