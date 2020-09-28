#pragma once

//Maths for OpenGL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Cg/cgSetup.h"

#include <windows.h>									// Header File For Windows
#include <stdio.h>										// Header file for standard input output (IE, "FILE") (NEW)
#include <gl\glut.h>

#include "SOIL.h"

#define		MAP_SIZE	  512	
#define		STEP_SIZE	  1					

void LoadRawFile(LPSTR strName, int nSize, unsigned char *pHeightMap);

int GetHeight(unsigned char &pHeightMap, int X, int Y)	;

void RenderHeightMap(unsigned char &pHeightMap)	;

GLuint DrawHeightMap()	;
