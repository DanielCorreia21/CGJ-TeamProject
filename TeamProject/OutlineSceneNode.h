#pragma once
#include "SceneNode.h"
#include "ShaderProgram.h"

/*	AUTHORS
*	Group: 4
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*	Antoine Pontallier - 98316
*	André Santos - 91000
*/

class OutlineSceneNode : public SceneNode
{
protected:
	void duringDraw();
public:
	OutlineSceneNode();
};

