#pragma once
#include <string>

using namespace std;

bool menuIshowing = true;
bool pause = false;
bool buttonPressed = false;
bool exitomenu = false;
bool exitgame = false;

const string SLIDING_PUZZLE_SCENE_GRAPH = "SlidingPuzzle";
const string MENU_SCENE_GRAPH = "MainMenu";
const string ESC_MENU_SCENE_GRAPH = "EscMenu";
const string MENU_SHADER = "menu";
const string COLOR_SHADER = "color";
const string PIECES_SHADER = "MarbleShader";
const string FRAME_SHADER = "GraniteShader";
const string BACKBOARD_SHADER = "Wood Shader";
const string COLOR_UNIFORM = "Color";
const string TEXTURE_UNIFORM_COLOR = "Texture";
const string TEXTURE_UNIFORM_NOISE = "NoiseTexture";
const string CUBE_MESH = "cube";
const string DISC_MESH = "disc";

const char colorVertexShaderPath[] = "../Resources/color_vs.glsl";
const char colorFragmentShaderPath[] = "../Resources/color_fs.glsl";
const char textureVertexShaderPath[] = "../Resources/texture_vs.glsl";
const char marbleFragmentShaderPath[] = "../Resources/marble_fs.glsl";
const char graniteFragmentShaderPath[] = "../Resources/granite_fs.glsl";
const char menuFragmentShaderPath[] = "../Resources/menu_fs.glsl";
const char woodFragmentShaderPath[] = "../Resources/wood_fs.glsl";

const string snapshots_directory = "../Snapshots/";

const string cubeMeshPath = "../objs/cube5.obj";
const string discMeshPath = "../objs/disc.obj";