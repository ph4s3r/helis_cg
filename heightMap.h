#pragma once

#include <gl\glut.h>

//Maths for OpenGL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Cg/cgSetup.h"

#include <windows.h>									// Header File For Windows
#include <stdio.h>										// Header file for standard input output (IE, "FILE") (NEW)

#define		MAP_SIZE	  512	
#define		STEP_SIZE	  1	

void LoadRawFile(LPSTR strName, int nSize, BYTE *pHeightMap);

int GetHeight(BYTE *pHeightMap, int X, int Y)	;

void RenderHeightMap(BYTE pHeightMap[], GLuint heightMapTexId[])	;

void DrawHeightMap(GLuint heightMapTexId[]);
