#pragma once
#include "Mesh.h"
#include "Matrix4d.h"
#include "ShaderProgram.h"
#include <vector>

/*	AUTHORS
*	Group: 11
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
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
	fptr postDrawFun = nullptr;

	ShaderProgram* getShader();
	SceneGraph* getSceneGraph();

	virtual void preDraw();
	virtual void duringDraw();
	virtual void postDraw();
public:
	Matrix4d getModelMatrix();
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
	void setPostDrawFun(fptr f);
	std::vector<SceneNode*> getChildren();
	void draw();
};

