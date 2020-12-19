#include "OutlineSceneNode.h"
#include "MatrixFactory.h"

OutlineSceneNode::OutlineSceneNode() : SceneNode()
{

}

void setBlackColor() {
	//glUniform4f(shaders.getUniformColorId(), 0, 1, 0, 0);
	std::map<std::string, ShaderProgram::UniformInfo>::iterator it = ShaderProgramManager::getInstance()->get("main")->uniforms.find("Color");
	if (it != ShaderProgramManager::getInstance()->get("main")->uniforms.end()) {
		glUniform4f(it->second.index, 0, 0, 0, 0);
	}
}

//void OutlineSceneNode::duringDraw()
//{
//	//SceneNode::draw();
//
//	//NORMAL OBJECT
//	ShaderProgram* shaders = this->getShader();
//	Matrix4d modelMatrix = this->getModelMatrix();
//	float opengl_model_matrix[16];
//	glDisable(GL_CULL_FACE);
//	//scale down a bit
//	modelMatrix = modelMatrix * MatrixFactory::scalingMatrix(0.98f);
//	//prepare draw
//	modelMatrix.toColumnMajorArray(opengl_model_matrix);
//	glUniformMatrix4fv(shaders->getUniformId(), 1, GL_FALSE, opengl_model_matrix);
//	//draw 
//	this->mesh->draw();
//	
//	//BLACK OUTLINE
//	glEnable(GL_CULL_FACE);
//	glCullFace(GL_FRONT);
//	
//	//Scale
//	modelMatrix = this->getModelMatrix();
//	//Set black color
//	setBlackColor();
//	
//	//prepare draw
//	modelMatrix.toColumnMajorArray(opengl_model_matrix);
//	glUniformMatrix4fv(shaders->getUniformId(), 1, GL_FALSE, opengl_model_matrix);
//	//draw
//	this->mesh->draw();
//}

void OutlineSceneNode::duringDraw()
{
	glDisable(GL_CULL_FACE);
	Matrix4d initialMatrix = this->matrix;
	this->matrix *= MatrixFactory::scalingMatrix(0.98f);
	SceneNode::duringDraw();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	this->matrix = initialMatrix;
	setBlackColor();
	SceneNode::duringDraw();
}