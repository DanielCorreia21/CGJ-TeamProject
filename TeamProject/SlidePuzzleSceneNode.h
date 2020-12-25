#pragma once
#include "ShaderProgram.h"
#include "OutlineSceneNode.h"

/*	AUTHORS
*	Group: 4
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*	Antoine Pontallier - 98316
*	André Santos - 91000
*/
class SlidePuzzleSceneNode : public OutlineSceneNode
{
protected:
	int stencil_index;
	void duringDraw();

public:
	SlidePuzzleSceneNode(int stencil_index);
};

