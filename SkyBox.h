#pragma once

#include <stdlib.h> 
#include "texturing.h"

//Maths for OpenGL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Cg/cgSetup.h"

class SkyBox
{
		int SKYSIZE;

public:
	SkyBox(int SS);
	GLuint SkyBoxList(GLuint skyTexId[]);
};
