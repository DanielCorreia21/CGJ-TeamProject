#include "SlidePuzzleGameFileHandler.h"
#include <direct.h>

const string gameSaveFileName = "SlidePuzzleGame";

#pragma region Singleton
SlidePuzzleGameFileHandler* SlidePuzzleGameFileHandler::instance = NULL;
SlidePuzzleGameFileHandler* SlidePuzzleGameFileHandler::getInstance() {
	if (!instance) {
		instance = new SlidePuzzleGameFileHandler();
	}
	return instance;
}

SlidePuzzleGameFileHandler::SlidePuzzleGameFileHandler() {}
#pragma endregion

void SlidePuzzleGameFileHandler::saveGame(GameSlidingPuzzle* game) {
	vector<string> outputBuffer;

	string emptyPos = "emptyPos " + to_string(game->emptyPos);

	string auxPiecePositions = "";
	vector<SlidePuzzleSceneNode*> pieces = game->pieces;
	for (int i = 0; i < pieces.size(); i++) {
		if (pieces.at(i) != NULL) {
			auxPiecePositions = auxPiecePositions + to_string(pieces.at(i)->stencil_index-1) + " ";
		}
		else {
			auxPiecePositions = auxPiecePositions + "-1" + " ";
		}
	}
	string piecePositions = "piecePositions " + auxPiecePositions;

	string auxMouseMode = game->mouseMode == GameSlidingPuzzle::MouseMode::Drag ? "drag" : "click";
	string mouseMode = "mouseMode " + auxMouseMode;

	outputBuffer.push_back(emptyPos);
	outputBuffer.push_back(piecePositions);
	outputBuffer.push_back(mouseMode);

#pragma region writeToFile

	if (_mkdir(saves_dir.c_str()) == ENOENT) {
		cout << "Could not save game. Path not found.";
		return;
	}

	ofstream ofile;
	ofile.open("../Saves/" + gameSaveFileName + ".txt", ios::out);
	if (!ofile) {
		cout << "File not created!";
	}
	else {
		for (int i = 0; i < outputBuffer.size(); i++) {
			ofile << outputBuffer.at(i) + "\n";
		}
		cout << "Game saved successfully!\n";
		ofile.close();
	}
#pragma endregion
}

vector<string> SlidePuzzleGameFileHandler::split(const string& s) {
	vector<string> elems;
	for (size_t p = 0, q = 0; p != s.npos; p = q)
		elems.push_back(s.substr(p + (p != 0), (q = s.find(" ", p + 1)) - p - (p != 0)));
	return elems;
}

void SlidePuzzleGameFileHandler::parseLine(string line) {

	vector<string> lineElements = split(line);

	if (lineElements[0].compare("emptyPos") == 0) {
		this->emptyPos = stoi(lineElements[1]);
	}
	else if (lineElements[0].compare("piecePositions") == 0) {
		for (int i = 1; i < lineElements.size(); i++) {
			if (lineElements[i] == "") { continue; }
			this->piecesPositions.push_back(stoi(lineElements[i]));
		}
	} 
	else if (lineElements[0].compare("mouseMode") == 0) {
		if (lineElements[1].compare("drag") == 0) {
			this->mouseMode = GameSlidingPuzzle::MouseMode::Drag;
		}
		else if (lineElements[1].compare("click") == 0) {
			this->mouseMode = GameSlidingPuzzle::MouseMode::Click;
		}
	}
}

void SlidePuzzleGameFileHandler::loadGame(GameSlidingPuzzle* game) {

	if (_mkdir(saves_dir.c_str()) == ENOENT) {
		cout << "Could not load game. Path not found.";
		return;
	}

	this->emptyPos = -1;
	this->piecesPositions = vector<int>();

	ifstream ifile("../Saves/" + gameSaveFileName + ".txt");
	string line;
	if (!ifile) {
		cout << "Game not loaded!";
		return;
	}
	while (std::getline(ifile, line))
	{
		parseLine(line);
	}

	game->emptyPos = this->emptyPos;
	game->setMouseMode(this->mouseMode);
	cout << "Game loaded successfully!\n";
}