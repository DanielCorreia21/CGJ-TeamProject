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

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

/*	MODIFIED BY
*	Group: 11
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*/

bool tcoords = true;
bool normals = true;
bool menuIshowing = true;
bool pause = false;

const char vertexShaderPath[] = "../Resources/vertexShader.glsl";
const char vertexShaderPath_Texture[] = "../Resources/MarbleVS.glsl";
const char fragmentShaderPath[] = "../Resources/fragmentShader.glsl";
const char fragmentShaderPath_Texture[] = "../Resources/MarbleFS.glsl";
const char fragmentShaderPath_Granite[] = "../Resources/GraniteFS.glsl";
const char fragmentShaderPath_Menu[] = "../Resources/Menu_FS.glsl";

const string SLIDING_PUZZLE_SCENE_GRAPH = "SlidingPuzzle";
const string MENU_SCENE_GRAPH = "MainMenu";
const string ESC_MENU_SCENE_GRAPH = "EscMenu";
const string MAIN_SHADER = "main";
const string MENU_SHADER = "menu";
const string CUBE_MESH = "cube";
const string COLOR_UNIFORM = "Color";

Camera camera = Camera(Vector3d(0, 0, -10), Vector3d(0, 0, -1), Vector3d(0, 1, 0));

#define DEGREES_TO_RADIANS 0.01745329251994329547

/*#define ERROR_CALLBACK
#ifdef  ERROR_CALLBACK

////////////////////////////////////////////////// ERROR CALLBACK (OpenGL 4.3+)

static const std::string errorSource(GLenum source)
{
	switch (source) {
	case GL_DEBUG_SOURCE_API:				return "API";
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		return "window system";
	case GL_DEBUG_SOURCE_SHADER_COMPILER:	return "shader compiler";
	case GL_DEBUG_SOURCE_THIRD_PARTY:		return "third party";
	case GL_DEBUG_SOURCE_APPLICATION:		return "application";
	case GL_DEBUG_SOURCE_OTHER:				return "other";
	default:								exit(EXIT_FAILURE);
	}
}

static const std::string errorType(GLenum type)
{
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:				return "error";
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	return "deprecated behavior";
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	return "undefined behavior";
	case GL_DEBUG_TYPE_PORTABILITY:			return "portability issue";
	case GL_DEBUG_TYPE_PERFORMANCE:			return "performance issue";
	case GL_DEBUG_TYPE_MARKER:				return "stream annotation";
	case GL_DEBUG_TYPE_PUSH_GROUP:			return "push group";
	case GL_DEBUG_TYPE_POP_GROUP:			return "pop group";
	case GL_DEBUG_TYPE_OTHER_ARB:			return "other";
	default:								exit(EXIT_FAILURE);
	}
}

static const std::string errorSeverity(GLenum severity)
{
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:			return "high";
	case GL_DEBUG_SEVERITY_MEDIUM:			return "medium";
	case GL_DEBUG_SEVERITY_LOW:				return "low";
	case GL_DEBUG_SEVERITY_NOTIFICATION:	return "notification";
	default:								exit(EXIT_FAILURE);
	}
}

static void error(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
	const GLchar* message, const void* userParam)
{
	std::cerr << "GL ERROR:" << std::endl;
	std::cerr << "  source:     " << errorSource(source) << std::endl;
	std::cerr << "  type:       " << errorType(type) << std::endl;
	std::cerr << "  severity:   " << errorSeverity(severity) << std::endl;
	std::cerr << "  debug call: " << std::endl << message << std::endl;
	std::cerr << "Press <return>.";
	std::cin.ignore();
}

void setupErrorCallback()
{
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(error, 0);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, GL_TRUE);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, 0, GL_FALSE);
	// params: source, type, severity, count, ids, enabled
}

#else // ERROR_CALLBACK

///////////////////////////////////////////////// ERROR HANDLING (All versions)

static const std::string errorString(GLenum error)
{
	switch(error) {
	case GL_NO_ERROR:
		return "No error has been recorded.";
	case GL_INVALID_ENUM:
		return "An unacceptable value is specified for an enumerated argument.";
	case GL_INVALID_VALUE:
		return "A numeric argument is out of range.";
	case GL_INVALID_OPERATION:
		return "The specified operation is not allowed in the current state.";
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		return "The framebuffer object is not complete.";
	case GL_OUT_OF_MEMORY:
		return "There is not enough memory left to execute the command.";
	case GL_STACK_UNDERFLOW:
		return "An attempt has been made to perform an operation that would cause an internal stack to underflow.";
	case GL_STACK_OVERFLOW:
		return "An attempt has been made to perform an operation that would cause an internal stack to overflow.";
	default: exit(EXIT_FAILURE);
	}
}

static bool isOpenGLError()
{
	bool isError = false;
	GLenum errCode;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		std::cerr << "OpenGL ERROR [" << errorString(errCode) << "]." << std::endl;
	}
	return isError;
}

static void checkOpenGLError(std::string error)
{
	if (isOpenGLError()) {
		std::cerr << error << std::endl;
		exit(EXIT_FAILURE);
	}
}

#endif // ERROR_CALLBACK*/

///////////////////////////////////////////////////////////////////// PreDrawFunctions

void setCyanColor() {
	std::map<std::string, ShaderProgram::UniformInfo>::iterator it = ShaderProgramManager::getInstance()->get(MAIN_SHADER)->uniforms.find(COLOR_UNIFORM);
	if (it != ShaderProgramManager::getInstance()->get(MAIN_SHADER)->uniforms.end()) {
		glUniform4f(it->second.index, 0, 1, 1, 0);
	}
}

void setRedColor() {
	//glUniform4f(shaders.getUniformColorId(), 1, 0, 0, 0);
	std::map<std::string, ShaderProgram::UniformInfo>::iterator it = ShaderProgramManager::getInstance()->get(MAIN_SHADER)->uniforms.find(COLOR_UNIFORM);
	if (it != ShaderProgramManager::getInstance()->get(MAIN_SHADER)->uniforms.end()) {
		glUniform4f(it->second.index, 1, 0, 0, 0);
	}
}

void setGreenColor() {
	//glUniform4f(shaders.getUniformColorId(), 0, 1, 0, 0);
	std::map<std::string, ShaderProgram::UniformInfo>::iterator it = ShaderProgramManager::getInstance()->get(MAIN_SHADER)->uniforms.find(COLOR_UNIFORM);
	if (it != ShaderProgramManager::getInstance()->get(MAIN_SHADER)->uniforms.end()) {
		glUniform4f(it->second.index, 0, 1, 0, 0);
	}
}

void setBlueColor() {
	//glUniform4f(shaders.getUniformColorId(), 0, 0, 1, 0);
	std::map<std::string, ShaderProgram::UniformInfo>::iterator it = ShaderProgramManager::getInstance()->get(MAIN_SHADER)->uniforms.find(COLOR_UNIFORM);
	if (it != ShaderProgramManager::getInstance()->get(MAIN_SHADER)->uniforms.end()) {
		glUniform4f(it->second.index, 0, 0, 1, 0);
	}
}

///////////////////////////////////////////////////////////////////// SCENE

SceneNode* backboard;
SceneNode* frame;
SceneNode* pieces;

void createTextures() {

	#pragma region materials

	Texture3D* texture = new Texture3D();
	texture->createPerlinNoise(128, 7, 7, 7, 2, 2, 8);
	TextureManager::getInstance()->add("marble", (Texture*)texture);

	Texture3D* texture_0 = new Texture3D();
	texture_0->createPerlinNoise(128, 7, 7, 7, 2, 2, 8);
	TextureManager::getInstance()->add("granite", (Texture*)texture_0);

	#pragma endregion

	#pragma region numbers

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

	#pragma endregion
	
	#pragma region menu

	Texture2D* fundo_main = new Texture2D();
	fundo_main->load("../menu/fundo_main.png");
	TextureManager::getInstance()->add("fundo_main", (Texture*)fundo_main);

	Texture2D* fundo_esc = new Texture2D();
	fundo_esc->load("../menu/fundo_esc.png");
	TextureManager::getInstance()->add("fundo_esc", (Texture*)fundo_esc);

	Texture2D* title = new Texture2D();
	title->load("../menu/title.png");
	TextureManager::getInstance()->add("title", (Texture*)title);

	Texture2D* new_game = new Texture2D();
	new_game->load("../menu/new_game.png");
	TextureManager::getInstance()->add("new_game", (Texture*)new_game);

	Texture2D* load_game = new Texture2D();
	load_game->load("../menu/load_game.png");
	TextureManager::getInstance()->add("load_game", (Texture*)load_game);

	Texture2D* save = new Texture2D();
	save->load("../menu/save.png");
	TextureManager::getInstance()->add("save", (Texture*)save);

	Texture2D* load = new Texture2D();
	load->load("../menu/load.png");
	TextureManager::getInstance()->add("load", (Texture*)load);

	Texture2D* exit_main = new Texture2D();
	exit_main->load("../menu/exit_main.png");
	TextureManager::getInstance()->add("exit_main", (Texture*)exit_main);

	Texture2D* exit_esc = new Texture2D();
	exit_esc->load("../menu/exit_esc.png");
	TextureManager::getInstance()->add("exit_esc", (Texture*)exit_esc);

	Texture2D* continu = new Texture2D();
	continu->load("../menu/continue.png");
	TextureManager::getInstance()->add("continue", (Texture*)continu);

	Texture2D* snap = new Texture2D();
	snap->load("../menu/snap.png");
	TextureManager::getInstance()->add("snap", (Texture*)snap);

	#pragma endregion

}

void createEnvironmentSceneGraph()
{
	Mesh* cubeMesh = MeshManager::getInstance()->get(CUBE_MESH);

	#pragma region backboard
	backboard = new SceneNode();
	backboard->setParent(SceneGraphManager::getInstance()->get(SLIDING_PUZZLE_SCENE_GRAPH)->getRoot());
	backboard->setPreDrawFun(setBlueColor);
	backboard->setMesh(cubeMesh);
	backboard->setMatrix(
		MatrixFactory::scalingMatrix(Vector3d(11.0f,11.0f,1.0f))
	);

#pragma endregion

	#pragma region frame

	TextureInfo* tinfo_f = new TextureInfo(GL_TEXTURE0, 0, "NoiseTexture", TextureManager::getInstance()->get("granite"));

	frame = new SceneNode();
	frame->setParent(SceneGraphManager::getInstance()->get(SLIDING_PUZZLE_SCENE_GRAPH)->getRoot());
	//frame->setPreDrawFun(setRedColor);
	frame->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, 0.0f, 0.8f))
	);
	frame->setShaderProgram(ShaderProgramManager::getInstance()->get("Granite"));
	frame->addTexture(tinfo_f);

	SceneNode* frameUp = new SceneNode();
	frameUp->setParent(frame);
	//frameUp->setPreDrawFun(setRedColor);
	frameUp->setMesh(cubeMesh);
	//Define shape and set Z
	//UP Frame
	frameUp->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, 2.0f, 0.0f))
		* MatrixFactory::scalingMatrix(Vector3d(11.0f, 1.0f, 3.0f))
	);
	frameUp->setShaderProgram(ShaderProgramManager::getInstance()->get("Granite"));
	frameUp->addTexture(tinfo_f);

	//Define positions and rotations of other frame components
	//Down
	SceneNode* frameDown = new SceneNode();
	frameDown->setParent(frame);
	//frameDown->setPreDrawFun(setRedColor);
	frameDown->setMesh(cubeMesh);
	frameDown->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, -2.0f, 0.0f))
		* MatrixFactory::scalingMatrix(Vector3d(11.0f, 1.0f, 3.0f))
	);
	frameDown->setShaderProgram(ShaderProgramManager::getInstance()->get("Granite"));
	frameDown->addTexture(tinfo_f);

	//Right
	SceneNode* frameRight = new SceneNode();
	frameRight->setParent(frame);
	//frameRight->setPreDrawFun(setRedColor);
	frameRight->setMesh(cubeMesh);
	frameRight->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(2.0f, 0.0f, 0.0f))
		* MatrixFactory::rotateZMatrix(90)
		* MatrixFactory::scalingMatrix(Vector3d(11.0f, 1.0f, 3.0f))
	);
	frameRight->setShaderProgram(ShaderProgramManager::getInstance()->get("Granite"));
	frameRight->addTexture(tinfo_f);

	//Left
	SceneNode* frameLeft = new SceneNode();
	frameLeft->setParent(frame);
	//frameLeft->setPreDrawFun(setRedColor);
	frameLeft->setMesh(cubeMesh);
	frameLeft->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(-2.0f, 0.0f, 0.0f))
		* MatrixFactory::rotateZMatrix(90)
		* MatrixFactory::scalingMatrix(Vector3d(11.0f, 1.0f, 3.0f))
	);
	frameLeft->setShaderProgram(ShaderProgramManager::getInstance()->get("Granite"));
	frameLeft->addTexture(tinfo_f);

#pragma endregion

	#pragma region pieces

	pieces = new SceneNode();
	pieces->setParent(SceneGraphManager::getInstance()->get(SLIDING_PUZZLE_SCENE_GRAPH)->getRoot());
	//pieces->setPreDrawFun(setGreenColor);
	pieces->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, 0.0f, 0.8f))
		* MatrixFactory::scalingMatrix(3.0f)
	);


	TextureInfo* tinfo_0 = new TextureInfo(GL_TEXTURE0, 0, "NoiseTexture", TextureManager::getInstance()->get("marble"));

	SceneNode* piece1 = new SceneNode();
	piece1->setParent(pieces);
	piece1->setMesh(cubeMesh);
	piece1->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(-0.4f,0.4f,0.0f))
	);

	TextureInfo* tinfo_1 = new TextureInfo(GL_TEXTURE1, 1, "Texture_1", TextureManager::getInstance()->get("number_1"));
	piece1->setShaderProgram(ShaderProgramManager::getInstance()->get("Marble"));
	piece1->addTexture(tinfo_0);
	piece1->addTexture(tinfo_1);

	SceneNode* piece2 = new SceneNode();
	piece2->setParent(pieces);
	piece2->setMesh(cubeMesh);
	piece2->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, 0.4f, 0.0f))
	);
	TextureInfo* tinfo_2 = new TextureInfo(GL_TEXTURE1, 1, "Texture_1", TextureManager::getInstance()->get("number_2"));
	piece2->setShaderProgram(ShaderProgramManager::getInstance()->get("Marble"));
	piece2->addTexture(tinfo_0);
	piece2->addTexture(tinfo_2);

	SceneNode* piece3 = new SceneNode();
	piece3->setParent(pieces);
	piece3->setMesh(cubeMesh);
	piece3->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.4f, 0.4f, 0.0f))
	);
	TextureInfo* tinfo_3 = new TextureInfo(GL_TEXTURE1, 1, "Texture_1", TextureManager::getInstance()->get("number_3"));
	piece3->setShaderProgram(ShaderProgramManager::getInstance()->get("Marble"));
	piece3->addTexture(tinfo_0);
	piece3->addTexture(tinfo_3);

	SceneNode* piece4 = new SceneNode();
	piece4->setParent(pieces);
	piece4->setMesh(cubeMesh);
	piece4->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(-0.4f, 0.0f, 0.0f))
	);
	TextureInfo* tinfo_4 = new TextureInfo(GL_TEXTURE1, 1, "Texture_1", TextureManager::getInstance()->get("number_4"));
	piece4->setShaderProgram(ShaderProgramManager::getInstance()->get("Marble"));
	piece4->addTexture(tinfo_0);
	piece4->addTexture(tinfo_4);

	SceneNode* piece5 = new SceneNode();
	piece5->setParent(pieces);
	piece5->setMesh(cubeMesh);
	piece5->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, 0.0f, 0.0f))
	);
	TextureInfo* tinfo_5 = new TextureInfo(GL_TEXTURE1, 1, "Texture_1", TextureManager::getInstance()->get("number_5"));
	piece5->setShaderProgram(ShaderProgramManager::getInstance()->get("Marble"));
	piece5->addTexture(tinfo_0);
	piece5->addTexture(tinfo_5);

	SceneNode* piece6 = new SceneNode();
	piece6->setParent(pieces);
	piece6->setMesh(cubeMesh);
	piece6->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.4f, 0.0f, 0.0f))
	);
	TextureInfo* tinfo_6 = new TextureInfo(GL_TEXTURE1, 1, "Texture_1", TextureManager::getInstance()->get("number_6"));
	piece6->setShaderProgram(ShaderProgramManager::getInstance()->get("Marble"));
	piece6->addTexture(tinfo_0);
	piece6->addTexture(tinfo_6);

	SceneNode* piece7 = new SceneNode();
	piece7->setParent(pieces);
	piece7->setMesh(cubeMesh);
	piece7->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(-0.4f, -0.4f, 0.0f))
	);
	TextureInfo* tinfo_7 = new TextureInfo(GL_TEXTURE1, 1, "Texture_1", TextureManager::getInstance()->get("number_7"));
	piece7->setShaderProgram(ShaderProgramManager::getInstance()->get("Marble"));
	piece7->addTexture(tinfo_0);
	piece7->addTexture(tinfo_7);

	SceneNode* piece8 = new SceneNode();
	piece8->setParent(pieces);
	piece8->setMesh(cubeMesh);
	piece8->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, -0.4f, 0.0f))
	);
	TextureInfo* tinfo_8 = new TextureInfo(GL_TEXTURE1, 1, "Texture_1", TextureManager::getInstance()->get("number_8"));
	piece8->setShaderProgram(ShaderProgramManager::getInstance()->get("Marble"));
	piece8->addTexture(tinfo_0);
	piece8->addTexture(tinfo_8);

#pragma endregion
}

void createSceneGraph()
{
	Mesh* mesh = new Mesh();
	MeshManager::getInstance()->add(CUBE_MESH, mesh);

	string s = string("../objs/cube5.obj");

	//mesh->createMesh(s);
	//mesh->createMeshBufferObjects();
	mesh->init(s);


	SceneGraph* slidingPuzzleScenegraph = new SceneGraph();
	slidingPuzzleScenegraph->setCamera(&camera);
	SceneGraphManager::getInstance()->add(SLIDING_PUZZLE_SCENE_GRAPH, slidingPuzzleScenegraph);

	SceneNode* n = slidingPuzzleScenegraph->getRoot();
	n->setShaderProgram(ShaderProgramManager::getInstance()->get(MAIN_SHADER));


	createEnvironmentSceneGraph();

	slidingPuzzleScenegraph->init(*(ShaderProgramManager::getInstance()->get(MAIN_SHADER)));
}

void createMainMenuEnvironment()
{
	Mesh* cubeMesh = MeshManager::getInstance()->get(CUBE_MESH);

#pragma region fundo

	TextureInfo* fundo = new TextureInfo(GL_TEXTURE0, 0, "Texture_1", TextureManager::getInstance()->get("fundo_main"));

	SceneNode* plane = new SceneNode();
	plane->setParent(SceneGraphManager::getInstance()->get(MENU_SCENE_GRAPH)->getRoot());
	plane->setPreDrawFun(setBlueColor);
	plane->setMesh(cubeMesh);
	plane->setMatrix(
		MatrixFactory::scalingMatrix(Vector3d(15.0f, 15.0f, 0.1f))
	);
	plane->addTexture(fundo);

#pragma endregion

#pragma region top text

	TextureInfo* game_name = new TextureInfo(GL_TEXTURE0, 0, "Texture_1", TextureManager::getInstance()->get("title"));

	SceneNode* text = new SceneNode();
	text->setParent(SceneGraphManager::getInstance()->get(MENU_SCENE_GRAPH)->getRoot());
	text->setMesh(cubeMesh);
	text->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, 2.08f, 0.0f))
		* MatrixFactory::scalingMatrix(Vector3d(9.5f, 1.6f, 0.2f))
	);
	text->addTexture(game_name);

#pragma endregion

#pragma region buttons

	TextureInfo* start_button = new TextureInfo(GL_TEXTURE0, 0, "Texture_1", TextureManager::getInstance()->get("new_game"));

	SceneNode* startButton = new SceneNode();
	startButton->setParent(SceneGraphManager::getInstance()->get(MENU_SCENE_GRAPH)->getRoot());
	startButton->setMesh(cubeMesh);
	startButton->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, 0.4f, 0.0f))
		* MatrixFactory::scalingMatrix(Vector3d(6.0f, 1.8f, 0.5f))
	);
	startButton->addTexture(start_button);

	TextureInfo* load_button = new TextureInfo(GL_TEXTURE0, 0, "Texture_1", TextureManager::getInstance()->get("load_game"));

	SceneNode* loadButton = new SceneNode();
	loadButton->setParent(SceneGraphManager::getInstance()->get(MENU_SCENE_GRAPH)->getRoot());
	loadButton->setMesh(cubeMesh);
	loadButton->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, -0.5f, 0.0f))
		* MatrixFactory::scalingMatrix(Vector3d(6.0f, 1.8f, 0.5f))
	);
	loadButton->addTexture(load_button);

	TextureInfo* exit_button = new TextureInfo(GL_TEXTURE0, 0, "Texture_1", TextureManager::getInstance()->get("exit_main"));

	SceneNode* exitButton = new SceneNode();
	exitButton->setParent(SceneGraphManager::getInstance()->get(MENU_SCENE_GRAPH)->getRoot());
	exitButton->setMesh(cubeMesh);
	exitButton->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, -1.5f, 0.0f))
		* MatrixFactory::scalingMatrix(Vector3d(6.0f, 1.8f, 0.5f))
	);
	exitButton->addTexture(exit_button);

#pragma endregion
}

void createMainMenuGraph()
{
	Mesh* mesh = new Mesh();
	MeshManager::getInstance()->add(CUBE_MESH, mesh);

	string s = string("../objs/cube5.obj");
	mesh->init(s);

	SceneGraph* menuScenegraph = new SceneGraph();
	menuScenegraph->setCamera(&camera);
	SceneGraphManager::getInstance()->add(MENU_SCENE_GRAPH, menuScenegraph);

	SceneNode* n = menuScenegraph->getRoot();
	n->setShaderProgram(ShaderProgramManager::getInstance()->get(MENU_SHADER));

	createMainMenuEnvironment();

	menuScenegraph->init(*(ShaderProgramManager::getInstance()->get(MENU_SHADER)));
}

void createEscMenuEnvironment()
{
	Mesh* cubeMesh = MeshManager::getInstance()->get(CUBE_MESH);

#pragma region fundo

	TextureInfo* fundo = new TextureInfo(GL_TEXTURE0, 0, "Texture_1", TextureManager::getInstance()->get("fundo_esc"));

	SceneNode* plane = new SceneNode();
	plane->setParent(SceneGraphManager::getInstance()->get(ESC_MENU_SCENE_GRAPH)->getRoot());
	plane->setPreDrawFun(setBlueColor);
	plane->setMesh(cubeMesh);
	plane->setMatrix(
		MatrixFactory::scalingMatrix(Vector3d(15.0f, 15.0f, 0.1f))
	);
	plane->addTexture(fundo);

#pragma endregion

#pragma region buttons

	TextureInfo* continue_button = new TextureInfo(GL_TEXTURE0, 0, "Texture_1", TextureManager::getInstance()->get("continue"));

	SceneNode* continueButton = new SceneNode();
	continueButton->setParent(SceneGraphManager::getInstance()->get(ESC_MENU_SCENE_GRAPH)->getRoot());
	continueButton->setMesh(cubeMesh);
	continueButton->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, 1.6f, 0.0f))
		* MatrixFactory::scalingMatrix(Vector3d(6.0f, 1.8f, 0.5f))
	);
	continueButton->addTexture(continue_button);

	TextureInfo* save_button = new TextureInfo(GL_TEXTURE0, 0, "Texture_1", TextureManager::getInstance()->get("save"));

	SceneNode* saveButton = new SceneNode();
	saveButton->setParent(SceneGraphManager::getInstance()->get(ESC_MENU_SCENE_GRAPH)->getRoot());
	saveButton->setMesh(cubeMesh);
	saveButton->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, 0.7f, 0.0f))
		* MatrixFactory::scalingMatrix(Vector3d(6.0f, 1.8f, 0.5f))
	);
	saveButton->addTexture(save_button);

	TextureInfo* load_button = new TextureInfo(GL_TEXTURE0, 0, "Texture_1", TextureManager::getInstance()->get("load"));

	SceneNode* loadButton = new SceneNode();
	loadButton->setParent(SceneGraphManager::getInstance()->get(ESC_MENU_SCENE_GRAPH)->getRoot());
	loadButton->setMesh(cubeMesh);
	loadButton->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, -0.2f, 0.0f))
		* MatrixFactory::scalingMatrix(Vector3d(6.0f, 1.8f, 0.5f))
	);
	loadButton->addTexture(load_button);

	TextureInfo* snap_button = new TextureInfo(GL_TEXTURE0, 0, "Texture_1", TextureManager::getInstance()->get("snap"));

	SceneNode* snapButton = new SceneNode();
	snapButton->setParent(SceneGraphManager::getInstance()->get(ESC_MENU_SCENE_GRAPH)->getRoot());
	snapButton->setMesh(cubeMesh);
	snapButton->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, -1.1f, 0.0f))
		* MatrixFactory::scalingMatrix(Vector3d(6.0f, 1.8f, 0.5f))
	);
	snapButton->addTexture(snap_button);

	TextureInfo* exit_button = new TextureInfo(GL_TEXTURE0, 0, "Texture_1", TextureManager::getInstance()->get("exit_esc"));

	SceneNode* exitButton = new SceneNode();
	exitButton->setParent(SceneGraphManager::getInstance()->get(ESC_MENU_SCENE_GRAPH)->getRoot());
	exitButton->setMesh(cubeMesh);
	exitButton->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, -2.0f, 0.0f))
		* MatrixFactory::scalingMatrix(Vector3d(6.0f, 1.8f, 0.5f))
	);
	exitButton->addTexture(exit_button);

#pragma endregion
}

void createEscMenuGraph()
{
	Mesh* mesh = new Mesh();
	MeshManager::getInstance()->add(CUBE_MESH, mesh);

	string s = string("../objs/cube5.obj");
	mesh->init(s);

	SceneGraph* escMenuScenegraph = new SceneGraph();
	escMenuScenegraph->setCamera(&camera);
	SceneGraphManager::getInstance()->add(ESC_MENU_SCENE_GRAPH, escMenuScenegraph);

	SceneNode* n = escMenuScenegraph->getRoot();
	n->setShaderProgram(ShaderProgramManager::getInstance()->get(MENU_SHADER));

	createEscMenuEnvironment();

	escMenuScenegraph->init(*(ShaderProgramManager::getInstance()->get(MENU_SHADER)));
}

void drawScene()
{
	if (menuIshowing) {
		SceneGraphManager::getInstance()->get(MENU_SCENE_GRAPH)->draw();
	}

	else if (pause) {
		SceneGraphManager::getInstance()->get(ESC_MENU_SCENE_GRAPH)->draw();
	}

	else {
		SceneGraphManager::getInstance()->get(SLIDING_PUZZLE_SCENE_GRAPH)->draw();
	}

#ifndef ERROR_CALLBACK
	ErrorHandler::checkOpenGLError("ERROR: Could not draw scene.");
#endif
}

///////////////////////////////////////////////////////////////////// CALLBACKS

void window_close_callback(GLFWwindow* win)
{
	ShaderProgramManager::getInstance()->get(MAIN_SHADER)->destroy();
	SceneGraphManager::getInstance()->get(SLIDING_PUZZLE_SCENE_GRAPH)->destroy();
	//TODO maybe "delete mesh"
	//mesh->destroyMeshBufferObjects();
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
		camera.updateCameraPos(key, action);
	}

	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		camera.changeProjectionType();
	}

	if (key == GLFW_KEY_G && action == GLFW_PRESS)
	{
		camera.changeRotationType();
	}

}

int mouse_pressed = 0;

void mouse_callback(GLFWwindow* win, double xpos, double ypos)
{
	if (menuIshowing == false && pause == false) {
		camera.look(xpos, ypos, mouse_pressed);
	}
	
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

	ShaderProgram* shaders = new ShaderProgram();
	shaders->addUniform(COLOR_UNIFORM);
	shaders->init(vertexShaderPath, fragmentShaderPath, tcoords, normals);
	ShaderProgramManager::getInstance()->add(MAIN_SHADER, shaders);

	ShaderProgram* menu_shaders = new ShaderProgram();
	menu_shaders->addUniform(COLOR_UNIFORM);
	menu_shaders->addUniform("Texture_1");
	menu_shaders->init(vertexShaderPath_Texture, fragmentShaderPath_Menu, tcoords, normals);
	ShaderProgramManager::getInstance()->add(MENU_SHADER, menu_shaders);

	ShaderProgram* m_shaders = new ShaderProgram();
	m_shaders->addUniform(COLOR_UNIFORM);
	m_shaders->addUniform("Texture_1");
	m_shaders->addUniform("NoiseTexture");

	m_shaders->init(vertexShaderPath_Texture, fragmentShaderPath_Texture, tcoords, normals);
	ShaderProgramManager::getInstance()->add("Marble", m_shaders);

	ShaderProgram* g_shaders = new ShaderProgram();
	g_shaders->addUniform(COLOR_UNIFORM);
	g_shaders->addUniform("NoiseTexture");

	g_shaders->init(vertexShaderPath_Texture, fragmentShaderPath_Granite, tcoords, normals);
	ShaderProgramManager::getInstance()->add("Granite", g_shaders);

	createTextures();
	createEscMenuGraph();
	createMainMenuGraph();
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
		camera.updateCamera();
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