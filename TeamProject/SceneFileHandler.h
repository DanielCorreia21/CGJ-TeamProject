#pragma once
#include "SceneGraph.h"
#include "Texture2D.h"

class SceneFileHandler
{
public:
	static SceneFileHandler* instance;
	static SceneFileHandler* getInstance();
	void saveScene(SceneGraph* scene);
	SceneGraph* loadScene(string sceneId);
	vector<Camera*> getCamera(SceneGraph* scene);
private:
	SceneFileHandler();
	map<Camera*, SceneGraph*> sceneCameras;
#pragma region SaveHelperMethods
	int getIndexFromNode(map<int, SceneNode*> nodeMap, SceneNode* node);
	vector<SceneNode*> getAllNodes(SceneNode* node);
#pragma endregion

#pragma region LoadHelperVars
	enum class CurrentObjType {
		None,
		Camera,
		ShaderProgram,
		SceneGraph,
		SceneNode
	};
	CurrentObjType currentObjType = CurrentObjType::None;
	Camera* sceneCamera = NULL;
	SceneGraph* sceneGraph = NULL;
	map<int, SceneNode*> sceneNodes;
	map<string, TextureInfo*> loadedTextures;
#pragma endregion


#pragma region LoadHelperMethods
	vector<string> split(const string& s); //Split string by space into vector
	SceneNode* createSceneNode(string line);
	SceneGraph* createSceneGraph(string line);
	pair<string, ShaderProgram*> createShaderProgram(string line);
	Camera* createCamera(string line);
	void parseLine(string line);
#pragma endregion

};

