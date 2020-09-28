#include "SkyBox.h"

SkyBox::SkyBox(int SS)
{
	SKYSIZE = SS;
}

GLuint SkyBox::SkyBoxList(GLuint skyTexId[]){

	GLuint list;

	    //glm::vec3 array for storing vertices, then computing normals (5 faces - no bottom)
  glm::vec3 vertices[6][4];
		//Temp vars for calculating normals
  glm::vec3 p1, p2;

  glm::vec3 normals[6];

  //Using CallList for performance
  list = glGenLists(1);
  glNewList(list, GL_COMPILE);
	glActiveTexture(GL_TEXTURE0);
	glEnable (GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, skyTexId[0]);
			//Clamp texture fetches
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_EXT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_EXT);
	cgGLSetTextureParameter(myCgFragmentParam_decal, skyTexId[0]);
	cgUpdateProgramParameters(myFragmentProgram);

		vertices[0][0] = glm::vec3((2*SKYSIZE), 0.0, 0.0);
		vertices[0][1] = glm::vec3(0.0, 0.0, 0.0);
		vertices[0][2] = glm::vec3(0.0,  -(2*SKYSIZE), 0.0);
		vertices[0][3] = glm::vec3((2*SKYSIZE),  -(2*SKYSIZE), 0.0);
		
		p1 = vertices[0][0] - vertices[0][1];
		p2 = vertices[0][1] - vertices[0][2];

		normals[0] = -glm::cross(p1, p2);

	glBegin(GL_QUADS);

		glNormal3f(normals[0].x, normals[0].y, normals[0].z);
		glTexCoord2f(0, 0); glVertex3i(  vertices[0][0].x, vertices[0][0].y, vertices[0][0].z );
        glTexCoord2f(1, 0); glVertex3i( vertices[0][1].x, vertices[0][1].y, vertices[0][1].z );
        glTexCoord2f(1, 1); glVertex3i( vertices[0][2].x, vertices[0][2].y, vertices[0][2].z );
        glTexCoord2f(0, 1); glVertex3i(  vertices[0][3].x, vertices[0][3].y, vertices[0][3].z );
    
	glEnd();

    glBindTexture(GL_TEXTURE_2D, skyTexId[1]);
			//Clamp texture fetches
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_EXT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_EXT);
	cgGLSetTextureParameter(myCgFragmentParam_decal, skyTexId[1]);
	cgUpdateProgramParameters(myFragmentProgram);

		vertices[1][0] = glm::vec3((2*SKYSIZE), 0.0,  (2*SKYSIZE));
		vertices[1][1] = glm::vec3((2*SKYSIZE), 0.0, 0.0);
		vertices[1][2] = glm::vec3((2*SKYSIZE),  -(2*SKYSIZE), 0.0);
		vertices[1][3] = glm::vec3((2*SKYSIZE),  -(2*SKYSIZE),  (2*SKYSIZE));
		
		p1 = vertices[1][0] - vertices[1][1];
		p2 = vertices[1][1] - vertices[1][2];

		normals[1] = -glm::cross(p1, p2);

	glBegin(GL_QUADS);
	
		glNormal3f(normals[1].x, normals[1].y, normals[1].z);
		glTexCoord2f(0, 0); glVertex3i(  vertices[1][0].x, vertices[1][0].y, vertices[1][0].z );
        glTexCoord2f(1, 0); glVertex3i( vertices[1][1].x, vertices[1][1].y, vertices[1][1].z );
        glTexCoord2f(1, 1); glVertex3i( vertices[1][2].x, vertices[1][2].y, vertices[1][2].z );
        glTexCoord2f(0, 1); glVertex3i(  vertices[1][3].x, vertices[1][3].y, vertices[1][3].z );

    glEnd();

    glBindTexture(GL_TEXTURE_2D, skyTexId[2]);
			//Clamp texture fetches
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_EXT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_EXT);
	cgGLSetTextureParameter(myCgFragmentParam_decal, skyTexId[2]);
	cgUpdateProgramParameters(myFragmentProgram);

		vertices[2][0] = glm::vec3(0.0, 0.0,  (2*SKYSIZE));
		vertices[2][1] = glm::vec3((2*SKYSIZE), 0.0,  (2*SKYSIZE));
		vertices[2][2] = glm::vec3((2*SKYSIZE),  -(2*SKYSIZE),  (2*SKYSIZE));
		vertices[2][3] = glm::vec3(0.0,  -(2*SKYSIZE),  (2*SKYSIZE));
		
		p1 = vertices[2][0] - vertices[2][1];
		p2 = vertices[2][1] - vertices[2][2];

		normals[2] = -glm::cross(p1, p2);

	 glBegin(GL_QUADS);

		glNormal3f(normals[2].x, normals[2].y, normals[2].z);
		glTexCoord2f(0, 0); glVertex3i(  vertices[2][0].x, vertices[2][0].y, vertices[2][0].z );
        glTexCoord2f(1, 0); glVertex3i( vertices[2][1].x, vertices[2][1].y, vertices[2][1].z );
        glTexCoord2f(1, 1); glVertex3i( vertices[2][2].x, vertices[2][2].y, vertices[2][2].z );
        glTexCoord2f(0, 1); glVertex3i(  vertices[2][3].x, vertices[2][3].y, vertices[2][3].z );

    glEnd();

    glBindTexture(GL_TEXTURE_2D, skyTexId[3]);
			//Clamp texture fetches
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_EXT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_EXT);
	cgGLSetTextureParameter(myCgFragmentParam_decal, skyTexId[3]);
	cgUpdateProgramParameters(myFragmentProgram);    

		vertices[3][0] = glm::vec3(0.0, 0.0, 0.0);
		vertices[3][1] = glm::vec3(0.0, 0.0,  (2*SKYSIZE));
		vertices[3][2] = glm::vec3(0.0,  -(2*SKYSIZE),  (2*SKYSIZE));
		vertices[3][3] = glm::vec3(0.0,  -(2*SKYSIZE), 0.0);
		
		p1 = vertices[3][0] - vertices[3][1];
		p2 = vertices[3][1] - vertices[3][2];

		normals[3] = -glm::cross(p1, p2);

	glBegin(GL_QUADS);
	
		glNormal3f(normals[3].x, normals[3].y, normals[3].z);
		glTexCoord2f(0, 0); glVertex3i(  vertices[3][0].x, vertices[3][0].y, vertices[3][0].z );
        glTexCoord2f(1, 0); glVertex3i( vertices[3][1].x, vertices[3][1].y, vertices[3][1].z );
        glTexCoord2f(1, 1); glVertex3i( vertices[3][2].x, vertices[3][2].y, vertices[3][2].z );
        glTexCoord2f(0, 1); glVertex3i(  vertices[3][3].x, vertices[3][3].y, vertices[3][3].z );

    glEnd();

    glBindTexture(GL_TEXTURE_2D, skyTexId[4]);
			//Clamp texture fetches
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_EXT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_EXT);
	cgGLSetTextureParameter(myCgFragmentParam_decal, skyTexId[4]);
	cgUpdateProgramParameters(myFragmentProgram);

		vertices[4][0] = glm::vec3(0.0,  -(2*SKYSIZE), 0.0);
		vertices[4][1] = glm::vec3(0.0,  -(2*SKYSIZE),  (2*SKYSIZE));
		vertices[4][2] = glm::vec3((2*SKYSIZE),  -(2*SKYSIZE),  (2*SKYSIZE));
		vertices[4][3] = glm::vec3((2*SKYSIZE),  -(2*SKYSIZE), 0.0);
		
		p1 = vertices[4][0] - vertices[4][1];
		p2 = vertices[4][1] - vertices[4][2];

		normals[4] = -glm::cross(p1, p2);

	glBegin(GL_QUADS);

		glNormal3f(normals[4].x, normals[4].y, normals[4].z);
		glTexCoord2f(1, 1); glVertex3i(  vertices[4][0].x, vertices[4][0].y, vertices[4][0].z );
        glTexCoord2f(0, 1); glVertex3i( vertices[4][1].x, vertices[4][1].y, vertices[4][1].z );
        glTexCoord2f(0, 0); glVertex3i( vertices[4][2].x, vertices[4][2].y, vertices[4][2].z );
        glTexCoord2f(1, 0); glVertex3i(  vertices[4][3].x, vertices[4][3].y, vertices[4][3].z );

    glEnd();

	glBindTexture(GL_TEXTURE_2D, skyTexId[5]);
			//Clamp texture fetches
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_EXT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_EXT);
	cgGLSetTextureParameter(myCgFragmentParam_decal, skyTexId[5]);
	cgUpdateProgramParameters(myFragmentProgram);

	normals[4] = -glm::cross(p1, p2);

	glBegin(GL_QUADS);
		glNormal3f(0.0, 1.0, 0.0);
		glTexCoord2f(0, 0); glVertex3i(  vertices[5][0].x, vertices[5][0].y, vertices[5][0].z );
        glTexCoord2f(1, 0); glVertex3i( vertices[5][1].x, vertices[5][1].y, vertices[5][1].z );
        glTexCoord2f(1, 1); glVertex3i( vertices[5][2].x, vertices[5][2].y, vertices[5][2].z );
        glTexCoord2f(0, 1); glVertex3i(  vertices[5][3].x, vertices[5][3].y, vertices[5][3].z );

    glEnd();

	glDisable (GL_TEXTURE_2D);
  glEndList();

  return list;

}
