#pragma once
#include "SceneGraph.h"

class SceneFileHandler
{
public:
	SceneFileHandler();
	void saveScene(SceneGraph* scene);
	int getIndexFromNode(map<int, SceneNode*> nodeMap, SceneNode* node);
	vector<SceneNode*> getAllNodes(SceneNode* node);
};

