#pragma once
#include "SceneGraph.h"
#include "Texture2D.h"

class SceneFileHandler
{
public:
	SceneFileHandler();
	void saveScene(SceneGraph* scene);
	void loadScene();//TODO with a name
	Camera* getCamera();
private:
	int getIndexFromNode(map<int, SceneNode*> nodeMap, SceneNode* node);
	vector<SceneNode*> getAllNodes(SceneNode* node);

};

