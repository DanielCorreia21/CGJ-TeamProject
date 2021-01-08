#pragma once
#include <string>

using namespace std;

const string SLIDING_PUZZLE_SCENE_GRAPH = "SlidingPuzzle";
const string COLOR_SHADER = "color";
const string PIECES_SHADER = "MarbleShader";
const string COLOR_UNIFORM = "Color";
const string TEXTURE_UNIFORM_COLOR = "Texture";
const string TEXTURE_UNIFORM_NOISE = "NoiseTexture";
const string CUBE_MESH = "cube";
const string DISC_MESH = "disc";

const char colorVertexShaderPath[] = "../Resources/color_vs.glsl";
const char colorFragmentShaderPath[] = "../Resources/color_fs.glsl";
const char textureVertexShaderPath[] = "../Resources/marble_vs.glsl";
const char textureFragmentShaderPath[] = "../Resources/marble_fs.glsl";
const string cubeMeshPath = "../objs/cube5.obj";
const string discMeshPath = "../objs/disc.obj";