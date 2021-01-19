#include "SceneFileHandler.h"
#include "SlidePuzzleSceneNode.h"
#include <direct.h>

using namespace std;

#pragma region Singleton
SceneFileHandler* SceneFileHandler::instance = NULL;
SceneFileHandler* SceneFileHandler::getInstance() {
	if (!instance) {
		instance = new SceneFileHandler();
	}
	return instance;
}

SceneFileHandler::SceneFileHandler() {}
#pragma endregion

//TODO handle multiple sceneGraphs
vector<SceneNode*> allNodes;

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

		string shaderProgramMapName = "shaderProgramMapName " + shader->getId();

		string auxShaderUni = "";
		map<string, ShaderProgram::UniformInfo>::iterator itUni = shader->uniforms.begin();
		while (itUni != shader->uniforms.end()) {

			auxShaderUni = auxShaderUni + itUni->first + " ";
			itUni++;
		}
		if (auxShaderUni == "") { auxShaderUni = "NONE"; }
		string shaderUniforms = "shaderUniforms " + auxShaderUni;

		string auxShaderAtt = "";
		map<string, ShaderProgram::AttributeInfo>::iterator itAtt = shader->attributes.begin();
		while (itAtt != shader->attributes.end()) {

			auxShaderAtt = auxShaderAtt + itAtt->first + " ";
			itAtt++;
		}
		if (auxShaderAtt == "") { auxShaderAtt = "NONE"; }
		string shaderAtributes = "shaderAtributes " + auxShaderAtt;

		string vertexShaderPath = "vertexShaderPath " + shader->vertexPath;
		string fragmentShaderPath = "fragmentShaderPath " + shader->fragmentPath;

		outputBuffer.push_back("#shaderProgram");
		outputBuffer.push_back(shaderProgramMapName);
		outputBuffer.push_back(shaderUniforms);
		outputBuffer.push_back(shaderAtributes);
		outputBuffer.push_back(vertexShaderPath);
		outputBuffer.push_back(fragmentShaderPath);
		outputBuffer.push_back("#endshaderProgram\n");
	}
	

#pragma endregion

#pragma region sceneGraph
	string sceneGraphMapName = "sceneGraphMapName " + scene->getId();

	outputBuffer.push_back("#sceneGraph");
	outputBuffer.push_back(sceneGraphMapName);
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
		string auxMeshMapName = "";
		if (node->getMesh() != NULL) { 
			auxMeshPath = node->getMesh()->meshPath; 
			auxMeshMapName = node->getMesh()->getId();
		}
		else { auxMeshPath = "NONE"; auxMeshMapName = "NONE"; }
		string meshPath = "meshPath " + auxMeshPath;
		string meshMapName = "meshMapName " + auxMeshMapName;

		string auxLocalMatrix = "";
		float matrix[16]; node->getMatrix().toColumnMajorArray(matrix);
		for (int i = 0; i < 16; i++) {
			auxLocalMatrix = auxLocalMatrix + to_string(matrix[i]) + " ";
		}
		string localMatrix = "localMatrix " + auxLocalMatrix;

		string auxTexturePaths = "";
		string auxTextureUniforms = "";
		string auxTextureMapNames = "";
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
				auxTextureUniforms = auxTextureUniforms + textureInfo->uniform + " ";
				auxTextureMapNames = auxTextureMapNames + textureInfo->texture->getId() + " ";
			}
		}
		else { auxTexturePaths = "NONE"; auxTextureUniforms = "NONE"; auxTextureMapNames = "NONE"; }
		string texturePaths = "texturePaths " + auxTexturePaths;
		string textureUniforms = "textureUniforms " + auxTextureUniforms;
		string textureMapNames = "textureMapNames " + auxTextureMapNames;

		string auxShaderProgramName =
			ShaderProgramManager::getInstance()->get(node->getLocalShaderProgram()) == "" ? "NONE" :
				ShaderProgramManager::getInstance()->get(node->getLocalShaderProgram());
		string shaderProgramName = "shaderProgramName " + auxShaderProgramName;

		//TODO pre and post drawFun
		string preDrawFunName = "preDrawFunName " + (node->preDrawFun == NULL ? "NONE" : node->preDrawFun->getId());


		outputBuffer.push_back("#sceneNode");
		outputBuffer.push_back(nodeIndex);
		outputBuffer.push_back(parentIndex);
		outputBuffer.push_back(stencilIndex);
		outputBuffer.push_back(nodeType);
		outputBuffer.push_back(hasSceneGraph);
		outputBuffer.push_back(meshPath);
		outputBuffer.push_back(meshMapName);
		outputBuffer.push_back(localMatrix);
		outputBuffer.push_back(texturePaths);
		outputBuffer.push_back(textureUniforms);
		outputBuffer.push_back(textureMapNames);
		outputBuffer.push_back(shaderProgramName);
		outputBuffer.push_back(preDrawFunName);
		outputBuffer.push_back("#endsceneNode\n");
	}


#pragma endregion

#pragma region writeToFile
	ofstream ofile;
	ofile.open("../Saves/" + scene->getId() + ".txt", ios::out);
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

#pragma region resetVariables
	allNodes = vector<SceneNode*>();
#pragma endregion
}

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


vector<string> SceneFileHandler::split(const string& s) {
	vector<string> elems;
	for (size_t p = 0, q = 0; p != s.npos; p = q)
		elems.push_back(s.substr(p + (p != 0), (q = s.find(" ", p + 1)) - p - (p != 0)));
	return elems;
}

vector<Camera*> SceneFileHandler::getCamera(SceneGraph* scene) {
	typename map<Camera*, SceneGraph*>::iterator it = sceneCameras.begin();

	vector<Camera*> cameras;

	while (it != sceneCameras.end()) {

		if (it->second->getId() == scene->getId()) {
			cameras.push_back(it->first);
		}
		it++;
	}

	return cameras;
}

SceneNode* SceneFileHandler::createSceneNode(string line) {

	#pragma region nodeVars
	static int nodeIndex = -1;
	static int parentIndex = -1;
	static int stencilIndex = 0;
	static SceneNode::NodeType nodeType = SceneNode::NodeType::NORMAL;
	static bool hasSceneGraph = false;
	static string meshPath = "";
	static string meshMapName = "";
	static Matrix4d localMatrix = Matrix4d();
	static string texturePaths = "";
	static string textureUniforms = "";
	static string textureMapNames = "";
	static string shaderProgramName = "";
	static string preDrawFunName = "";
	#pragma endregion

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
	else if (lineElements[0].compare("meshMapName") == 0) {
		meshMapName = lineElements[1];
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
	else if (lineElements[0].compare("textureUniforms") == 0) {

		textureUniforms = "";
		for(int i = 1; i < lineElements.size(); i++) {
			textureUniforms = textureUniforms + lineElements[i] + " ";
		}
	}
	else if (lineElements[0].compare("textureMapNames") == 0) {

		textureMapNames = "";
		for(int i = 1; i < lineElements.size(); i++) {
			textureMapNames = textureMapNames + lineElements[i] + " ";
		}
	}
	else if (lineElements[0].compare("shaderProgramName") == 0) {
		shaderProgramName = lineElements[1];
	}
	else if (lineElements[0].compare("preDrawFunName") == 0) {
		preDrawFunName = lineElements[1];
	}
	else if (line.compare("#endsceneNode") == 0) {
		
#pragma region createNode
		ShaderProgram* shader = ShaderProgramManager::getInstance()->get(shaderProgramName);
		vector<TextureInfo*> textures;

		vector<string> texturesP = split(texturePaths);
		vector<string> texturesUni = split(textureUniforms);
		vector<string> texturesMapNames = split(textureMapNames);

		if (texturesP[0].compare("NONE") != 0) {

			for (int i = 0; i < texturesP.size(); i++) {

				string textureString = texturesP[i];				
				if (textureString == "") { continue; }

				TextureInfo* textureInfo;
				map<string, TextureInfo*>::iterator it = loadedTextures.find(textureString);
				if (it != loadedTextures.end()) { //We already have a textureInfo with this name
					textureInfo = it->second;
				}
				else { // We don't have a textureInfo
					if (textureString.find('/') != string::npos) {//if its a path, we create it ourselves

						Texture* texture = TextureManager::getInstance()->get(texturesMapNames[i]);
						if (texture == NULL) {
							//TODO 3D textures?
							texture = new Texture2D();
							((Texture2D*)texture)->load(textureString);
							TextureManager::getInstance()->add(texturesMapNames[i], texture);
						}
						textureInfo = new TextureInfo(GL_TEXTURE1, 1, texturesUni[i], texture);
					}
					else { 
						//If we don't have a path, we assume the texture is a default asset from the application, and is therefore loaded
						textureInfo = new TextureInfo(GL_TEXTURE0, 0, texturesUni[i],
							TextureManager::getInstance()->get(texturesMapNames[i]));
					}
				}

				textures.push_back(textureInfo);
			}
		}
		//---------------Node------------------
		SceneNode* node = NULL;

		if (nodeType == SceneNode::NodeType::NORMAL) { node = new SceneNode(); }
		else if (nodeType == SceneNode::NodeType::OUTLINE) { node = new OutlineSceneNode(); }
		else if (nodeType == SceneNode::NodeType::SLIDEPUZZLE) { node = new SlidePuzzleSceneNode(stencilIndex); }

		if(parentIndex != -1){ node->setParent(sceneNodes[parentIndex]);}

		if(meshPath != "NONE"){ 
			Mesh* mesh = MeshManager::getInstance()->get(meshMapName);
			if (mesh == NULL){
				mesh = new Mesh();
				mesh->init(meshPath);
				MeshManager::getInstance()->add(meshMapName, mesh);
			}
			node->setMesh(mesh);
		}
		
		node->setMatrix(localMatrix);

		if (shader != NULL) {

			node->setShaderProgram(shader);
		}

		for (int i = 0; i < textures.size(); i++) {
			node->addTexture(textures.at(i));
		}

		if (hasSceneGraph) { node->setSceneGraph(sceneGraph); }

		if (preDrawFunName.compare("NONE") != 0) {
			node->setPreDrawFun(PreDrawFunctionManager::getInstance()->get(preDrawFunName));
		}

		sceneNodes.insert(pair<int, SceneNode*>(nodeIndex, node));
		return node;
#pragma endregion

	}

	return NULL;
}

SceneGraph* SceneFileHandler::createSceneGraph(string line) {

	static string sceneGraphMapName = "";

	vector<string> lineElements = split(line);

	if (lineElements[0].compare("sceneGraphMapName") == 0) {
		sceneGraphMapName = lineElements[1];
	}
	else if (line.compare("#endsceneGraph") == 0) {
		SceneGraph* sceneGraph = new SceneGraph();
		SceneGraphManager::getInstance()->add(sceneGraphMapName, sceneGraph);
		return sceneGraph;
	}

	return NULL;
}

pair<string,ShaderProgram*> SceneFileHandler::createShaderProgram(string line) {

	static string shaderProgramMapName = "";
	static string vertexShaderPath = "";
	static string fragmentShaderPath = "";
	static string shaderUniforms = "";
	static string shaderAtributes = "";

	vector<string> lineElements = split(line);

	if (lineElements[0].compare("shaderProgramMapName") == 0) {
		shaderProgramMapName = lineElements[1];
	}
	else if (lineElements[0].compare("vertexShaderPath") == 0) {
		vertexShaderPath = lineElements[1];
	}
	else if (lineElements[0].compare("shaderUniforms") == 0) {
		shaderUniforms = "";
		for (int i = 1; i < lineElements.size(); i++) {
			shaderUniforms = shaderUniforms + lineElements[i] + " ";
		}
	}
	else if (lineElements[0].compare("shaderAtributes") == 0) {
		shaderAtributes = "";
		for (int i = 1; i < lineElements.size(); i++) {
			shaderAtributes = shaderAtributes + lineElements[i] + " ";
		}
	}
	else if (lineElements[0].compare("fragmentShaderPath") == 0) {
		fragmentShaderPath = lineElements[1];
	}
	else if (line.compare("#endshaderProgram") == 0) {
		ShaderProgram* shaderProgram = new ShaderProgram();

		vector<string> shaderUnis = split(shaderUniforms);
		if (shaderUnis[0].compare("NONE") != 0) {
			for (int i = 0; i < shaderUnis.size(); i++) {
				string uniformString = shaderUnis[i];
				if (uniformString == "") { continue; }
				shaderProgram->addUniform(uniformString);
			}
		}

		vector<string> shaderAtt = split(shaderAtributes);
		if (shaderAtt[0].compare("NONE") != 0) {
			for (int i = 0; i < shaderAtt.size(); i++) {
				string atributeString = shaderAtt[i];
				if (atributeString == "") { continue; }
				//shaderProgram->addAttribute(atributeString);
			}
		}

		shaderProgram->init(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
		ShaderProgramManager::getInstance()->add(shaderProgramMapName, shaderProgram);

		return pair<string,ShaderProgram*>(shaderProgramMapName, shaderProgram);
	}

	return pair<string, ShaderProgram*>("", NULL);
}

Camera* SceneFileHandler::createCamera(string line) {

	static Vector3d eulerAngles = Vector3d();
	static Camera::CameraType projectionType;
	static Camera::RotationMode rotationType;
	static Vector3d translationVector = Vector3d();

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

void SceneFileHandler::parseLine(string line) {

	if (line.compare("#camera") == 0) { currentObjType = CurrentObjType::Camera; }
	else if (line.compare("#shaderProgram") == 0) { currentObjType = CurrentObjType::ShaderProgram; }
	else if (line.compare("#sceneGraph") == 0) { currentObjType = CurrentObjType::SceneGraph; }
	else if (line.compare("#sceneNode") == 0) { currentObjType = CurrentObjType::SceneNode; }

	switch (currentObjType) {
		case CurrentObjType::None:
		break;
		case CurrentObjType::Camera:
		{
			Camera* camera = createCamera(line);
			if (camera != NULL) { 
				sceneCamera = camera; 
				currentObjType = CurrentObjType::None;

			}
		}
		break;
		case CurrentObjType::ShaderProgram:
		{
			pair<string, ShaderProgram*> shaderProgram = createShaderProgram(line);
			if (shaderProgram.second != NULL) { currentObjType = CurrentObjType::None; }
		}
		break;
		case CurrentObjType::SceneGraph:
		{
			SceneGraph* scGrph = createSceneGraph(line);
			if (scGrph != NULL) {
				sceneGraph = scGrph;
				sceneGraph->setCamera(sceneCamera);
				sceneCameras.insert(make_pair(sceneCamera, scGrph));
				sceneGraph->init();
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


SceneGraph* SceneFileHandler::loadScene(string sceneId) {

	if (_mkdir(saves_dir.c_str()) == ENOENT) {
		cout << "Could not load scene. Path not found.";
		return NULL;
	}

#pragma region resetVariables
	currentObjType = CurrentObjType::None;
	sceneCamera = NULL;
	sceneGraph = NULL;
	sceneNodes = map<int, SceneNode*>();
	loadedTextures = map<string, TextureInfo*>();
	sceneCameras = map<Camera*, SceneGraph*>();
#pragma endregion



	ifstream ifile("../Saves/" + sceneId + ".txt");
	string line;
	if (!ifile) {
		cout << "Scene not loaded!";
		return NULL;
	}
	while (std::getline(ifile, line))
	{
		parseLine(line);
	}

	sceneGraph->setRoot(sceneNodes[0]);
	cout << "Scene loaded successfully!\n";
	return SceneGraphManager::getInstance()->get(sceneId);
}



