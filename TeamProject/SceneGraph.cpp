#include "SceneGraph.h"

/*	AUTHORS
*	Group: 4
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*	Antoine Pontallier - 98316
*	André Santos - 91000
*/

SceneGraph::SceneGraph()
{
	this->root = new SceneNode();
	this->root->setSceneGraph(this);
	this->camera = nullptr;
}

SceneGraph::~SceneGraph()
{
	std::cout << "Destroying SceneGraph\n";
	this->destroy();
}

void SceneGraph::setCamera(Camera* camera)
{
	this->camera = camera;
}

Camera* SceneGraph::getCamera()
{
	return this->camera;
}

SceneNode* SceneGraph::getRoot()
{
	return this->root;
}

void SceneGraph::init(ShaderProgram* shaders)
{
	this->camera->initBuffer(shaders);
}

void SceneGraph::destroy()
{
	this->camera->destroyBuffer();
}

void SceneGraph::draw()
{
	this->camera->draw();
	this->root->draw();
}
