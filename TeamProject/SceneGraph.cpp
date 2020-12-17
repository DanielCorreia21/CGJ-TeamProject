#include "SceneGraph.h"

/*	AUTHORS
*	Group: 11
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*/

SceneGraph::SceneGraph()
{
	this->root = new SceneNode();
	this->root->setSceneGraph(this);
	this->camera = nullptr;
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

void SceneGraph::init(ShaderProgram shaders)
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
