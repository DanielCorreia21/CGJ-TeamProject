#pragma once
#include "SceneNode.h"
#include "Camera.h"
#include "ShaderProgram.h"
#include "Manager.h"

/*	AUTHORS
*	Group: 4
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*	Antoine Pontallier - 98316
*	André Santos - 91000
*/

class SceneGraph
{
private:
	SceneNode *root;
	Camera *camera;
	void destroy();
public:
	SceneGraph();
	~SceneGraph();
	void setCamera(Camera *camera);
	Camera *getCamera();
	SceneNode *getRoot();
	void init();
	void draw();
};

class SceneGraphManager : public Manager<SceneGraph> {};