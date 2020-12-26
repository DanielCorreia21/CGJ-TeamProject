#include "GameSlidingPuzzle.h"

#define MOVE_AMOUNT 0.4f
#define MOVE_UP Vector3d(0.0f, MOVE_AMOUNT, 0.0f)
#define MOVE_DOWN Vector3d(0.0f, -MOVE_AMOUNT, 0.0f)
#define MOVE_LEFT Vector3d(-MOVE_AMOUNT, 0.0f, 0.0f)
#define MOVE_RIGHT Vector3d(MOVE_AMOUNT, 0.0f, 0.0f)

/*
*@requires piecesRoot to have its children by the winning order.
* @param pos : the position that has no piece, from 0 to amountOfPieces - 1 
* TODO: Scramble the pieces
* 
*/
GameSlidingPuzzle::GameSlidingPuzzle(SceneNode* piecesRoot, int pos)
{
	this->emptyPos = pos;

	vector<SlidePuzzleSceneNode*>* v = new vector<SlidePuzzleSceneNode*>();
	vector<SceneNode*> auxPieces = piecesRoot->getChildren();
	this->winningOrder = new int[auxPieces.size()];

	for (int i = 0; i < auxPieces.size() + 1; i++) {
		if (i == this->emptyPos) { v->push_back(NULL); }
		else {
			v->push_back(((SlidePuzzleSceneNode*) auxPieces.at(i)));
			int stencilIndex = v->at(i)->stencil_index-1;
			stencilToGameIndex.insert(pair<int, int>(stencilIndex,i));
			this->winningOrder[i] = stencilIndex;
		}
	}
	this->pieces = *v;
	scramblePieces();
}

#pragma region keyboardExternalMethods
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
#pragma endregion

#pragma region mouseExternalMethods
void GameSlidingPuzzle::handleMouseClick(float xpos, float ypos, int pieceIndex)
{	
	//If we didn't click on a piece, we return
	if (pieceIndex <= 0 || pieceIndex > this->pieces.size()) {
		this->selectedPiece = NULL;
		return;
	}

	pieceIndex--; //Stencil index for pieces goes from 1 to X. We decrement it to match the game index
	int gamePieceIndex = stencilToGameIndex[pieceIndex];

	if (mouseMode == MouseMode::Drag) {
		initMouseDrag(xpos, ypos, pieceIndex, gamePieceIndex);
	}
	else if (mouseMode == MouseMode::Click) {
		simpleMouseMove(pieceIndex,gamePieceIndex);
	}
}

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

	float moveAmount;

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
		std::swap(pieces[selectedPieceGameIndex], pieces[emptyPos]);
		stencilToGameIndex[selectedPieceStencilIndex] = this->emptyPos;
		this->emptyPos = selectedPieceGameIndex;
		this->selectedPiece = NULL;
		checkWinningState();
	}
	totalMoved = 0;
}
#pragma endregion

#pragma region mouseHelpers
void GameSlidingPuzzle::initMouseDrag(float xpos, float ypos, int pieceIndex, int gamePieceIndex) {
	this->selectedPiece = this->pieces[gamePieceIndex];
	this->selectedPieceGameIndex = gamePieceIndex;
	this->selectedPieceStencilIndex = pieceIndex;
	savedMatrix = selectedPiece->getMatrix();
	setNewMouseMoveDir(gamePieceIndex);
	this->lastX = xpos;
	this->lastY = ypos;
}

void GameSlidingPuzzle::simpleMouseMove(int pieceIndex, int gamePieceIndex) {
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
	else if (gamePieceIndex - this->emptyPos == 3) {
		stencilToGameIndex[pieceIndex] = this->emptyPos;
		pieceToMove = getDownPiece();
		movePiece(MOVE_UP, pieceToMove);
	}
	else if (this->emptyPos - gamePieceIndex == 3) {
		stencilToGameIndex[pieceIndex] = this->emptyPos;
		pieceToMove = getUpPiece();
		movePiece(MOVE_DOWN, pieceToMove);
	}
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
#pragma endregion

#pragma region helperMethods
//Get DirectionPiece gets the piece in the direction FROM the empty position.
//e.g: getRightPiece gets the piece on the right of the emptyPos
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
	if ((size_t)this->emptyPos + 3 >= this->pieces.size()) return NULL;

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
	checkWinningState();
}

void GameSlidingPuzzle::scramblePieces()
{
	int piecesSize = (int)pieces.size();
	int totalSwaps = piecesSize * 3;

	//Next time think about impossible puzzles first instead of spending 15mins like a retard.
	//https://puzzling.stackexchange.com/questions/25563/do-i-have-an-unsolvable-15-puzzle
	if (totalSwaps % 2 != 0) totalSwaps--;

	int randomNumber1;
	int randomNumber2;

	for (int i = 0; i < totalSwaps; i++) {
		randomNumber1 = getRandomIntDifferentFromTwo(this->emptyPos, -1, piecesSize);
		randomNumber2 = getRandomIntDifferentFromTwo(this->emptyPos, randomNumber1, piecesSize);
		
		swapPieces(randomNumber1, randomNumber2);
	}
}

int GameSlidingPuzzle::getRandomIntDifferentFromTwo(int x,int y, int range) {
	int randomNumber = rand() % range;
	while (randomNumber == x || randomNumber == y) {
		randomNumber = rand() % range;
	}
	return randomNumber;
}

void GameSlidingPuzzle::swapPieces(int randomNumber1, int randomNumber2) {

	/* Things to swap: 
		- Local matrixes
		- GameIndexes
	*/
	
	SlidePuzzleSceneNode* piece1 = pieces.at(randomNumber1);
	SlidePuzzleSceneNode* piece2 = pieces.at(randomNumber2);
	Matrix4d piece1Matrix = piece1->getMatrix();

	int piece1GameIndex = this->stencilToGameIndex[piece1->stencil_index-1];
	int piece2GameIndex = this->stencilToGameIndex[piece2->stencil_index-1];

	this->stencilToGameIndex[piece1->stencil_index-1] = piece2GameIndex;
	this->stencilToGameIndex[piece2->stencil_index-1] = piece1GameIndex;

	piece1->setMatrix(piece2->getMatrix());
	piece2->setMatrix(piece1Matrix);
	std::swap(pieces[randomNumber1], pieces[randomNumber2]);
}

bool GameSlidingPuzzle::checkWinningState() {
	bool puzzleSolved = true;
	for (int i = 0; i < this->pieces.size(); i++) {
		if (i == this->emptyPos) { continue; }
		if (this->winningOrder[i] != this->pieces.at(i)->stencil_index) {
			puzzleSolved = false;
		}
	}

	if (puzzleSolved) {
		cout << "**************\n"
			<< "You won! :)\n"
			<< "**************\n";
	}

	return puzzleSolved;
}
#pragma endregion
