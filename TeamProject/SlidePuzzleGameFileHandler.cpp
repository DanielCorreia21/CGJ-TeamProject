#include "SlidePuzzleGameFileHandler.h"

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

	outputBuffer.push_back(emptyPos);
	outputBuffer.push_back(piecePositions);

#pragma region writeToFile
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
	if (lineElements[0].compare("piecePositions") == 0) {
		for (int i = 1; i < lineElements.size(); i++) {
			if (lineElements[i] == "") { continue; }
			this->piecesPositions.push_back(stoi(lineElements[i]));
		}
		
	}
}

void SlidePuzzleGameFileHandler::loadGame(GameSlidingPuzzle* game) {
	this->emptyPos = -1;
	this->piecesPositions = vector<int>();

	ifstream ifile("../Saves/" + gameSaveFileName + ".txt");
	string line;
	if (!ifile) {
		cout << "File not loaded!";
		return;
	}
	while (std::getline(ifile, line))
	{
		parseLine(line);
	}

	game->emptyPos = this->emptyPos;
	cout << "Game loaded successfully!\n";
}