#pragma once

//Maths for OpenGL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Cg/cgSetup.h"

#define		MAP_SIZE	  512	
#define		STEP_SIZE	  1	

void LoadRawFile(LPSTR strName, int nSize, BYTE *pHeightMap);

int GetHeight(BYTE *pHeightMap, int X, int Y)	;

void RenderHeightMap(BYTE pHeightMap[])	;

GLuint DrawHeightMap()	;
