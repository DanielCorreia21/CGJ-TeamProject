///////////////////////////////////////////////////////////////////////////////
// AUTHORS
// Group: 4
// Bernardo Pinto - 98734
// Daniel Correia - 98745
// Antoine Pontallier - 98316
// André Santos - 91000
///////////////////////////////////////////////////////////////////////////////

#include <regex>
#include <iostream>
#include <time.h>
#include <direct.h>
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
#include "SlidePuzzleSceneNode.h"
#include "GameSlidingPuzzle.h"
#include "Globals.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <FreeImage.h>
#include "SceneFileHandler.h"
#include "PreDrawFunction.h"
#include "SlidePuzzleGameFileHandler.h"

using namespace std;

#pragma region AppInstances
Camera* camera;
SceneFileHandler* sceneFileHandler;
SlidePuzzleGameFileHandler* slidePuzzleGameFileHandler;
GameSlidingPuzzle* game;
#pragma endregion

#pragma region HelperVariables
//Menus
bool menuIshowing = true;
bool pause = false;
bool buttonPressed = false;
bool exitomenu = false;
bool exitgame = false;

//Snapshot
int width, height;
#pragma endregion

/////////////////////////////////////////////////////////////////////////////// PreDrawFunctions
#pragma region PREDRAWFUNCTIONS
void setRedColor() {
	ShaderProgram::UniformInfo* uniInfo = ShaderProgramManager::getInstance()->get(COLOR_SHADER)->getUniform(COLOR_UNIFORM);
	glUniform4f(uniInfo->index, 1, 0, 0, 0);
}

void setBlueColor() {
	ShaderProgram::UniformInfo* uniInfo = ShaderProgramManager::getInstance()->get(COLOR_SHADER)->getUniform(COLOR_UNIFORM);
	glUniform4f(uniInfo->index, 0, 0, 1, 0);
}

typedef void (*PreDrawFunc)();
pair<string,PreDrawFunc> preDrawFunc[2] = { 
	make_pair("RedColor", &setRedColor), 
	make_pair("BlueColor", &setBlueColor),
};

#pragma endregion
/////////////////////////////////////////////////////////////////////////////// Load by default
#pragma region LOADDEFAULT
void loadDefaultTextures() {
	//These are textures that I cannot recreate unless I start storing a lot of things, which doens't make sense.
	//Therefore, these textures are loaded on startup
	Texture3D* texture_0 = new Texture3D();
	texture_0->createPerlinNoise(128, 7, 7, 7, 2, 2, 8);
	TextureManager::getInstance()->add("marble", (Texture*)texture_0);

	Texture3D* texture_1 = new Texture3D();
	texture_1->createPerlinNoise(128, 7, 7, 7, 2, 2, 8);
	TextureManager::getInstance()->add("granite", (Texture*)texture_1);

	Texture3D* texture_wood = new Texture3D();
	//texture_wood->createPerlinNoise(128, 5, 5, 5, 2, 2, 8);
	TextureManager::getInstance()->add("wood", (Texture*)texture_wood);
}

void loadDefaultPreDrawFunctions() {
	int len = sizeof(preDrawFunc) / sizeof(preDrawFunc[0]);
	for (int i = 0; i < len; i++) {
		PreDrawFunction* pdf = new PreDrawFunction(preDrawFunc[i].second);
		PreDrawFunctionManager::getInstance()->add(preDrawFunc[i].first, pdf);
	}
}
#pragma endregion
/////////////////////////////////////////////////////////////////////////////// Create new Scene
#pragma region SCENE

void createShaders() {

	//Basic Color Shader
	ShaderProgram* shaders = new ShaderProgram();
	shaders->addUniform(COLOR_UNIFORM);
	shaders->init(colorVertexShaderPath, colorFragmentShaderPath);
	ShaderProgramManager::getInstance()->add(COLOR_SHADER, shaders);

	//Menu Shader
	ShaderProgram* menu_shaders = new ShaderProgram();
	menu_shaders->addUniform(COLOR_UNIFORM);
	menu_shaders->addUniform(TEXTURE_UNIFORM_COLOR);
	menu_shaders->init(textureVertexShaderPath, menuFragmentShaderPath);
	ShaderProgramManager::getInstance()->add(MENU_SHADER, menu_shaders);

	//Marble Shader
	ShaderProgram* m_shaders = new ShaderProgram();
	m_shaders->addUniform(COLOR_UNIFORM);
	m_shaders->addUniform(TEXTURE_UNIFORM_COLOR);
	m_shaders->addUniform(TEXTURE_UNIFORM_NOISE);

	m_shaders->init(textureVertexShaderPath, marbleFragmentShaderPath);
	ShaderProgramManager::getInstance()->add(PIECES_SHADER, m_shaders);

	//Granite Shader
	ShaderProgram* g_shaders = new ShaderProgram();
	g_shaders->addUniform(COLOR_UNIFORM);
	g_shaders->addUniform(TEXTURE_UNIFORM_NOISE);

	g_shaders->init(textureVertexShaderPath, graniteFragmentShaderPath);
	ShaderProgramManager::getInstance()->add(FRAME_SHADER, g_shaders);

	//Wood Shader
	ShaderProgram* w_shaders = new ShaderProgram();
	w_shaders->addUniform(COLOR_UNIFORM);
	w_shaders->addUniform(TEXTURE_UNIFORM_COLOR);
	w_shaders->addUniform(TEXTURE_UNIFORM_NOISE);

	w_shaders->init(textureVertexShaderPath, woodFragmentShaderPath);
	ShaderProgramManager::getInstance()->add(BACKBOARD_SHADER, w_shaders);
}

void createTextures() {

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
	Mesh* discMesh = MeshManager::getInstance()->get(DISC_MESH);

#pragma region backboard
	TextureInfo* tinfo_wood = new TextureInfo(GL_TEXTURE0, 0, TEXTURE_UNIFORM_NOISE, TextureManager::getInstance()->get("wood"));
	ShaderProgram* backboardShader = ShaderProgramManager::getInstance()->get(BACKBOARD_SHADER);
	SceneNode* backboard = new SceneNode();
	backboard->setParent(SceneGraphManager::getInstance()->get(SLIDING_PUZZLE_SCENE_GRAPH)->getRoot());
	backboard->setPreDrawFun(PreDrawFunctionManager::getInstance()->get("BlueColor"));
	backboard->setMesh(cubeMesh);
	backboard->setMatrix(
		MatrixFactory::scalingMatrix(Vector3d(11.0f, 11.0f, 1.0f))
	);
	backboard->setShaderProgram(backboardShader);

#pragma endregion

	#pragma region frame

	TextureInfo* tinfo_f = new TextureInfo(GL_TEXTURE0, 0, TEXTURE_UNIFORM_NOISE, TextureManager::getInstance()->get("granite"));
	ShaderProgram* frameShader = ShaderProgramManager::getInstance()->get(FRAME_SHADER);

	SceneNode* frame = new SceneNode();
	frame->setParent(SceneGraphManager::getInstance()->get(SLIDING_PUZZLE_SCENE_GRAPH)->getRoot());
	//frame->setPreDrawFun(PreDrawFunctionManager::getInstance()->get("RedColor"));
	frame->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, 0.0f, 0.8f))
	);

	SceneNode* frameUp = new SceneNode();
	frameUp->setParent(frame);
	//frameUp->setPreDrawFun(PreDrawFunctionManager::getInstance()->get("RedColor"));
	frameUp->setMesh(cubeMesh);

	//UP Frame
	frameUp->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, 2.0f, 0.0f))
		* MatrixFactory::scalingMatrix(Vector3d(11.0f, 1.0f, 3.0f))
	);
	frameUp->setShaderProgram(frameShader);
	frameUp->addTexture(tinfo_f);

	//Define positions and rotations of other frame components
	//Down
	SceneNode* frameDown = new SceneNode();
	frameDown->setParent(frame);
	//frameDown->setPreDrawFun(PreDrawFunctionManager::getInstance()->get("RedColor"));
	frameDown->setMesh(cubeMesh);
	frameDown->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, -2.0f, 0.0f))
		* MatrixFactory::scalingMatrix(Vector3d(11.0f, 1.0f, 3.0f))
	);
	frameDown->setShaderProgram(frameShader);
	frameDown->addTexture(tinfo_f);

	//Right
	SceneNode* frameRight = new SceneNode();
	frameRight->setParent(frame);
	//frameRight->setPreDrawFun(PreDrawFunctionManager::getInstance()->get("RedColor"));
	frameRight->setMesh(cubeMesh);
	frameRight->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(2.0f, 0.0f, 0.0f))
		* MatrixFactory::rotateZMatrix(90)
		* MatrixFactory::scalingMatrix(Vector3d(11.0f, 1.0f, 3.0f))
	);
	frameRight->setShaderProgram(frameShader);
	frameRight->addTexture(tinfo_f);

	//Left
	SceneNode* frameLeft = new SceneNode();
	frameLeft->setParent(frame);
	//frameLeft->setPreDrawFun(PreDrawFunctionManager::getInstance()->get("RedColor"));
	frameLeft->setMesh(cubeMesh);
	frameLeft->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(-2.0f, 0.0f, 0.0f))
		* MatrixFactory::rotateZMatrix(90)
		* MatrixFactory::scalingMatrix(Vector3d(11.0f, 1.0f, 3.0f))
	);
	frameLeft->setShaderProgram(frameShader);
	frameLeft->addTexture(tinfo_f);

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
	SceneNode* piece1 = new SlidePuzzleSceneNode(1);
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
	SceneNode* piece2 = new SlidePuzzleSceneNode(2);
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
	SceneNode* piece3 = new SlidePuzzleSceneNode(3);
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
	SceneNode* piece4 = new SlidePuzzleSceneNode(4);
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
	SceneNode* piece5 = new SlidePuzzleSceneNode(5);
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
	SceneNode* piece6 = new SlidePuzzleSceneNode(6);
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
	SceneNode* piece7 = new SlidePuzzleSceneNode(7);
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
	SceneNode* piece8 = new SlidePuzzleSceneNode(8);
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

	Mesh* meshD = new Mesh();
	string sD = string(discMeshPath);
	meshD->init(sD);

	MeshManager::getInstance()->add(DISC_MESH, meshD);

	SceneGraph* slidingPuzzleScenegraph = new SceneGraph();
	slidingPuzzleScenegraph->setCamera(camera);
	SceneGraphManager::getInstance()->add(SLIDING_PUZZLE_SCENE_GRAPH, slidingPuzzleScenegraph);

	SceneNode* n = slidingPuzzleScenegraph->getRoot();
	n->setShaderProgram(ShaderProgramManager::getInstance()->get(COLOR_SHADER));

	createEnvironmentSceneGraph();

	slidingPuzzleScenegraph->init();
}

void createMainMenuEnvironment()
{
	Mesh* cubeMesh = MeshManager::getInstance()->get(CUBE_MESH);

#pragma region fundo

	TextureInfo* fundo = new TextureInfo(GL_TEXTURE0, 0, TEXTURE_UNIFORM_COLOR, TextureManager::getInstance()->get("fundo_main"));

	SceneNode* plane = new SceneNode();
	plane->setParent(SceneGraphManager::getInstance()->get(MENU_SCENE_GRAPH)->getRoot());
	plane->setPreDrawFun(PreDrawFunctionManager::getInstance()->get("BlueColor"));
	plane->setMesh(cubeMesh);
	plane->setMatrix(
		MatrixFactory::scalingMatrix(Vector3d(15.0f, 15.0f, 0.1f))
	);
	plane->addTexture(fundo);

#pragma endregion

#pragma region top text

	TextureInfo* game_name = new TextureInfo(GL_TEXTURE0, 0, TEXTURE_UNIFORM_COLOR, TextureManager::getInstance()->get("title"));

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

	TextureInfo* start_button = new TextureInfo(GL_TEXTURE0, 0, TEXTURE_UNIFORM_COLOR, TextureManager::getInstance()->get("new_game"));

	SceneNode* startButton = new SceneNode();
	startButton->setParent(SceneGraphManager::getInstance()->get(MENU_SCENE_GRAPH)->getRoot());
	startButton->setMesh(cubeMesh);
	startButton->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, 0.4f, 0.0f))
		* MatrixFactory::scalingMatrix(Vector3d(6.0f, 1.8f, 0.5f))
	);
	startButton->addTexture(start_button);

	TextureInfo* load_button = new TextureInfo(GL_TEXTURE0, 0, TEXTURE_UNIFORM_COLOR, TextureManager::getInstance()->get("load_game"));

	SceneNode* loadButton = new SceneNode();
	loadButton->setParent(SceneGraphManager::getInstance()->get(MENU_SCENE_GRAPH)->getRoot());
	loadButton->setMesh(cubeMesh);
	loadButton->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, -0.5f, 0.0f))
		* MatrixFactory::scalingMatrix(Vector3d(6.0f, 1.8f, 0.5f))
	);
	loadButton->addTexture(load_button);

	TextureInfo* exit_button = new TextureInfo(GL_TEXTURE0, 0, TEXTURE_UNIFORM_COLOR, TextureManager::getInstance()->get("exit_main"));

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
	string s = string(cubeMeshPath);
	mesh->init(s);

	MeshManager::getInstance()->add(CUBE_MESH, mesh);

	SceneGraph* menuScenegraph = new SceneGraph();
	menuScenegraph->setCamera(camera);
	SceneGraphManager::getInstance()->add(MENU_SCENE_GRAPH, menuScenegraph);

	SceneNode* n = menuScenegraph->getRoot();
	n->setShaderProgram(ShaderProgramManager::getInstance()->get(MENU_SHADER));

	createMainMenuEnvironment();

	menuScenegraph->init();
}

void createEscMenuEnvironment()
{
	Mesh* cubeMesh = MeshManager::getInstance()->get(CUBE_MESH);

#pragma region fundo

	TextureInfo* fundo = new TextureInfo(GL_TEXTURE0, 0, TEXTURE_UNIFORM_COLOR, TextureManager::getInstance()->get("fundo_esc"));

	SceneNode* plane = new SceneNode();
	plane->setParent(SceneGraphManager::getInstance()->get(ESC_MENU_SCENE_GRAPH)->getRoot());
	plane->setPreDrawFun(PreDrawFunctionManager::getInstance()->get("BlueColor"));
	plane->setMesh(cubeMesh);
	plane->setMatrix(
		MatrixFactory::scalingMatrix(Vector3d(15.0f, 15.0f, 0.1f))
	);
	plane->addTexture(fundo);

#pragma endregion

#pragma region buttons

	TextureInfo* continue_button = new TextureInfo(GL_TEXTURE0, 0, TEXTURE_UNIFORM_COLOR, TextureManager::getInstance()->get("continue"));

	SceneNode* continueButton = new SceneNode();
	continueButton->setParent(SceneGraphManager::getInstance()->get(ESC_MENU_SCENE_GRAPH)->getRoot());
	continueButton->setMesh(cubeMesh);
	continueButton->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, 1.6f, 0.0f))
		* MatrixFactory::scalingMatrix(Vector3d(6.0f, 1.8f, 0.5f))
	);
	continueButton->addTexture(continue_button);

	TextureInfo* save_button = new TextureInfo(GL_TEXTURE0, 0, TEXTURE_UNIFORM_COLOR, TextureManager::getInstance()->get("save"));

	SceneNode* saveButton = new SceneNode();
	saveButton->setParent(SceneGraphManager::getInstance()->get(ESC_MENU_SCENE_GRAPH)->getRoot());
	saveButton->setMesh(cubeMesh);
	saveButton->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, 0.7f, 0.0f))
		* MatrixFactory::scalingMatrix(Vector3d(6.0f, 1.8f, 0.5f))
	);
	saveButton->addTexture(save_button);

	TextureInfo* load_button = new TextureInfo(GL_TEXTURE0, 0, TEXTURE_UNIFORM_COLOR, TextureManager::getInstance()->get("load"));

	SceneNode* loadButton = new SceneNode();
	loadButton->setParent(SceneGraphManager::getInstance()->get(ESC_MENU_SCENE_GRAPH)->getRoot());
	loadButton->setMesh(cubeMesh);
	loadButton->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, -0.2f, 0.0f))
		* MatrixFactory::scalingMatrix(Vector3d(6.0f, 1.8f, 0.5f))
	);
	loadButton->addTexture(load_button);

	TextureInfo* snap_button = new TextureInfo(GL_TEXTURE0, 0, TEXTURE_UNIFORM_COLOR, TextureManager::getInstance()->get("snap"));

	SceneNode* snapButton = new SceneNode();
	snapButton->setParent(SceneGraphManager::getInstance()->get(ESC_MENU_SCENE_GRAPH)->getRoot());
	snapButton->setMesh(cubeMesh);
	snapButton->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, -1.1f, 0.0f))
		* MatrixFactory::scalingMatrix(Vector3d(6.0f, 1.8f, 0.5f))
	);
	snapButton->addTexture(snap_button);

	TextureInfo* exit_button = new TextureInfo(GL_TEXTURE0, 0, TEXTURE_UNIFORM_COLOR, TextureManager::getInstance()->get("exit_esc"));

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
	string s = string(cubeMeshPath);
	mesh->init(s);

	MeshManager::getInstance()->add(CUBE_MESH, mesh);

	SceneGraph* escMenuScenegraph = new SceneGraph();
	escMenuScenegraph->setCamera(camera);
	SceneGraphManager::getInstance()->add(ESC_MENU_SCENE_GRAPH, escMenuScenegraph);

	SceneNode* n = escMenuScenegraph->getRoot();
	n->setShaderProgram(ShaderProgramManager::getInstance()->get(MENU_SHADER));

	createEscMenuEnvironment();

	escMenuScenegraph->init();
}


void drawScene()
{
	if (menuIshowing) {
		camera->menu = 1;
		SceneGraphManager::getInstance()->get(MENU_SCENE_GRAPH)->draw();
	}

	else if (pause) {
		camera->menu = 1;
		SceneGraphManager::getInstance()->get(ESC_MENU_SCENE_GRAPH)->draw();
	}

	else {
		camera->menu = 0;
		SceneGraphManager::getInstance()->get(SLIDING_PUZZLE_SCENE_GRAPH)->draw();
	}

#ifndef ERROR_CALLBACK
	ErrorHandler::checkOpenGLError("ERROR: Could not draw scene.");
#endif
}

void takeSnapshot() {

	const int numPixels = 3 * width * height;
	GLubyte* pixels = (unsigned char*)malloc(numPixels * sizeof(GLubyte));

	if (!pixels) {
		std::cerr << "Error allocating memory for snapshot\n";
		return;
	}

	GLenum format = GL_RGB;
	if (FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_BGR) {
		format = GL_BGR;
	}

	glReadPixels(0, 0, width, height, format, GL_UNSIGNED_BYTE, pixels);

	
	//Use time for snapshot name
	time_t now = time(NULL);
	char str[26] = {};
	ctime_s(str, 26, &now);
	

	//creates directory if it doesnt exist
	if (_mkdir(snapshots_directory.c_str()) != ENOENT) {
		//if directory was created or already exists:

		string name = regex_replace(str, regex{ " " }, "_");
		string accepted_name = regex_replace(name, regex{ ":" }, "-");

		string file_type = ".png";
		string fullname = snapshots_directory + accepted_name.substr(0, accepted_name.size() - 1) + file_type;

		int stride = width * 3;

		FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, width, height, stride, 24, 0, 0, 0, false);
		FreeImage_Save(FIF_PNG, image, fullname.c_str(), 0);

		// Free resources
		FreeImage_Unload(image);
		free(pixels);
	}
}


void createMenus() {
	camera = new Camera(Vector3d(0, 0, -10), Vector3d(0, 0, -1), Vector3d(0, 1, 0));

	createShaders();
	createTextures();
	createEscMenuGraph();
	createMainMenuGraph();
}

void createNewSlidePuzzleGame() {
	//camera = new Camera(Vector3d(0, 0, -10), Vector3d(0, 0, -1), Vector3d(0, 1, 0));

	createSceneGraph();

	//Start the slide puzzle game
	//Hardcoded: The third child of the sceneGraph's root node should be the piece's root node
	game = new GameSlidingPuzzle(
		SceneGraphManager::getInstance()->get(SLIDING_PUZZLE_SCENE_GRAPH)->getRoot()->getChildren().at(2)
		, 8);
	game->scramblePieces();
	game->setMouseMode(GameSlidingPuzzle::MouseMode::Drag);
}

void evalButton(GLFWwindow* win, float xpos, float ypos) {

	if (menuIshowing) { //Main menu
		//New Game
		if ((xpos >= 190.0f && xpos <= 445.0f) && (ypos >= 230.0f && ypos <= 298.0f)) {
			createNewSlidePuzzleGame();
			buttonPressed = true;
		}
		//Load Game
		else if ((xpos >= 190.0f && xpos <= 445.0f) && (ypos >= 340.0f && ypos <= 408.0f)) {
			cout << "Loading game...\n";
			SceneGraph* scene = sceneFileHandler->loadScene(SLIDING_PUZZLE_SCENE_GRAPH);
			camera = sceneFileHandler->getCamera(scene)[0]; // We only use one camera

			SceneGraphManager::getInstance()->get(MENU_SCENE_GRAPH)->setCamera(camera);
			SceneGraphManager::getInstance()->get(ESC_MENU_SCENE_GRAPH)->setCamera(camera);

			slidePuzzleGameFileHandler->loadGame(game);
			game->reload(scene->getRoot()->getChildren().at(2)); //Update/Reload references to nodes
			game->setPiecePositions(slidePuzzleGameFileHandler->piecesPositions); //Update positions of nodes in game
			buttonPressed = true;
		}
		//Exit Game
		else if ((xpos >= 190.0f && xpos <= 445.0f) && (ypos >= 460.0f && ypos <= 528.0f)) {
			exitgame = true;
			buttonPressed = true;
		}
	}

	else if (pause) { //Menu Pausa (ESC)
		//Continue
		if ((xpos >= 188.0f && xpos <= 445.0f) && (ypos >= 90.0f && ypos <= 152.0f)) {
			buttonPressed = true;
		}
		//Save
		else if ((xpos >= 188.0f && xpos <= 445.0f) && (ypos >= 200.0f && ypos <= 262.0f)) {
			cout << "Saving game...\n";
			sceneFileHandler->saveScene(SceneGraphManager::getInstance()->get(SLIDING_PUZZLE_SCENE_GRAPH));
			slidePuzzleGameFileHandler->saveGame(game);
			buttonPressed = true;
		}
		//Load
		else if ((xpos >= 188.0f && xpos <= 445.0f) && (ypos >= 310.0f && ypos <= 372.0f)) {
			cout << "Loading game...\n";
			SceneGraph* scene = sceneFileHandler->loadScene(SLIDING_PUZZLE_SCENE_GRAPH);
			camera = sceneFileHandler->getCamera(scene)[0]; // We only use one camera

			SceneGraphManager::getInstance()->get(MENU_SCENE_GRAPH) -> setCamera(camera);
			SceneGraphManager::getInstance()->get(ESC_MENU_SCENE_GRAPH)->setCamera(camera);

			slidePuzzleGameFileHandler->loadGame(game);
			game->reload(scene->getRoot()->getChildren().at(2)); //Update/Reload references to nodes
			game->setPiecePositions(slidePuzzleGameFileHandler->piecesPositions); //Update positions of nodes in game
			buttonPressed = true;
		}
		//Snap
		else if ((xpos >= 188.0f && xpos <= 445.0f) && (ypos >= 418.0f && ypos <= 478.0f)) {
			//Take a  snapshot
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			pause = false;
			menuIshowing = false;
			drawScene();
			takeSnapshot();
			buttonPressed = true;
		}
		//Exit
		else if ((xpos >= 188.0f && xpos <= 445.0f) && (ypos >= 532.0f && ypos <= 584.0f)) {
			exitomenu = true;
			buttonPressed = true;
		}
	}
}

#pragma endregion
/////////////////////////////////////////////////////////////////////////////// Callbacks
#pragma region CALLBACKS
void window_close_callback(GLFWwindow* win)
{
	delete ShaderProgramManager::getInstance();
	delete SceneGraphManager::getInstance();
	delete MeshManager::getInstance();
}

void window_size_callback(GLFWwindow* win, int winx, int winy)
{
	glViewport(0, 0, winx, winy);
	width = winx;
	height = winy;
}

void glfw_error_callback(int error, const char* description)
{
	std::cerr << "GLFW Error: " << description << std::endl;
}

void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		if (menuIshowing == false) {
			pause = true;
		}
	}
	game->handleKeyboardInput(key, action);

	if (action == GLFW_PRESS || action == GLFW_RELEASE) {
		if (menuIshowing == false && pause == false) {
			camera->updateCameraPos(key, action);
		}
	}

	if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		if (menuIshowing == false && pause == false) {
			camera->changeProjectionType();
		}
	}

	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		camera->changeRotationType();
	}

	//P : Save the slidePuzzleScene and game
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		cout << "Saving game...\n";
		sceneFileHandler->saveScene(SceneGraphManager::getInstance()->get(SLIDING_PUZZLE_SCENE_GRAPH));
		slidePuzzleGameFileHandler->saveGame(game);
	}
	//L : Load the last saved slidePuzzleScene and game
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		cout << "Loading game...\n";
		SceneGraph* scene = sceneFileHandler->loadScene(SLIDING_PUZZLE_SCENE_GRAPH);
		camera = sceneFileHandler->getCamera(scene)[0]; // We only use one camera

		SceneGraphManager::getInstance()->get(MENU_SCENE_GRAPH)->setCamera(camera);
		SceneGraphManager::getInstance()->get(ESC_MENU_SCENE_GRAPH)->setCamera(camera);

		slidePuzzleGameFileHandler->loadGame(game);
		game->reload(scene->getRoot()->getChildren().at(2)); //Update/Reload references to nodes
		game->setPiecePositions(slidePuzzleGameFileHandler->piecesPositions); //Update positions of nodes in game

	}
	//O : Start a new slidePuzzle game, without creating a new scene
	if (key == GLFW_KEY_O && action == GLFW_PRESS)
	{
		game->scramblePieces();
	}

	//Z : Start a new slidePuzzle game, on a new scene
	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		createNewSlidePuzzleGame();
	}

	//M : Change mouse input for game
	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		string mode = game->changeMouseMode();
		cout << "Mouse mode change to : " + mode + "\n";
	}
}

int right_mouse_pressed = 0;
int left_mouse_pressed = 0;

void mouse_callback(GLFWwindow* win, double xpos, double ypos)
{
	if (menuIshowing == false && pause == false) {
		camera->look((float)xpos, (float)ypos, right_mouse_pressed);
		game->handleMouseDrag((float)xpos, (float)ypos, left_mouse_pressed);
	}
}

void mouse_button_callback(GLFWwindow* win, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		right_mouse_pressed = action;
	}

	else if (button == GLFW_MOUSE_BUTTON_LEFT) {

		//Se não estiver num Menu
		if (menuIshowing == false && pause == false) {

			if (action == GLFW_PRESS) {
				double xpos, ypos;
				glfwGetCursorPos(win, &xpos, &ypos);

				int viewport[4];
				glGetIntegerv(GL_VIEWPORT, viewport);

				GLint index;
				glReadPixels((GLint)xpos, (GLint)(viewport[3] - ypos), 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

				game->handleMouseClick((float)xpos, (float)ypos, index);
			}
			else if (action == GLFW_RELEASE) {
				game->releasePiece();
			}
		}

		else { //Quando está num menu

			if (action == GLFW_PRESS) {

				double xpos, ypos;
				glfwGetCursorPos(win, &xpos, &ypos);

				evalButton(win, (float)xpos, (float)ypos);
			}

			else if (action == GLFW_RELEASE) {

				//Main Menu
				if (menuIshowing && buttonPressed && exitgame) { //Se foi carregado o botão exit
					glfwSetWindowShouldClose(win, GLFW_TRUE);
					window_close_callback(win);
				}

				else if (menuIshowing && buttonPressed) { //Se foi carregado um botão
					menuIshowing = false;
					buttonPressed = false;
				}

				//Pausa (ESC MENU)
				else if (pause && buttonPressed && exitomenu) { //Se foi carregado o botão exit
					pause = false;
					buttonPressed = false;
					exitomenu = false;
					menuIshowing = true;
				}

				else if (pause && buttonPressed) { //Se foi carregado um botão
					pause = false;
					buttonPressed = false;
				}
			}
		}

		left_mouse_pressed = action;
	}
}

#pragma endregion
/////////////////////////////////////////////////////////////////////////////// Setup
#pragma region SETUP
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
	glClearColor(0.1f, 0.1f, 0.5f, 1.0f);
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

	//Set filehandlers
	sceneFileHandler = SceneFileHandler::getInstance();
	slidePuzzleGameFileHandler = SlidePuzzleGameFileHandler::getInstance();

	//First, load the default "assets" the app needs
	loadDefaultTextures();
	loadDefaultPreDrawFunctions();

	createMenus();

	// By default, we always start a new game, on a new scene
	createNewSlidePuzzleGame(); 

	return win;

#ifdef ERROR_CALLBACK
	setupErrorCallback();
#endif
}

#pragma endregion
/////////////////////////////////////////////////////////////////////////////// Run
#pragma region RUN
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

		glClearStencil(0);
		// Double Buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
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

#pragma endregion
/////////////////////////////////////////////////////////////////////////////// Main
int main(int argc, char* argv[])
{
	int gl_major = 4, gl_minor = 3;
	int is_fullscreen = 0;
	int is_vsync = 1;
	width = 640;
	height = 640;
	GLFWwindow* win = setup(gl_major, gl_minor,
		width, height, "Team project", is_fullscreen, is_vsync);
	run(win);

	exit(EXIT_SUCCESS);
}
/////////////////////////////////////////////////////////////////////////////// End