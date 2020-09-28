/*
 *		This Code Was Created By Jeff Molofee 2000
 *		A HUGE Thanks To Fredric Echols For Cleaning Up
 *		And Optimizing The Base Code, Making It More Flexible!
 *		If You've Found This Code Useful, Please Let Me Know.
 *		Visit My Site At nehe.gamedev.net
 */
#pragma once

//glut.h modded with glew
#include <GL/glut.h>

#include <stdio.h>			//printf...

//Simple OpenGL Image Lib 
//(Note: the original .lib file had problems.. it was recompiled with the downloaded VC project)
#pragma comment (lib, "SOIL.lib")
#include "SOIL.h"

GLuint SOIL_LoadTexDirectly(char *Filename, unsigned int flags) ;