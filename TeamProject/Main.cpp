///////////////////////////////////////////////////////////////////////////////
//
// Drawing two instances of a triangle in Clip Space.
// A "Hello 2D World" of Modern OpenGL.
//
// (c)2013-20 by Carlos Martinho
//
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "Matrix2d.h"
#include "Matrix3d.h"
#include "MatrixFactory.h"
#include "Vector4d.h"
#include "ShaderProgram.h"
#include "GLStatics.h"
#include "Camera.h"
#include "util.h"
#include "SceneGraph.h"
#include "SceneNode.h"
#include "Mesh.h"
#include "ErrorHandler.h"
#include "Texture.h"
#include "TextureInfo.h"
#include "Texture2D.h"
#include "Texture3D.h"
#include "OutlineSceneNode.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

/*	MODIFIED BY
*	Group: 4
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*	Antoine Pontallier - 98316
*	André Santos - 91000
*/

const char colorVertexShaderPath[] = "../Resources/color_vs.glsl";
const char colorFragmentShaderPath[] = "../Resources/color_fs.glsl";
const char textureVertexShaderPath[] = "../Resources/marble_vs.glsl";
const char textureFragmentShaderPath[] = "../Resources/marble_fs.glsl";

const string SLIDING_PUZZLE_SCENE_GRAPH = "SlidingPuzzle";

const string COLOR_SHADER = "color";
const string PIECES_SHADER = "Marble Shader";

const string cubeMeshPath = "../objs/cube5.obj";
const string CUBE_MESH = "cube";

const string COLOR_UNIFORM = "Color";
const string TEXTURE_UNIFORM_COLOR = "Texture";
const string TEXTURE_UNIFORM_NOISE = "NoiseTexture";

Camera* camera;


///////////////////////////////////////////////////////////////////// PreDrawFunctions

void setRedColor() {
	ShaderProgram::UniformInfo* uniInfo = ShaderProgramManager::getInstance()->get(COLOR_SHADER)->getUniform(COLOR_UNIFORM);
	glUniform4f(uniInfo->index, 1, 0, 0, 0);
}

void setBlueColor() {
	ShaderProgram::UniformInfo* uniInfo = ShaderProgramManager::getInstance()->get(COLOR_SHADER)->getUniform(COLOR_UNIFORM);
	glUniform4f(uniInfo->index, 0, 0, 1, 0);
}

///////////////////////////////////////////////////////////////////// SCENE

void createTextures() {

	Texture3D* texture_0 = new Texture3D();
	texture_0->createPerlinNoise(128, 5, 5, 5, 2, 2, 8);
	TextureManager::getInstance()->add("marble", (Texture*)texture_0);

	Texture2D* texture_1 = new Texture2D();
	texture_1->load("../numbers/1.png");
	TextureManager::getInstance()->add("number_1", (Texture*)texture_1);

	Texture2D* texture_2 = new Texture2D();
	texture_2->load("../numbers/2.png");
	TextureManager::getInstance()->add("number_2", (Texture*)texture_2);

	Texture2D* texture_3 = new Texture2D();
	texture_3->load("../numbers/3.png");
	TextureManager::getInstance()->add("number_3", (Texture*)texture_3);

	Texture2D* texture_4 = new Texture2D();
	texture_4->load("../numbers/4.png");
	TextureManager::getInstance()->add("number_4", (Texture*)texture_4);

	Texture2D* texture_5 = new Texture2D();
	texture_5->load("../numbers/5.png");
	TextureManager::getInstance()->add("number_5", (Texture*)texture_5);

	Texture2D* texture_6 = new Texture2D();
	texture_6->load("../numbers/6.png");
	TextureManager::getInstance()->add("number_6", (Texture*)texture_6);

	Texture2D* texture_7 = new Texture2D();
	texture_7->load("../numbers/7.png");
	TextureManager::getInstance()->add("number_7", (Texture*)texture_7);

	Texture2D* texture_8 = new Texture2D();
	texture_8->load("../numbers/8.png");
	TextureManager::getInstance()->add("number_8", (Texture*)texture_8);
	
	//Texture2D* backb = new Texture2D();
	//backb->load("../numbers/backb.png");
	//TextureManager::getInstance()->add("backb", (Texture*)backb);

	//Texture2D* fram = new Texture2D();
	//fram->load("../numbers/fram.png");
	//TextureManager::getInstance()->add("fram", (Texture*)fram);

}

void createEnvironmentSceneGraph()
{
	Mesh* cubeMesh = MeshManager::getInstance()->get(CUBE_MESH);

	#pragma region backboard
	SceneNode* backboard = new SceneNode();
	backboard->setParent(SceneGraphManager::getInstance()->get(SLIDING_PUZZLE_SCENE_GRAPH)->getRoot());
	backboard->setPreDrawFun(setBlueColor);
	backboard->setMesh(cubeMesh);
	backboard->setMatrix(
		MatrixFactory::scalingMatrix(Vector3d(11.0f,11.0f,1.0f))
	);

#pragma endregion

	#pragma region frame


	SceneNode* frame = new SceneNode();
	frame->setParent(SceneGraphManager::getInstance()->get(SLIDING_PUZZLE_SCENE_GRAPH)->getRoot());
	frame->setPreDrawFun(setRedColor);
	frame->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, 0.0f, 0.8f))
	);

	SceneNode* frameUp = new SceneNode();
	frameUp->setParent(frame);
	frameUp->setPreDrawFun(setRedColor);
	frameUp->setMesh(cubeMesh);

	//UP Frame
	frameUp->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, 2.0f, 0.0f))
		* MatrixFactory::scalingMatrix(Vector3d(11.0f, 1.0f, 3.0f))
	);

	//Define positions and rotations of other frame components
	//Down
	SceneNode* frameDown = new SceneNode();
	frameDown->setParent(frame);
	frameDown->setPreDrawFun(setRedColor);
	frameDown->setMesh(cubeMesh);
	frameDown->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, -2.0f, 0.0f))
		* MatrixFactory::scalingMatrix(Vector3d(11.0f, 1.0f, 3.0f))
	);

	//Right
	SceneNode* frameRight = new SceneNode();
	frameRight->setParent(frame);
	frameRight->setPreDrawFun(setRedColor);
	frameRight->setMesh(cubeMesh);
	frameRight->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(2.0f, 0.0f, 0.0f))
		* MatrixFactory::rotateZMatrix(90)
		* MatrixFactory::scalingMatrix(Vector3d(11.0f, 1.0f, 3.0f))
	);

	//Left
	SceneNode* frameLeft = new SceneNode();
	frameLeft->setParent(frame);
	frameLeft->setPreDrawFun(setRedColor);
	frameLeft->setMesh(cubeMesh);
	frameLeft->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(-2.0f, 0.0f, 0.0f))
		* MatrixFactory::rotateZMatrix(90)
		* MatrixFactory::scalingMatrix(Vector3d(11.0f, 1.0f, 3.0f))
	);

#pragma endregion

	#pragma region pieces

	SceneNode* pieces = new SceneNode();
	pieces->setParent(SceneGraphManager::getInstance()->get(SLIDING_PUZZLE_SCENE_GRAPH)->getRoot());
	pieces->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, 0.0f, 0.8f))
		* MatrixFactory::scalingMatrix(3.0f)
	);


	TextureInfo* tinfo_0 = new TextureInfo(GL_TEXTURE0, 0, TEXTURE_UNIFORM_NOISE, TextureManager::getInstance()->get("marble"));
	ShaderProgram* piecesShader = ShaderProgramManager::getInstance()->get(PIECES_SHADER);
	//---------------Piece 1------------------
	SceneNode* piece1 = new OutlineSceneNode();
	piece1->setParent(pieces);
	piece1->setMesh(cubeMesh);
	piece1->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(-0.4f,0.4f,0.0f))
	);

	TextureInfo* tinfo_1 = new TextureInfo(GL_TEXTURE1, 1, TEXTURE_UNIFORM_COLOR, TextureManager::getInstance()->get("number_1"));
	piece1->setShaderProgram(piecesShader);
	piece1->addTexture(tinfo_0);
	piece1->addTexture(tinfo_1);

	//---------------Piece 2------------------
	SceneNode* piece2 = new OutlineSceneNode();
	piece2->setParent(pieces);
	piece2->setMesh(cubeMesh);
	piece2->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, 0.4f, 0.0f))
	);
	TextureInfo* tinfo_2 = new TextureInfo(GL_TEXTURE1, 1, TEXTURE_UNIFORM_COLOR, TextureManager::getInstance()->get("number_2"));
	piece2->setShaderProgram(piecesShader);
	piece2->addTexture(tinfo_0);
	piece2->addTexture(tinfo_2);

	//---------------Piece 3------------------
	SceneNode* piece3 = new OutlineSceneNode();
	piece3->setParent(pieces);
	piece3->setMesh(cubeMesh);
	piece3->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.4f, 0.4f, 0.0f))
	);
	TextureInfo* tinfo_3 = new TextureInfo(GL_TEXTURE1, 1, TEXTURE_UNIFORM_COLOR, TextureManager::getInstance()->get("number_3"));
	piece3->setShaderProgram(piecesShader);
	piece3->addTexture(tinfo_0);
	piece3->addTexture(tinfo_3);

	//---------------Piece 4------------------
	SceneNode* piece4 = new OutlineSceneNode();
	piece4->setParent(pieces);
	piece4->setMesh(cubeMesh);
	piece4->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(-0.4f, 0.0f, 0.0f))
	);
	TextureInfo* tinfo_4 = new TextureInfo(GL_TEXTURE1, 1, TEXTURE_UNIFORM_COLOR, TextureManager::getInstance()->get("number_4"));
	piece4->setShaderProgram(piecesShader);
	piece4->addTexture(tinfo_0);
	piece4->addTexture(tinfo_4);

	//---------------Piece 5------------------
	SceneNode* piece5 = new OutlineSceneNode();
	piece5->setParent(pieces);
	piece5->setMesh(cubeMesh);
	piece5->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, 0.0f, 0.0f))
	);
	TextureInfo* tinfo_5 = new TextureInfo(GL_TEXTURE1, 1, TEXTURE_UNIFORM_COLOR, TextureManager::getInstance()->get("number_5"));
	piece5->setShaderProgram(piecesShader);
	piece5->addTexture(tinfo_0);
	piece5->addTexture(tinfo_5);

	//---------------Piece 6------------------
	SceneNode* piece6 = new OutlineSceneNode();
	piece6->setParent(pieces);
	piece6->setMesh(cubeMesh);
	piece6->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.4f, 0.0f, 0.0f))
	);
	TextureInfo* tinfo_6 = new TextureInfo(GL_TEXTURE1, 1, TEXTURE_UNIFORM_COLOR, TextureManager::getInstance()->get("number_6"));
	piece6->setShaderProgram(piecesShader);
	piece6->addTexture(tinfo_0);
	piece6->addTexture(tinfo_6);

	//---------------Piece 7------------------
	SceneNode* piece7 = new OutlineSceneNode();
	piece7->setParent(pieces);
	piece7->setMesh(cubeMesh);
	piece7->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(-0.4f, -0.4f, 0.0f))
	);
	TextureInfo* tinfo_7 = new TextureInfo(GL_TEXTURE1, 1, TEXTURE_UNIFORM_COLOR, TextureManager::getInstance()->get("number_7"));
	piece7->setShaderProgram(piecesShader);
	piece7->addTexture(tinfo_0);
	piece7->addTexture(tinfo_7);

	//---------------Piece 8------------------
	SceneNode* piece8 = new OutlineSceneNode();
	piece8->setParent(pieces);
	piece8->setMesh(cubeMesh);
	piece8->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, -0.4f, 0.0f))
	);
	TextureInfo* tinfo_8 = new TextureInfo(GL_TEXTURE1, 1, TEXTURE_UNIFORM_COLOR, TextureManager::getInstance()->get("number_8"));
	piece8->setShaderProgram(piecesShader);
	piece8->addTexture(tinfo_0);
	piece8->addTexture(tinfo_8);

#pragma endregion
}


void createSceneGraph()
{
	Mesh* mesh = new Mesh();
	string s = string(cubeMeshPath);
	mesh->init(s);

	MeshManager::getInstance()->add(CUBE_MESH, mesh);


	SceneGraph* slidingPuzzleScenegraph = new SceneGraph();
	slidingPuzzleScenegraph->setCamera(camera);
	SceneGraphManager::getInstance()->add(SLIDING_PUZZLE_SCENE_GRAPH, slidingPuzzleScenegraph);

	SceneNode* n = slidingPuzzleScenegraph->getRoot();
	n->setShaderProgram(ShaderProgramManager::getInstance()->get(COLOR_SHADER));


	createEnvironmentSceneGraph();

	slidingPuzzleScenegraph->init(ShaderProgramManager::getInstance()->get(COLOR_SHADER));
}

void drawScene()
{
	SceneGraphManager::getInstance()->get(SLIDING_PUZZLE_SCENE_GRAPH)->draw();

#ifndef ERROR_CALLBACK
	ErrorHandler::checkOpenGLError("ERROR: Could not draw scene.");
#endif
}

///////////////////////////////////////////////////////////////////// CALLBACKS

void window_close_callback(GLFWwindow* win)
{
	delete ShaderProgramManager::getInstance();
	delete SceneGraphManager::getInstance();
	delete MeshManager::getInstance();
}

void window_size_callback(GLFWwindow* win, int winx, int winy)
{
	glViewport(0, 0, winx, winy);
}

void glfw_error_callback(int error, const char* description)
{
	std::cerr << "GLFW Error: " << description << std::endl;
}

void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(win, GLFW_TRUE);
		window_close_callback(win);
	}

	if (action == GLFW_PRESS || action == GLFW_RELEASE) {
		camera->updateCameraPos(key, action);
	}

	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		camera->changeProjectionType();
	}

	if (key == GLFW_KEY_G && action == GLFW_PRESS)
	{
		camera->changeRotationType();
	}

}

int mouse_pressed = 0;

void mouse_callback(GLFWwindow* win, double xpos, double ypos)
{
	camera->look((float) xpos, (float) ypos, mouse_pressed);

}

void mouse_button_callback(GLFWwindow* win, int button, int action, int mods)
{
	if (button == 0) {
		mouse_pressed = action;
	}
}

///////////////////////////////////////////////////////////////////////// SETUP

GLFWwindow* setupWindow(int winx, int winy, const char* title,
	int is_fullscreen, int is_vsync)
{
	GLFWmonitor* monitor = is_fullscreen ? glfwGetPrimaryMonitor() : 0;
	GLFWwindow* win = glfwCreateWindow(winx, winy, title, monitor, 0);
	if (!win)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(win);
	glfwSwapInterval(is_vsync);
	return win;
}

void setupCallbacks(GLFWwindow* win)
{
	glfwSetWindowCloseCallback(win, window_close_callback);
	glfwSetWindowSizeCallback(win, window_size_callback);

	glfwSetKeyCallback(win, key_callback);
	glfwSetCursorPosCallback(win, mouse_callback);
	glfwSetMouseButtonCallback(win, mouse_button_callback);
}

GLFWwindow* setupGLFW(int gl_major, int gl_minor,
	int winx, int winy, const char* title, int is_fullscreen, int is_vsync)
{
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_minor);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

	GLFWwindow* win = setupWindow(winx, winy, title, is_fullscreen, is_vsync);
	setupCallbacks(win);

#if _DEBUG
	std::cout << "GLFW " << glfwGetVersionString() << std::endl;
#endif
	return win;
}

void setupGLEW()
{
	glewExperimental = GL_TRUE;
	// Allow extension entry points to be loaded even if the extension isn't 
	// present in the driver's extensions string.
	GLenum result = glewInit();
	if (result != GLEW_OK)
	{
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	}
	GLenum err_code = glGetError();
	// You might get GL_INVALID_ENUM when loading GLEW.
}

void checkOpenGLInfo()
{
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	std::cerr << "OpenGL Renderer: " << renderer << " (" << vendor << ")" << std::endl;
	std::cerr << "OpenGL version " << version << std::endl;
	std::cerr << "GLSL version " << glslVersion << std::endl;
}

void setupOpenGL(int winx, int winy)
{
#if _DEBUG
	checkOpenGLInfo();
#endif
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glViewport(0, 0, winx, winy);
}

GLFWwindow* setup(int major, int minor,
	int winx, int winy, const char* title, int is_fullscreen, int is_vsync)
{
	GLFWwindow* win =
		setupGLFW(major, minor, winx, winy, title, is_fullscreen, is_vsync);
	setupGLEW();
	setupOpenGL(winx, winy);

	camera = new Camera(Vector3d(0, 0, -10), Vector3d(0, 0, -1), Vector3d(0, 1, 0));

	ShaderProgram* shaders = new ShaderProgram();
	shaders->addUniform(COLOR_UNIFORM);
	shaders->init(colorVertexShaderPath, colorFragmentShaderPath);
	ShaderProgramManager::getInstance()->add(COLOR_SHADER, shaders);


	ShaderProgram* g_shaders = new ShaderProgram();
	g_shaders->addUniform(COLOR_UNIFORM);
	g_shaders->addUniform(TEXTURE_UNIFORM_COLOR);
	g_shaders->addUniform(TEXTURE_UNIFORM_NOISE);

	g_shaders->init(textureVertexShaderPath, textureFragmentShaderPath);
	ShaderProgramManager::getInstance()->add(PIECES_SHADER, g_shaders);

	createTextures();
	createSceneGraph();

	return win;

#ifdef ERROR_CALLBACK
	setupErrorCallback();
#endif
}

////////////////////////////////////////////////////////////////////////// RUN

void display(GLFWwindow* win, double elapsed_sec)
{
	drawScene();
}

void run(GLFWwindow* win)
{
	double last_time = glfwGetTime();

	while (!glfwWindowShouldClose(win))
	{
		double time = glfwGetTime();
		double elapsed_time = time - last_time;
		last_time = time;

		// Double Buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//updates camera position
		camera->updateCamera();
		display(win, elapsed_time);
		glfwSwapBuffers(win);
		glfwPollEvents();
#ifndef ERROR_CALLBACK
		ErrorHandler::checkOpenGLError("ERROR: MAIN/RUN");
#endif
	}
	glfwDestroyWindow(win);
	glfwTerminate();
}

////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char* argv[])
{
	int gl_major = 4, gl_minor = 3;
	int is_fullscreen = 0;
	int is_vsync = 1;

	GLFWwindow* win = setup(gl_major, gl_minor,
		640, 640, "Team project", is_fullscreen, is_vsync);
	run(win);


	exit(EXIT_SUCCESS);
}

/////////////////////////////////////////////////////////////////////////// END