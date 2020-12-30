#include "OutlineSceneNode.h"
#include "MatrixFactory.h"

/*	AUTHORS
*	Group: 4
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*	Antoine Pontallier - 98316
*	André Santos - 91000
*/

OutlineSceneNode::OutlineSceneNode() : SceneNode()
{
	this->nodeType = NodeType::OUTLINE;
}

void setBlackColor() {

	ShaderProgram::UniformInfo* uniInfo = ShaderProgramManager::getInstance()->get("color")->getUniform("Color");
	glUniform4f(uniInfo->index, 0, 0, 0, 0);
}

void setDifferentColor() {
	ShaderProgram* sp = ShaderProgramManager::getInstance()->get("color");
	ShaderProgram::UniformInfo* uniInfo = sp->getUniform("Color");
	glUniform4f(uniInfo->index, 0, 0, 0, 1);
}

void OutlineSceneNode::duringDraw()
{

	glDisable(GL_CULL_FACE);
	Matrix4d initialMatrix = this->matrix;
	setDifferentColor();
	this->matrix *= MatrixFactory::scalingMatrix(0.98f);
	SceneNode::duringDraw();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	this->matrix = initialMatrix;
	setBlackColor();
	SceneNode::duringDraw();
	glDisable(GL_CULL_FACE);
}