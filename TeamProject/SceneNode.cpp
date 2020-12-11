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

fptr SceneNode::getPreDrawFun()
{
	if (this->preDrawFun == nullptr && this->parent != nullptr) {
		return this->parent->getPreDrawFun();
	}
	return this->preDrawFun;
}

bool SceneNode::getShouldOutline() {
	if (!this->shouldOutline && this->parent != nullptr) {
		return this->parent->getShouldOutline();
	}
	return this-> shouldOutline;
}

SceneNode::SceneNode(SceneNode* parent)
{
	this->parent = parent;
	this->matrix = MatrixFactory::identityMatrix4d();
	this->shouldOutline = false;
}

SceneNode* SceneNode::createNode()
{
	SceneNode* result = new SceneNode(this);
	result->setSceneGraph(this->sceneGraph);
	result->setShaderProgram(this->shader);
	this->children.push_back(result);
	return result;
}

void SceneNode::addNode(SceneNode* node)
{
	this->children.push_back(node);
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

void SceneNode::outline(ShaderProgram* shaders) {
	Matrix4d modelMatrix = this->getModelMatrix();
	float opengl_model_matrix[16];

	//NORMAL OBJECT
	glDisable(GL_CULL_FACE);
	//scale down a bit
	modelMatrix = modelMatrix * MatrixFactory::scalingMatrix(0.98f);
	//set black color
	if (this->getPreDrawFun() != nullptr) {
		this->getPreDrawFun()();
	}

	//prepare draw
	modelMatrix.toColumnMajorArray(opengl_model_matrix);
	glUniformMatrix4fv(shaders->getUniformId(), 1, GL_FALSE, opengl_model_matrix);
	//draw 
	this->mesh->draw();

	//BLACK OUTLINE
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	//Scale
	modelMatrix = this->getModelMatrix();
	//Set normal color
	glUniform4f(shaders->getUniformColorId(), 0, 0, 0, 0);

	//prepare draw
	modelMatrix.toColumnMajorArray(opengl_model_matrix);
	glUniformMatrix4fv(shaders->getUniformId(), 1, GL_FALSE, opengl_model_matrix);
	//draw
	this->mesh->draw();
}

void SceneNode::draw()
{
	for (SceneNode* child : this->children) {
		child->draw();
	}

	if (this->mesh == nullptr) {
		return;
	}

	ShaderProgram* shaders = this->getShader();
	shaders->bind();
	this->mesh->bind();


	if (this->getShouldOutline()) {
		this->outline(shaders);
	}
	else {

		//set color
		if (this->getPreDrawFun() != nullptr) {
			this->getPreDrawFun()();
		}

		Matrix4d modelMatrix = this->getModelMatrix();
		float opengl_model_matrix[16];

		//prepare draw
		modelMatrix.toColumnMajorArray(opengl_model_matrix);
		glUniformMatrix4fv(shaders->getUniformId(), 1, GL_FALSE, opengl_model_matrix);
		//draw 
		this->mesh->draw();
	}


	this->mesh->unbind();
	shaders->unbind();
}
