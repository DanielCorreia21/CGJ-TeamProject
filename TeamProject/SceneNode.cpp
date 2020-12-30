#include "SceneNode.h"
#include "SceneGraph.h"
#include "MatrixFactory.h"

/*	AUTHORS
*	Group: 4
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*	Antoine Pontallier - 98316
*	André Santos - 91000
*/

Matrix4d SceneNode::getModelMatrix()
{
	Matrix4d result = this->matrix.copy();
	if (this->parent != nullptr) {
		result = this->parent->getModelMatrix() * result;
	}
	return result;
}

Matrix4d SceneNode::getMatrix(){
	return this->matrix.copy();
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
	this->nodeType = NodeType::NORMAL;
	this->parent = parent;
	this->matrix = MatrixFactory::identityMatrix4d();
}

SceneNode::SceneNode()
{
	this->nodeType = NodeType::NORMAL;
	this->matrix = MatrixFactory::identityMatrix4d();
}

void SceneNode::setParent(SceneNode* parent)
{
	//TODO if parent exists
	this->parent = parent;
	parent->children.push_back(this);
}

SceneNode* SceneNode::getParent() {
	return this->parent;
}

void SceneNode::setSceneGraph(SceneGraph* sceneGraph)
{
	this->sceneGraph = sceneGraph;
}

SceneGraph* SceneNode::getLocalSceneGraph() {
	return this->sceneGraph;
}

void SceneNode::setMesh(Mesh* mesh)
{
	this->mesh = mesh;
}

Mesh* SceneNode::getMesh()
{
	return this->mesh;
}

void SceneNode::setMatrix(Matrix4d matrix)
{
	this->matrix = matrix;
}

void SceneNode::setShaderProgram(ShaderProgram* shader)
{
	this->shader = shader;
}

ShaderProgram* SceneNode::getLocalShaderProgram() {

	return this->shader;
}

void SceneNode::setPreDrawFun(fptr f)
{
	this->preDrawFun = f;
}

void SceneNode::setTextures(std::vector<TextureInfo*> n_textures)
{
	this->textures = n_textures;
}

void SceneNode::setPostDrawFun(fptr f)
{
	this->postDrawFun = f;
}

std::vector<SceneNode*> SceneNode::getChildren()
{
	return this->children;
}

std::vector<TextureInfo*> SceneNode::getTextures()
{
	return this->textures;
}

void SceneNode::addTexture(TextureInfo* texture)
{
	this->textures.push_back(texture);
}

void SceneNode::preDraw()
{
	ShaderProgram* shaders = this->getShader();
	shaders->bind();

	if (textures.empty() == false) {
		for (auto& t : textures) {
			t->updateShader(shaders);
		}
	}

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
	glUniformMatrix4fv(this->getShader()->getModelMatrixIndex(), 1, GL_FALSE, opengl_model_matrix);
	this->mesh->draw();
}

void SceneNode::postDraw()
{
	if (this->postDrawFun != nullptr) {
		this->postDrawFun();
	}
	this->mesh->unbind();

	if (textures.empty() == false) {
		for (auto& t : textures) {
			t->texture->unbind();
		}
	}

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
