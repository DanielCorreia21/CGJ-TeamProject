#include "SceneFileHandler.h"

using namespace std;

SceneFileHandler::SceneFileHandler() {}

//TODO handle multiple sceneGraphs

void SceneFileHandler::saveScene(SceneGraph* scene) {
	vector<string> outputBuffer;

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
	outputBuffer.push_back("#camera");
	outputBuffer.push_back(eulerAngles);
	outputBuffer.push_back(projectionType);
	outputBuffer.push_back(rotationType);
	outputBuffer.push_back(translationVector);
	outputBuffer.push_back("#endcamera\n");

#pragma endregion

#pragma region shaderProgram

	vector<ShaderProgram*> shaders = ShaderProgramManager::getInstance()->getAllValues();
	int shaderProgramIndx = 0;
	map<int, ShaderProgram*> indexToShaderProgram;
	for (int i = 0; i < shaders.size(); i++) {

		ShaderProgram* shader = shaders.at(i);
		string shaderProgramIndex = "shaderProgramIndex " + to_string(shaderProgramIndx);
		indexToShaderProgram.insert(pair<int, ShaderProgram*>(shaderProgramIndx, shader));
		shaderProgramIndx++;

		string vertexShaderPath = "vertexShaderPath " + shader->vertexPath;
		string fragmentShaderPath = "fragmentShaderPath " + shader->fragmentPath;
		outputBuffer.push_back("#shaderProgram");
		outputBuffer.push_back(shaderProgramIndex);
		outputBuffer.push_back(vertexShaderPath);
		outputBuffer.push_back(fragmentShaderPath);
		outputBuffer.push_back("#endshaderProgram\n");
	}
	

#pragma endregion

#pragma region sceneGraph
	string sceneGraphName = "sceneGraphName " + SceneGraphManager::getInstance()->get(scene);
	outputBuffer.push_back("#sceneGraph");
	outputBuffer.push_back(sceneGraphName);
	outputBuffer.push_back("#endsceneGraph\n");
#pragma endregion

#pragma region sceneNodes

	//helper vars
	map<int, SceneNode*> indexToNode;
	int nodeIndx = 0;

	SceneNode* node = scene->getRoot();
	vector<SceneNode*> nodes = getAllNodes(node);
	for (int i = 0; i < nodes.size(); i++) {
		node = nodes.at(i);
		string nodeIndex = "nodeIndex " + to_string(nodeIndx);
		indexToNode.insert(pair<int, SceneNode*>(nodeIndx, node));
		nodeIndx++;

		string auxParentIdx = "";
		if (node->getParent() != NULL) {
			auxParentIdx = to_string(getIndexFromNode(indexToNode,node->getParent()));
		}
		else { auxParentIdx = to_string(-1); }
		string parentIndex = "parentIndex " + auxParentIdx;

		string auxType = "";
		if (node->nodeType == SceneNode::NodeType::NORMAL) { auxType = "normal"; }
		else if (node->nodeType == SceneNode::NodeType::OUTLINE) { auxType = "outline"; }
		else if (node->nodeType == SceneNode::NodeType::SLIDEPUZZLE) { auxType = "slidepuzzle"; }
		string nodeType = "nodeType " + auxType;

		string auxHasSceneGraph = "";
		if (node->getLocalSceneGraph() != NULL) { auxHasSceneGraph = "true"; } else { auxHasSceneGraph = "false"; }
		string hasSceneGraph = "hasSceneGraph " + auxHasSceneGraph;

		string auxMeshPath = "";
		if (node->getMesh() != NULL) { auxMeshPath = node->getMesh()->meshPath; }
		else { auxMeshPath = "NONE"; }
		string meshPath = "meshPath " + auxMeshPath;

		string auxLocalMatrix = "";
		float matrix[16]; node->getMatrix().toColumnMajorArray(matrix);
		for (int i = 0; i < 16; i++) {
			auxLocalMatrix = auxLocalMatrix + to_string(matrix[i]) + " ";
		}
		string localMatrix = "localMatrix " + auxLocalMatrix;

		string auxTexturePaths = "";
		vector<TextureInfo*> textures = node->getTextures();
		if (textures.size() > 0) { 
			for (int i = 0; i < textures.size(); i++) {

				auxTexturePaths = auxTexturePaths + textures.at(i)->texture->texture_path + " ";
			}
		}
		else { auxTexturePaths = "NONE"; }
		string texturePaths = "texturePaths " + auxTexturePaths;

		string auxShaderProgramName =
			ShaderProgramManager::getInstance()->get(node->getLocalShaderProgram()) == "" ? "NONE" :
				ShaderProgramManager::getInstance()->get(node->getLocalShaderProgram());
		string shaderProgramName = "shaderProgramName " + auxShaderProgramName;

		//TODO pre and post drawFun

		outputBuffer.push_back("#sceneNode");
		outputBuffer.push_back(nodeIndex);
		outputBuffer.push_back(parentIndex);
		outputBuffer.push_back(nodeType);
		outputBuffer.push_back(hasSceneGraph);
		outputBuffer.push_back(meshPath);
		outputBuffer.push_back(localMatrix);
		outputBuffer.push_back(texturePaths);
		outputBuffer.push_back(shaderProgramName);
		outputBuffer.push_back("#endsceneNode\n");
	}


#pragma endregion

#pragma region writeToFile
	ofstream my_file;
	my_file.open("../Saves/savedScene.txt", ios::out);
	if (!my_file) {
		cout << "File not created!";
	}
	else {
		for (int i = 0; i < outputBuffer.size(); i++) {
			my_file << outputBuffer.at(i) + "\n";
		}
		cout << "Scene saved successfully!";
		my_file.close();
	}
#pragma endregion
}


vector<SceneNode*> allNodes;
vector<SceneNode*> SceneFileHandler::getAllNodes(SceneNode* node) {

	if (node->getChildren().size() == 0) {
		allNodes.push_back(node);
	}
	else {
		allNodes.push_back(node);
		for (int i = 0; i < node->getChildren().size(); i++) {
			getAllNodes(node->getChildren().at(i));
		}
	}
	if(node->getParent() == NULL) return allNodes;

	vector<SceneNode*> empty;
	return empty;
}

int SceneFileHandler::getIndexFromNode(map<int,SceneNode*> nodeMap, SceneNode* node) {
	typename map<int, SceneNode*>::iterator it = nodeMap.begin();

	while (it != nodeMap.end()) {

		if (it->second == node) {
			return it->first;
		}
		it++;
	}
	return -1;
}
