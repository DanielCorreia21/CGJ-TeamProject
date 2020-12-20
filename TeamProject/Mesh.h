#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Manager.h"

/*	AUTHORS
*	Group: 4
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*	Antoine Pontallier - 98316
*	André Santos - 91000
*/

class Mesh
{
	public:

		Mesh();
		~Mesh();
		void init(std::string& filename);
		void bind();
		void unbind();
		void draw();

	private:
		void createMeshBufferObjects();
		void destroyMeshBufferObjects();

		void loadMeshData(std::string& filename);
		void processMeshData();
		void freeMeshData();
		void parseLine(std::stringstream& sin);
		void parseFace(std::stringstream& sin);
		void parseNormal(std::stringstream& sin);
		void parseTexcoord(std::stringstream& sin);
		void parseVertex(std::stringstream& sin);
};

class MeshManager : public Manager<Mesh> {};