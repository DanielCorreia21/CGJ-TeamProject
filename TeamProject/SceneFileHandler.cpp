#include "SceneFileHandler.h"
#include "SlidePuzzleSceneNode.h"

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
		/*string shaderProgramIndex = "shaderProgramIndex " + to_string(shaderProgramIndx);
		indexToShaderProgram.insert(pair<int, ShaderProgram*>(shaderProgramIndx, shader));
		shaderProgramIndx++;*/

		string shaderProgramName = "shaderProgramName " + ShaderProgramManager::getInstance()->get(shader);

		string vertexShaderPath = "vertexShaderPath " + shader->vertexPath;
		string fragmentShaderPath = "fragmentShaderPath " + shader->fragmentPath;

		outputBuffer.push_back("#shaderProgram");
		outputBuffer.push_back(shaderProgramName);
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

		int auxStencilIndex = -1;
		if (node->nodeType == SceneNode::NodeType::SLIDEPUZZLE) { auxStencilIndex = ((SlidePuzzleSceneNode*)node)->stencil_index; }
		string stencilIndex = "stencilIndex " + to_string(auxStencilIndex);

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
				TextureInfo* textureInfo = textures.at(i);
				string textPath = textureInfo->texture->texture_path;
				if (textPath.find('/') != string::npos) {
					auxTexturePaths = auxTexturePaths + textPath + " ";
				}
				else {
					auxTexturePaths = auxTexturePaths + TextureManager::getInstance()->get(textures.at(i)->texture) + " ";
				}
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
		outputBuffer.push_back(stencilIndex);
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
	ofstream ofile;
	ofile.open("../Saves/savedScene.txt", ios::out);
	if (!ofile) {
		cout << "File not created!";
	}
	else {
		for (int i = 0; i < outputBuffer.size(); i++) {
			ofile << outputBuffer.at(i) + "\n";
		}
		cout << "Scene saved successfully!\n";
		ofile.close();
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
	if (node->getParent() == NULL) return allNodes;

	vector<SceneNode*> empty;
	return empty;
}

int SceneFileHandler::getIndexFromNode(map<int, SceneNode*> nodeMap, SceneNode* node) {
	typename map<int, SceneNode*>::iterator it = nodeMap.begin();

	while (it != nodeMap.end()) {

		if (it->second == node) {
			return it->first;
		}
		it++;
	}
	return -1;
}


enum class CurrentObjType {
	None,
	Camera,
	ShaderProgram,
	SceneGraph,
	SceneNode
};

CurrentObjType currentObjType = CurrentObjType::None;



vector<string> split(const string& s) {
	vector<string> elems;
	for (size_t p = 0, q = 0; p != s.npos; p = q)
		elems.push_back(s.substr(p + (p != 0), (q = s.find(" ", p + 1)) - p - (p != 0)));
	return elems;
}

Camera* sceneCamera = NULL;
map<string, ShaderProgram*> shaderPrograms;
SceneGraph* sceneGraph = NULL;
map<int, SceneNode*> sceneNodes;
map<string, Mesh*> loadedMeshes;
map<string, TextureInfo*> loadedTextures;

const string TEXTURE_UNIFORM_NOISE = "NoiseTexture";
const string TEXTURE_UNIFORM_COLOR = "Texture";

SceneNode* createSceneNode(string line) {

	static int nodeIndex = -1;
	static int parentIndex = -1;
	static int stencilIndex = 0;
	static SceneNode::NodeType nodeType = SceneNode::NodeType::NORMAL;
	static bool hasSceneGraph = false;
	static string meshPath = "";
	static Matrix4d localMatrix = Matrix4d();
	static string texturePaths = "";
	static string shaderProgramName = "";


	vector<string> lineElements = split(line);

	if (lineElements[0].compare("nodeIndex") == 0) {
		nodeIndex = stoi(lineElements[1]);
	}
	else if (lineElements[0].compare("parentIndex") == 0) {
		parentIndex = stoi(lineElements[1]);
	}
	else if (lineElements[0].compare("stencilIndex") == 0) {
		stencilIndex = stoi(lineElements[1]);
	}
	else if (lineElements[0].compare("nodeType") == 0) {
		if (lineElements[1].compare("normal") == 0) { nodeType = SceneNode::NodeType::NORMAL; }
		else if (lineElements[1].compare("outline") == 0) { nodeType = SceneNode::NodeType::OUTLINE; }
		else if (lineElements[1].compare("slidepuzzle") == 0) { nodeType = SceneNode::NodeType::SLIDEPUZZLE; }
	}
	else if (lineElements[0].compare("hasSceneGraph") == 0) {
		hasSceneGraph = lineElements[1].compare("true") == 0 ? true : false;
	}
	else if (lineElements[0].compare("meshPath") == 0) {
		meshPath = lineElements[1];
	}
	else if (lineElements[0].compare("localMatrix") == 0) {
		float auxMatrix[16];
		for (int i = 1; i < 17; i++) {
			auxMatrix[i - 1] = stof(lineElements[i]);
		}
		localMatrix = Matrix4d(auxMatrix);
	}
	else if (lineElements[0].compare("texturePaths") == 0) {

		texturePaths = "";
		for(int i = 1; i < lineElements.size(); i++) {
			texturePaths = texturePaths + lineElements[i] + " ";
		}
	}
	else if (lineElements[0].compare("shaderProgramName") == 0) {
		shaderProgramName = lineElements[1];
	}
	else if (line.compare("#endsceneNode") == 0) {
		
#pragma region createNode
		ShaderProgram* shader = ShaderProgramManager::getInstance()->get(shaderProgramName);
		vector<TextureInfo*> textures;

		vector<string> texturesP = split(texturePaths);
		if (texturesP[0].compare("NONE") != 0) {

			for (int i = 0; i < texturesP.size(); i++) {

				string textureString = texturesP[i];				
				if (textureString == "") { continue; }

				TextureInfo* textureInfo;
				if (textureString.find('/') != string::npos) {//if its a path, we create it ourselves

					map<string, TextureInfo*>::iterator it = loadedTextures.find(textureString);
					if (it != loadedTextures.end()) {
						textureInfo = it->second;
					}
					else {
						Texture2D* texture = new Texture2D();
						texture->load(textureString);
						textureInfo = new TextureInfo(GL_TEXTURE0, 0, TEXTURE_UNIFORM_NOISE, texture);
					}
					textures.push_back(textureInfo);
				
				}
				else {
					map<string, TextureInfo*>::iterator it = loadedTextures.find(textureString);
					if (it != loadedTextures.end()) {
						textureInfo = it->second;
					}
					else {
						textureInfo = new TextureInfo(GL_TEXTURE0, 0, TEXTURE_UNIFORM_NOISE, TextureManager::getInstance()->get(textureString));
					}
					textures.push_back(textureInfo);
				}
			}
		}
		//---------------Piece------------------
		SceneNode* node = NULL;

		if (nodeType == SceneNode::NodeType::NORMAL) { node = new SceneNode(); }
		else if (nodeType == SceneNode::NodeType::OUTLINE) { node = new OutlineSceneNode(); }
		else if (nodeType == SceneNode::NodeType::SLIDEPUZZLE) { node = new SlidePuzzleSceneNode(stencilIndex); }

		if(parentIndex != -1){ node->setParent(sceneNodes[parentIndex]);}

		if(meshPath != "NONE"){ 
			Mesh* mesh;
			map<string, Mesh*>::iterator it = loadedMeshes.find(meshPath);
			if (it != loadedMeshes.end()){
				mesh = it->second;
			}
			else { //create mesh if not created yet
				mesh = new Mesh();
				mesh->init(meshPath);
				loadedMeshes.insert(pair<string,Mesh*>(meshPath,mesh));
				MeshManager::getInstance()->add(meshPath, mesh);
			}
			node->setMesh(mesh);
		}
		
		node->setMatrix(localMatrix);

		node->setShaderProgram(shader);
		for (int i = 0; i < textures.size(); i++) {
			node->addTexture(textures.at(i));
		}

		if (hasSceneGraph) { node->setSceneGraph(sceneGraph); }

		sceneNodes.insert(pair<int, SceneNode*>(nodeIndex, node));
		return node;
#pragma endregion

	}

	return NULL;
}

SceneGraph* createSceneGraph(string line) {

	static string sceneGraphName;

	vector<string> lineElements = split(line);

	if (lineElements[0].compare("sceneGraphName") == 0) {
		sceneGraphName = lineElements[1];
	}
	else if (line.compare("#endsceneGraph") == 0) {
		SceneGraph* sceneGraph = new SceneGraph();
		SceneGraphManager::getInstance()->add(sceneGraphName, sceneGraph);
		return sceneGraph;
	}

	return NULL;
}

pair<string,ShaderProgram*> createShaderProgram(string line) {

	static string shaderProgramName;
	static string vertexShaderPath;
	static string fragmentShaderPath;

	vector<string> lineElements = split(line);

	if (lineElements[0].compare("shaderProgramName") == 0) {
		shaderProgramName = lineElements[1];
	}
	else if (lineElements[0].compare("vertexShaderPath") == 0) {
		vertexShaderPath = lineElements[1];
	}
	else if (lineElements[0].compare("fragmentShaderPath") == 0) {
		fragmentShaderPath = lineElements[1];
	}
	else if (line.compare("#endshaderProgram") == 0) {
		ShaderProgram* shaderProgram = new ShaderProgram();
		shaderProgram->init(vertexShaderPath.c_str(),fragmentShaderPath.c_str());
		return pair<string,ShaderProgram*>(shaderProgramName, shaderProgram);
	}

	return pair<string, ShaderProgram*>("", NULL);
}

Camera* createCamera(string line) {

	static Vector3d eulerAngles;
	static Camera::CameraType projectionType;
	static Camera::RotationMode rotationType;
	static Vector3d translationVector;

	vector<string> lineElements = split(line);

	if (lineElements[0].compare("angles") == 0) {
		float x = stof(lineElements[1]);
		float y = stof(lineElements[2]);
		float z = stof(lineElements[3]);
		eulerAngles = Vector3d(x,y,z);
	}else if (lineElements[0].compare("projectionType") == 0) {
		projectionType = lineElements[1].compare("perspective") == 0 ? Camera::CameraType::PERSPECTIVE : Camera::CameraType::ORTHOGONAL;
	}
	else if (lineElements[0].compare("rotationType") == 0) {
		rotationType = lineElements[1].compare("euler") == 0 ? Camera::RotationMode::EULER : Camera::RotationMode::QUATERNION;
	}
	else if (lineElements[0].compare("translationVector") == 0) {
		float x = stof(lineElements[1]);
		float y = stof(lineElements[2]);
		float z = stof(lineElements[3]);
		translationVector = Vector3d(x, y, z);
	}
	else if (line.compare("#endcamera") == 0) {
		Camera* camera = new Camera(Vector3d(0, 0, -10), Vector3d(0, 0, -1), Vector3d(0, 1, 0));
		if (camera->currentType != projectionType) { camera->changeProjectionType(); }
		if (camera->currentRotation != rotationType) { camera->changeRotationType(); }
		camera->setEulerAngles(eulerAngles);
		camera->translationVector = translationVector;
		return camera;
	}

	return NULL;
}

void parseLine(string line) {

	if (line.compare("#camera") == 0) { currentObjType = CurrentObjType::Camera; }
	else if (line.compare("#shaderProgram") == 0) { currentObjType = CurrentObjType::ShaderProgram; }
	else if (line.compare("#sceneGraph") == 0) { currentObjType = CurrentObjType::SceneGraph; }
	else if (line.compare("#sceneNode") == 0) { currentObjType = CurrentObjType::SceneNode; }

	switch (currentObjType) {
		case CurrentObjType::None:
		{}
		case CurrentObjType::Camera:
		{
			Camera* camera = createCamera(line);
			if (camera != NULL) { sceneCamera = camera; currentObjType = CurrentObjType::None; }
		}
		break;
		case CurrentObjType::ShaderProgram:
		{
			pair<string, ShaderProgram*> shaderProgram = createShaderProgram(line);
			if (shaderProgram.second != NULL) { shaderPrograms.insert(shaderProgram); currentObjType = CurrentObjType::None; }
		}
		break;
		case CurrentObjType::SceneGraph:
		{
			SceneGraph* scGrph = createSceneGraph(line);
			if (scGrph != NULL) {
				sceneGraph = scGrph;
				sceneGraph->setCamera(sceneCamera);
				currentObjType = CurrentObjType::None;
			};
			
		}
		break;
		case CurrentObjType::SceneNode:
		{
			createSceneNode(line);
		}
		break;
	}
}


SceneGraph* SceneFileHandler::loadScene() {
	ifstream ifile("../Saves/savedScene.txt");
	string line;
	if (!ifile) {
		cout << "File not loaded!";
		return NULL;
	}
	while (std::getline(ifile, line))
	{
		parseLine(line);
	}

	sceneGraph->setRoot(sceneNodes[0]);

	return sceneGraph;
}



