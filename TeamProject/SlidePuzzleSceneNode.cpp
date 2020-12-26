#include "SlidePuzzleSceneNode.h"

SlidePuzzleSceneNode::SlidePuzzleSceneNode(int stencil_index) : OutlineSceneNode()
{
	this->stencil_index = stencil_index;
}

void SlidePuzzleSceneNode::duringDraw()
{
	glStencilFunc(GL_ALWAYS, this->stencil_index, -1);
	OutlineSceneNode::duringDraw();
	glStencilFunc(GL_ALWAYS, 0, -1);
}