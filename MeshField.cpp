//MeshField.h
//20106_田中　蓮
//24_09_17
#include "MeshField.h"
#include "Manager.h"
#include "ShaderManager.h"
#include "Scene.h"
#include "Camera.h"

float g_FieldHeight[42][42] =
{
	{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f },
	{ 1.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f, 1.0f },
	{ 1.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f, 1.0f },
	{ 1.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f, 1.0f },
	{ 1.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f, 1.0f },
	{ 1.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f, 1.0f },
	{ 1.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f, 1.0f },
	{ 1.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f, 1.0f },
	{ 1.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f, 1.0f },
	{ 1.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f, 1.0f },
	{ 1.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f,-6.0f, 1.0f },
	{ 1.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f, 1.0f },
	{ 1.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f, 1.0f },
	{ 1.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f, 1.0f },
	{ 1.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f, 1.0f },
	{ 1.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f, 1.0f },
	{ 1.0f,-2.0f,-2.0f,-2.0f,-2.0f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f, 1.0f },
	{ 1.0f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f, 1.0f },
	{ 1.0f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.1f,-1.1f,-1.1f,-1.1f,-1.1f,-1.1f,-1.1f,-1.1f,-1.1f,-1.1f,-1.1f, 1.0f },
	{ 1.0f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.0f,-1.0f,-1.0f,-1.0f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f, 1.0f },
	{ 1.0f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f, 1.0f },
	{ 1.0f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f, 0.0f,-0.8f, 1.0f },
	{ 1.0f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,-0.8f, 1.0f },
	{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,-0.8f, 1.0f },
	{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,-0.8f, 1.0f },
	{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f },
	{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.5f, 1.0f, 2.0f, 2.0f, 2.0f, 2.0f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f },
	{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 1.0f, 2.0f, 2.0f, 2.0f, 2.0f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f },
	{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 1.0f, 2.0f, 2.0f, 2.0f, 2.0f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f },
	{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 1.0f, 2.0f, 2.0f, 2.0f, 2.0f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f },
	{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.5f, 1.0f, 1.0f, 0.8f, 1.0f, 1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f },
	{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f },
	{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f },
	{ 1.0f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f, 1.0f },
	{ 1.0f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f, 1.0f },
	{ 1.0f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f, 1.0f },
	{ 1.0f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f, 1.0f },
	{ 1.0f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f, 1.0f },
	{ 1.0f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f, 1.0f },
	{ 1.0f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f, 1.0f },
	{ 1.0f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-0.7f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f,-1.5f, 1.0f },
	{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }
};
void MeshField::Init() {
	//頂点バッファ生成
	{
		for (int x = 0; x < VERTEX_NUM; x++) {
			for (int z = 0; z < VERTEX_NUM; z++) {
				_vertex[x][z].Position = XMFLOAT3((x - (VERTEX_NUM * 0.5f)) * 5.0f, g_FieldHeight[x][z], (z - (VERTEX_NUM * 0.5f)) * -5.0f);
				_vertex[x][z].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
				_vertex[x][z].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) * (_vertex[x][z].Position.y / 6.0f) + 0.5f;
				_vertex[x][z].TexCoord = XMFLOAT2((float)x, (float)z);
				_vertex[x][z].Tangent = XMFLOAT3(1.0f, 0.0f, 0.0f);
			}
		}

		//法線ベクトル算出
		for (int x = 1; x < VERTEX_NUM - 1; x++) {
			for (int z = 1; z < VERTEX_NUM - 1; z++) {
				XMFLOAT3	vx, vz, vn;
				vx = _vertex[x + 1][z].Position - _vertex[x - 1][z].Position;

				vz = _vertex[x][z - 1].Position - _vertex[x][z + 1].Position;

				//外積
				vn.x = vz.y * vx.z - vz.z * vx.y;
				vn.y = vz.z * vx.x - vz.x * vx.z;
				vn.z = vz.x * vx.y - vz.y * vx.x;

				float len = sqrt(vn.x * vn.x + vn.y * vn.y + vn.z * vn.z);
				vn /= len;

				_vertex[x][z].Normal = vn;
			}
		}



		D3D11_BUFFER_DESC bd{};
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * VERTEX_NUM * VERTEX_NUM;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA sd{};
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = _vertex;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &_vertexbuffer);
	}

	//インデックスバッファ生成
	unsigned int index[((VERTEX_NUM + 1) * 2) * (VERTEX_NUM - 1) - 2];

	int i = 0;
	for (int x = 0; x < VERTEX_NUM - 1; x++) {
		for (int z = 0; z < VERTEX_NUM; z++) {
			index[i] = x * VERTEX_NUM + z;
			i++;

			index[i] = (x + 1) * VERTEX_NUM + z;
			i++;
		}
		if (x == VERTEX_NUM - 2) {
			break;
		}
		index[i] = (x + 1) * VERTEX_NUM + VERTEX_NUM - 1;
		i++;

		index[i] = (x + 1) * VERTEX_NUM;
		i++;
	}
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(unsigned int) * (((VERTEX_NUM + 1) * 2) * (VERTEX_NUM - 1) - 2);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = index;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &_indexbuffer);


	//DDSでテクスチャ読み込み	BC5はノーマルマップで使う
	TexMetadata metadata;
	ScratchImage image;
	LoadFromDDSFile(_texname, DDS_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), image.GetMetadata(), &_texture);
	assert(_texture);
}

void MeshField::Uninit() {
	_vertexbuffer->Release();
	_texture->Release();
}

void MeshField::Update() {}

void MeshField::Draw() {
	Shader::SetShader(ShaderName::DepthShadow);


	//ワールドマトリクス設定
	XMMATRIX world, scale, trans, rot;
	scale = XMMatrixScaling(GetScale().x, GetScale().y, GetScale().z);
	rot = XMMatrixRotationRollPitchYaw(GetRotation().x, GetRotation().y, GetRotation().z);
	trans = XMMatrixTranslation(GetPosition().x, GetPosition().y, GetPosition().z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &_vertexbuffer, &stride, &offset);

	//インデックスバッファ設定
	Renderer::GetDeviceContext()->IASetIndexBuffer(_indexbuffer, DXGI_FORMAT_R32_UINT, 0);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &_texture);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	Renderer::GetDeviceContext()->DrawIndexed(((VERTEX_NUM + 1) * 2) * (VERTEX_NUM - 1) - 2, 0, 0);
}

float MeshField::GetHeight(const XMFLOAT3& pos) {
	int x, z;
	//ブロック番号算出
	x = pos.x / 5.0f + (VERTEX_NUM * 0.5f);
	z = pos.z / -5.0f + (VERTEX_NUM * 0.5f);

	XMFLOAT3 pos0, pos1, pos2, pos3;
	pos0 = _vertex[x][z].Position;
	pos1 = _vertex[x + 1][z].Position;
	pos2 = _vertex[x][z + 1].Position;
	pos3 = _vertex[x + 1][z + 1].Position;

	XMFLOAT3 v12, v1p;
	v12 = pos2 - pos1;
	v1p = pos - pos1;

	//外積
	float cy = v12.z * v1p.x - v12.x * v1p.z;

	float py;
	XMFLOAT3 n{};

	if (cy > 0.0f) {
		//左上ポリゴン
		XMFLOAT3 v10;
		v10 = pos0 - pos1;

		//外積
		n.x = v10.y * v12.z - v10.z * v12.y;
		n.y = v10.z * v12.x - v10.x * v12.z;
		n.z = v10.x * v12.y - v10.y * v12.x;
	}
	else {
		//右上ポリゴン
		XMFLOAT3 v13;
		v13 = pos3 - pos1;

		//外積
		n.x = v12.y * v13.z - v12.z * v13.y;
		n.y = v12.z * v13.x - v12.x * v13.z;
		n.z = v12.x * v13.y - v12.y * v13.x;
	}

	//高さ取得
	py = -((pos.x - pos1.x) * n.x + (pos.z - pos1.z) * n.z) / n.y + pos1.y;

	return py;
}
