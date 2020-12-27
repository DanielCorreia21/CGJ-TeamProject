#include "SceneFileHandler.h"

using namespace std;

SceneFileHandler::SceneFileHandler() {}

void SceneFileHandler::saveScene(SceneGraph* scene) {

	vector<string> outputBuffer;

	string sceneGraphName = SceneGraphManager::getInstance()->get(scene);
	outputBuffer.push_back(sceneGraphName);
#pragma region camera
	Camera* camera = scene->getCamera();
	Vector3d angles = camera->getEulerAngles();
	string proj = camera->currentType == Camera::CameraType::ORTHOGONAL ? "orthogonal" : "perspective";
	string rot = camera->currentRotation == Camera::RotationMode::EULER ? "euler" : "quaternion";

	string eulerAngles = "angles " + to_string(angles.getX()) + " " + to_string(angles.getY()) + " " + to_string(angles.getZ());
	string projectionType = "projectionType " + proj;
	string rotationType = "rotationType " + rot;
	string translationVector = "translationVector " + to_string(camera->translationVector.getX()) + " "
		+ to_string(camera->translationVector.getY()) + " "
		+ to_string(camera->translationVector.getZ());
	outputBuffer.push_back(eulerAngles);
	outputBuffer.push_back(projectionType);
	outputBuffer.push_back(rotationType);
	outputBuffer.push_back(translationVector);
#pragma endregion

#pragma region writeToFile
	ofstream my_file;
	my_file.open("../Saves/testSave.txt", ios::out);
	if (!my_file) {
		cout << "File not created!";
	}
	else {
		for (int i = 0; i < outputBuffer.size(); i++) {
			my_file << outputBuffer.at(i) + "\n";
		}
		cout << "File created successfully!";
		my_file.close();
	}
#pragma endregion
}