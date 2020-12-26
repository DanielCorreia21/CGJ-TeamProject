#include "GameSlidingPuzzle.h"
#include "MatrixFactory.h"

#define MOVE_AMOUNT 0.4f
#define MOVE_UP Vector3d(0.0f, MOVE_AMOUNT, 0.0f)
#define MOVE_DOWN Vector3d(0.0f, -MOVE_AMOUNT, 0.0f)
#define MOVE_LEFT Vector3d(-MOVE_AMOUNT, 0.0f, 0.0f)
#define MOVE_RIGHT Vector3d(MOVE_AMOUNT, 0.0f, 0.0f)

/*
*@requires piecesRoot to have its children by the winning order.
* TODO: The pieces will then be scrambled
* 
*/
GameSlidingPuzzle::GameSlidingPuzzle(SceneNode* piecesRoot, int pos)
{
	this->emptyPos = pos;

	vector<SlidePuzzleSceneNode*>* v = new vector<SlidePuzzleSceneNode*>();
	vector<SceneNode*> auxPieces = piecesRoot->getChildren();
	for (int i = 0; i < auxPieces.size() + 1; i++) {
		if (i != this->emptyPos) {
			v->push_back(((SlidePuzzleSceneNode*) auxPieces.at(i)));
			stencilToGameIndex.insert(pair<int, int>(i,i));
		}
		else {
			v->push_back(NULL);
		}
	}
	
	this->pieces = *v;
	this->totalSlots = this->pieces.size();
}

void GameSlidingPuzzle::handleKeyboardInput(int key, int action)
{
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		SlidePuzzleSceneNode* piece = getLeftPiece();
		movePiece(MOVE_RIGHT, piece);
	}
	else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		SlidePuzzleSceneNode* piece = getRightPiece();
		movePiece(MOVE_LEFT, piece);
	}
	else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		SlidePuzzleSceneNode* piece = getDownPiece();
		movePiece(MOVE_UP, piece);
	}
	else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		SlidePuzzleSceneNode* piece = getUpPiece();
		movePiece(MOVE_DOWN, piece);
	}
}


Matrix4d savedMatrix;
float totalMoved = 0;
float baseMovementValue = 0.008f;
float moveAmount;
void GameSlidingPuzzle::handleMouseDrag(float xpos, float ypos, int pressed)
{
	if (!pressed) {return;}
	if (this->selectedPiece == NULL) {
		return;
	}

	float xoffset = xpos - lastX;
	float yoffset = ypos - lastY;
	lastX = xpos;
	lastY = ypos;

	if (mouseMoveDir == MouseMoveDir::Right || mouseMoveDir == MouseMoveDir::Left) {
		moveAmount = baseMovementValue * xoffset/2; //trial and error...
	}
	else if (mouseMoveDir == MouseMoveDir::Up || mouseMoveDir == MouseMoveDir::Down) {
		moveAmount = -(baseMovementValue * yoffset/2); //trial and error...
	}

	if (mouseMoveDir == MouseMoveDir::Right) {
		//If the moveAmount would exceed the bounds, make moveAmount the value to get to the bounds.
		moveAmount = moveAmount > 0 && totalMoved + moveAmount > 0.4f ? 0.4f - totalMoved : moveAmount;
		moveAmount = moveAmount < 0 && totalMoved + moveAmount < 0 ? -totalMoved : moveAmount;
		selectedPiece->setMatrix(
			MatrixFactory::translationMatrix(Vector3d(moveAmount, 0.0f, 0.0f))
			* selectedPiece->getMatrix()
		);

	}
	else if (mouseMoveDir == MouseMoveDir::Left) {
		//If the moveAmount would exceed the bounds, make moveAmount the value to get to the bounds.
		moveAmount = moveAmount < 0 && totalMoved + moveAmount < -0.4f ? -0.4f - totalMoved : moveAmount;
		moveAmount = moveAmount > 0 && totalMoved + moveAmount > 0 ? -totalMoved : moveAmount;
		selectedPiece->setMatrix(
			MatrixFactory::translationMatrix(Vector3d(moveAmount, 0.0f, 0.0f))
			* selectedPiece->getMatrix()
		);

	} else if (mouseMoveDir == MouseMoveDir::Up) {
		//If the moveAmount would exceed the bounds, make moveAmount the value to get to the bounds.
		moveAmount = moveAmount > 0 && totalMoved + moveAmount > 0.4f ? 0.4f - totalMoved : moveAmount;
		moveAmount = moveAmount < 0 && totalMoved + moveAmount < 0 ? -totalMoved : moveAmount;
		
		selectedPiece->setMatrix(
			MatrixFactory::translationMatrix(Vector3d(0.0f, moveAmount, 0.0f))
			* selectedPiece->getMatrix()
		);

	} else if (mouseMoveDir == MouseMoveDir::Down) {
		//If the moveAmount would exceed the bounds, make moveAmount the value to get to the bounds.
		moveAmount = moveAmount < 0 && totalMoved + moveAmount < -0.4f ? -0.4f - totalMoved : moveAmount;
		moveAmount = moveAmount > 0 && totalMoved + moveAmount > 0 ? -totalMoved : moveAmount;
		selectedPiece->setMatrix(
			MatrixFactory::translationMatrix(Vector3d(0.0f, moveAmount, 0.0f))
			* selectedPiece->getMatrix()
		);
	}
	
	if(mouseMoveDir != MouseMoveDir::None) totalMoved += moveAmount;
}


void GameSlidingPuzzle::releasePiece()
{
	if (this->mouseMoveDir == MouseMoveDir::None || this->selectedPiece == NULL) return;
	
	bool positiveMoveNotDone =
		(this->mouseMoveDir == MouseMoveDir::Right ||this->mouseMoveDir == MouseMoveDir::Up) 
		&& totalMoved < 0.4f;
	bool negativeMoveNotDone =
		(this->mouseMoveDir == MouseMoveDir::Left || this->mouseMoveDir == MouseMoveDir::Down)
		&& totalMoved > -0.4f;

	if (selectedPiece != NULL && (positiveMoveNotDone || negativeMoveNotDone)) {
		selectedPiece->setMatrix(
			savedMatrix
		);
		this->selectedPiece = NULL;
	}
	else if(selectedPiece != NULL){
		switch (this->mouseMoveDir) {
			case(MouseMoveDir::Right):
				selectedPiece->setMatrix(
					MatrixFactory::translationMatrix(MOVE_RIGHT)
					* savedMatrix
				);
				break;
			case(MouseMoveDir::Left):
				selectedPiece->setMatrix(
					MatrixFactory::translationMatrix(MOVE_LEFT)
					* savedMatrix
				);
				break;
			case(MouseMoveDir::Up):
				selectedPiece->setMatrix(
					MatrixFactory::translationMatrix(MOVE_UP)
					* savedMatrix
				);
				break;
			case(MouseMoveDir::Down):
				selectedPiece->setMatrix(
					MatrixFactory::translationMatrix(MOVE_DOWN)
					* savedMatrix
				);
				break;
		}
		cout << "\\Release Piece// \n";
		cout << "selectedPieceStencilIndex : " << selectedPieceStencilIndex << "\n";
		cout << "emptyPos : " << emptyPos << "\n";
		std::swap(pieces[selectedPieceGameIndex], pieces[emptyPos]);
		stencilToGameIndex[selectedPieceStencilIndex] = this->emptyPos;
		this->emptyPos = selectedPieceGameIndex;
		this->selectedPiece = NULL;
	}
	totalMoved = 0;
}


void GameSlidingPuzzle::handleMouseClick(float xpos, float ypos, int pieceIndex)
{	
	//If we didn't click on a piece, we return
	if (pieceIndex <= 0 || pieceIndex > this->pieces.size()) {
		this->selectedPiece == NULL;
		cout << "NULL piece\n";
		return;
	}

	pieceIndex--; //Stencil index for pieces goes from 1 to X. We decrement it to match the game index
	int gamePieceIndex = stencilToGameIndex[pieceIndex];

	if (mouseMode == MouseMode::Drag) {
		this -> selectedPiece = this->pieces[gamePieceIndex];
		this->selectedPieceGameIndex = gamePieceIndex;
		this->selectedPieceStencilIndex = pieceIndex;
		savedMatrix = selectedPiece->getMatrix();
		setNewMouseMoveDir(gamePieceIndex);
		this->lastX = xpos;
		this->lastY = ypos;
		return;
	}

	#pragma region simpleMouseMove
	SlidePuzzleSceneNode* pieceToMove = NULL;
	if ((this->emptyPos % 3) - (gamePieceIndex % 3) == 1 && this->emptyPos - gamePieceIndex == 1) {
		stencilToGameIndex[pieceIndex] = this->emptyPos;
		pieceToMove = getLeftPiece();
		movePiece(MOVE_RIGHT, pieceToMove);
	}
	else if ((gamePieceIndex % 3) - (this->emptyPos % 3) == 1 && gamePieceIndex - this->emptyPos == 1) {
		stencilToGameIndex[pieceIndex] = this->emptyPos;
		pieceToMove = getRightPiece();
		movePiece(MOVE_LEFT, pieceToMove);
	}
	else if (gamePieceIndex - this-> emptyPos == 3) {
		stencilToGameIndex[pieceIndex] = this->emptyPos;
		pieceToMove = getDownPiece();
		movePiece(MOVE_UP, pieceToMove);
	}
	else if (this->emptyPos - gamePieceIndex == 3) {
		stencilToGameIndex[pieceIndex] = this->emptyPos;
		pieceToMove = getUpPiece();
		movePiece(MOVE_DOWN, pieceToMove);
	}
	#pragma endregion
}

SlidePuzzleSceneNode* GameSlidingPuzzle::getRightPiece()
{
	if ((this->emptyPos % 3) + 1 > 2) return NULL;

	SlidePuzzleSceneNode* piece = this->pieces[(size_t)this->emptyPos + 1];
	std::swap(pieces[(size_t)emptyPos + 1], pieces[emptyPos]);
	this->stencilToGameIndex[piece->stencil_index - 1] = this->emptyPos;
	this->emptyPos += 1;

	return piece;
}
SlidePuzzleSceneNode* GameSlidingPuzzle::getLeftPiece()
{
	if ((this->emptyPos % 3) - 1 < 0) return NULL;

	SlidePuzzleSceneNode* piece = this->pieces[(size_t)this->emptyPos - 1];
	std::swap(pieces[(size_t)emptyPos - 1], pieces[emptyPos]);
	this->stencilToGameIndex[piece->stencil_index - 1] = this->emptyPos;
	this->emptyPos -= 1;

	return piece;
}
SlidePuzzleSceneNode* GameSlidingPuzzle::getUpPiece()
{
	if (this->emptyPos - 3 < 0) return NULL;

	SlidePuzzleSceneNode* piece = this->pieces[(size_t)this->emptyPos - 3];
	std::swap(pieces[(size_t)emptyPos - 3], pieces[emptyPos]);
	this->stencilToGameIndex[piece->stencil_index - 1] = this->emptyPos;
	this->emptyPos -= 3;

	return piece;
}
SlidePuzzleSceneNode* GameSlidingPuzzle::getDownPiece()
{
	if (this->emptyPos + 3 >= this->totalSlots) return NULL;

	SlidePuzzleSceneNode* piece = this->pieces[(size_t)this->emptyPos + 3];
	std::swap(pieces[(size_t)emptyPos + 3], pieces[emptyPos]);
	this->stencilToGameIndex[piece->stencil_index - 1] = this->emptyPos;
	this->emptyPos += 3;

	return piece;
}
void GameSlidingPuzzle::movePiece(Vector3d translation,SlidePuzzleSceneNode* piece)
{
	if (piece == NULL) return;

	piece->setMatrix(
		MatrixFactory::translationMatrix(translation) 
		* piece->getMatrix()
	);
}

void GameSlidingPuzzle::setMouseMode(MouseMode mode) {
	this->mouseMode = mode;
}
void GameSlidingPuzzle::setNewMouseMoveDir(int gamePieceIndex) {
	if ((this->emptyPos % 3) - (gamePieceIndex % 3) == 1 && this->emptyPos - gamePieceIndex == 1) {
		this->mouseMoveDir = MouseMoveDir::Right;
	}
	else if ((gamePieceIndex % 3) - (this->emptyPos % 3) == 1 && gamePieceIndex - this->emptyPos == 1) {
		this->mouseMoveDir = MouseMoveDir::Left;
	}
	else if (gamePieceIndex - this->emptyPos == 3) {
		this->mouseMoveDir = MouseMoveDir::Up;
	}
	else if (this->emptyPos - gamePieceIndex == 3) {
		this->mouseMoveDir = MouseMoveDir::Down;
	}
	else {
		this->mouseMoveDir = MouseMoveDir::None;
	}
}