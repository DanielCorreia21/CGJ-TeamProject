#include "OutlineSceneNode.h"
#include "MatrixFactory.h"

OutlineSceneNode::OutlineSceneNode()
{

}

void setBlackColor() {
	//glUniform4f(shaders.getUniformColorId(), 0, 1, 0, 0);
	std::map<std::string, ShaderProgram::UniformInfo>::iterator it = ShaderProgramManager::getInstance()->get("main")->uniforms.find("Color");
	if (it != ShaderProgramManager::getInstance()->get("main")->uniforms.end()) {
		glUniform4f(it->second.index, 0, 0, 0, 0);
	}
}

void OutlineSceneNode::draw()
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

	//------------
	Matrix4d modelMatrix = this->getModelMatrix();
	float opengl_model_matrix[16];

	//NORMAL OBJECT
	glDisable(GL_CULL_FACE);
	//scale down a bit
	modelMatrix = modelMatrix * MatrixFactory::scalingMatrix(0.98f);
	//set black color
	if (this->preDrawFun != nullptr) {
		this->preDrawFun();
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
	setBlackColor();

	//prepare draw
	modelMatrix.toColumnMajorArray(opengl_model_matrix);
	glUniformMatrix4fv(shaders->getUniformId(), 1, GL_FALSE, opengl_model_matrix);
	//draw
	this->mesh->draw();
	//------------

	this->mesh->unbind();
	shaders->unbind();
}
