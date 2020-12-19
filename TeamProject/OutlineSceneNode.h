#pragma once
#include "SceneNode.h"
#include "ShaderProgram.h"

class OutlineSceneNode : public SceneNode
{
protected:
	void duringDraw();
public:
	OutlineSceneNode();
};

