#pragma once
#include "SceneNode.h"
#include "Camera.h"
#include "ShaderProgram.h"
#include "Manager.h"

/*	AUTHORS
*	Group: 11
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*/

class SceneGraph
{
private:
	SceneNode *root;
	Camera *camera;
public:
	SceneGraph();
	void setCamera(Camera *camera);
	Camera *getCamera();
	SceneNode *getRoot();
	void init(ShaderProgram shaders);
	void destroy();
	void draw();
};

class SceneGraphManager : public Manager<SceneGraph> {};