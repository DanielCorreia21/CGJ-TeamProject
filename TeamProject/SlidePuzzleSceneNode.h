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
	void duringDraw();

public:
	int stencil_index;
	SlidePuzzleSceneNode(int stencil_index);
};

