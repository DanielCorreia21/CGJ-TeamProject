#include "GameSlidingPuzzle.h"
#include "MatrixFactory.h"

#define MOVE_AMOUNT 0.4f
#define MOVE_UP Vector3d(0.0f, MOVE_AMOUNT, 0.0f)
#define MOVE_DOWN Vector3d(0.0f, -MOVE_AMOUNT, 0.0f)
#define MOVE_LEFT Vector3d(-MOVE_AMOUNT, 0.0f, 0.0f)
#define MOVE_RIGHT Vector3d(MOVE_AMOUNT, 0.0f, 0.0f)


/*
*@requires piecesRoot to have its children by the correct order. Meaning, piece at pos 0 is the top leftmost one
* 
*/
GameSlidingPuzzle::GameSlidingPuzzle(SceneNode* piecesRoot, int pos)
{
	this->emptyPos = pos;

	vector<SceneNode*>* v = new vector<SceneNode*>();
	vector<SceneNode*> auxPieces = piecesRoot->getChildren();
	for (int i = 0; i < auxPieces.size() + 1; i++) {
		if (i != this->emptyPos) {
			v->push_back(auxPieces.at(i));
		}
		else {
			v->push_back(NULL);
		}
	}
	
	this->pieces = *v;
	this->totalSlots = this->pieces.size();
}

void GameSlidingPuzzle::handleInput(int key, int action)
{
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		
		SceneNode* piece = getLeftPiece();
		movePiece(MOVE_RIGHT, piece);
	}
	else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		SceneNode* piece = getRightPiece();
		movePiece(MOVE_LEFT, piece);
	}
	else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		SceneNode* piece = getDownPiece();
		movePiece(MOVE_UP, piece);
	}
	else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		SceneNode* piece = getUpPiece();
		movePiece(MOVE_DOWN, piece);
	}
}

SceneNode* GameSlidingPuzzle::getRightPiece()
{
	if ((this->emptyPos % 3) + 1 > 2) return NULL;

	SceneNode* piece = this->pieces[(size_t)this->emptyPos + 1];
	std::swap(pieces[(size_t)emptyPos + 1], pieces[emptyPos]);
	this->emptyPos += 1;

	return piece;
}
SceneNode* GameSlidingPuzzle::getLeftPiece()
{
	if ((this->emptyPos % 3) - 1 < 0) return NULL;

	SceneNode* piece = this->pieces[(size_t)this->emptyPos - 1];
	std::swap(pieces[(size_t)emptyPos - 1], pieces[emptyPos]);
	this->emptyPos -= 1;

	return piece;
}
SceneNode* GameSlidingPuzzle::getUpPiece()
{
	if (this->emptyPos - 3 < 0) return NULL;

	SceneNode* piece = this->pieces[(size_t)this->emptyPos - 3];
	std::swap(pieces[(size_t)emptyPos - 3], pieces[emptyPos]);
	this->emptyPos -= 3;

	return piece;
}
SceneNode* GameSlidingPuzzle::getDownPiece()
{
	if (this->emptyPos + 3 >= this->totalSlots) return NULL;

	SceneNode* piece = this->pieces[(size_t)this->emptyPos + 3];
	std::swap(pieces[(size_t)emptyPos + 3], pieces[emptyPos]);
	this->emptyPos += 3;

	return piece;
}

void GameSlidingPuzzle::movePiece(Vector3d translation,SceneNode* piece)
{
	if (piece == NULL) return;

	piece->setMatrix(
		MatrixFactory::translationMatrix(translation) 
		* piece->getMatrix()
	);
}

