#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

/*	AUTHORS
*	Group: 11
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*/

class Mesh
{
	public:

		Mesh();
		//~Mesh();
		void bind();
		void unbind();
		void draw();
		const void createMesh(std::string& filename);
		void createMeshBufferObjects();
		void destroyMeshBufferObjects();

	private:

		void loadMeshData(std::string& filename);
		void processMeshData();
		void freeMeshData();
		void parseLine(std::stringstream& sin);
		void parseFace(std::stringstream& sin);
		void parseNormal(std::stringstream& sin);
		void parseTexcoord(std::stringstream& sin);
		void parseVertex(std::stringstream& sin);
};

