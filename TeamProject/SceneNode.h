#pragma once
#include "Mesh.h"
#include "Matrix4d.h"
#include "ShaderProgram.h"
#include <vector>
#include "TextureInfo.h"

/*	AUTHORS
*	Group: 4
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*	Antoine Pontallier - 98316
*	André Santos - 91000
*/

class SceneGraph;
typedef void (*fptr)();

class SceneNode
{
protected:
	SceneGraph *sceneGraph = nullptr;
	SceneNode *parent = nullptr;
	Mesh* mesh = nullptr;
	Matrix4d matrix;
	std::vector<SceneNode*> children;
	ShaderProgram *shader = nullptr;
	fptr preDrawFun = nullptr;
	std::vector<TextureInfo*> textures;
	fptr postDrawFun = nullptr;

	ShaderProgram* getShader();
	SceneGraph* getSceneGraph();

	virtual void preDraw();
	virtual void duringDraw();
	virtual void postDraw();
public:
	Matrix4d getModelMatrix();
	Matrix4d getMatrix();
	SceneNode(SceneNode* parent);
	SceneNode();
	void setParent(SceneNode* parent);
	//SceneNode *createNode();
	//void addNode(SceneNode *node);
	void setSceneGraph(SceneGraph *sceneGraph);
	void setMesh(Mesh* mesh);
	void setMatrix(Matrix4d matrix);
	void setShaderProgram(ShaderProgram* shader);
	void setPreDrawFun(fptr f);
	void setTextures(std::vector<TextureInfo*> textures);
	std::vector<SceneNode*> getChildren();
	std::vector<TextureInfo*> getTextures();
	void addTexture(TextureInfo* texture);
	void setPostDrawFun(fptr f);
	virtual void draw();
};

