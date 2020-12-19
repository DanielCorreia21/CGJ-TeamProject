#include "SceneNode.h"
#include "SceneGraph.h"
#include "MatrixFactory.h"

/*	AUTHORS
*	Group: 11
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*/

Matrix4d SceneNode::getModelMatrix()
{
	Matrix4d result = this->matrix.copy();
	if (this->parent != nullptr) {
		result = this->parent->getModelMatrix() * result;
	}
	return result;
}

ShaderProgram* SceneNode::getShader()
{
	//caso nos nao tivermos shader vemos se o no pai tem shader
	if (this->shader == nullptr && this->parent != nullptr) {
		return this->parent->getShader();
	}
	return this->shader;
}

SceneGraph* SceneNode::getSceneGraph()
{
	if (this->sceneGraph == nullptr && this->parent != nullptr) {
		return this->parent->getSceneGraph();
	}
	return this->sceneGraph;
}

SceneNode::SceneNode(SceneNode* parent)
{
	this->parent = parent;
	this->matrix = MatrixFactory::identityMatrix4d();
}

SceneNode::SceneNode()
{
	this->matrix = MatrixFactory::identityMatrix4d();
}

void SceneNode::setParent(SceneNode* parent)
{
	//this->setSceneGraph(parent->sceneGraph);
	//this->setShaderProgram(parent->shader);

	//TODO if parent exists
	this->parent = parent;
	parent->children.push_back(this);
}

void SceneNode::setSceneGraph(SceneGraph* sceneGraph)
{
	this->sceneGraph = sceneGraph;
}

void SceneNode::setMesh(Mesh* mesh)
{
	this->mesh = mesh;
}

void SceneNode::setMatrix(Matrix4d matrix)
{
	this->matrix = matrix;
}

void SceneNode::setShaderProgram(ShaderProgram* shader)
{
	this->shader = shader;
}

void SceneNode::setPreDrawFun(fptr f)
{
	this->preDrawFun = f;
}

std::vector<SceneNode*> SceneNode::getChildren()
{
	return this->children;
}

void SceneNode::preDraw()
{

	this->getShader()->bind();
	this->mesh->bind();

	//pre-draw function
	if (this->preDrawFun != nullptr) {
		this->preDrawFun();
	}
}

void SceneNode::duringDraw()
{
	float opengl_model_matrix[16];
	this->getModelMatrix().toColumnMajorArray(opengl_model_matrix);
	glUniformMatrix4fv(this->getShader()->getUniformId(), 1, GL_FALSE, opengl_model_matrix);
}

void SceneNode::postDraw()
{
	this->mesh->draw();

	this->mesh->unbind();
	this->getShader()->unbind();
}

void SceneNode::draw()
{
	for (SceneNode* child : this->children) {
		child->draw();
	}
	if (this->mesh == nullptr) {
		return;
	}

	preDraw();
	duringDraw();
	postDraw();
}
