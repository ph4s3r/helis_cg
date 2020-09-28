
#include "heightMap.h"


BYTE g_HeightMap[MAP_SIZE*MAP_SIZE];	

void LoadRawFile(LPSTR strName, int nSize, BYTE *pHeightMap)
{
	FILE *pFile = NULL;

	pFile = fopen( strName, "rb" );

	if ( pFile == NULL )	
	{

		printf("Can't Find The Height Map!");
		return;
	}

	fread( pHeightMap, 1, nSize, pFile );

	int result = ferror( pFile );

	if (result)
	{
		printf("Failed To Get Data!");
	}

	fclose(pFile);
}

int GetHeight(BYTE *pHeightMap, int X, int Y)
{
	if(X == (int)MAP_SIZE)    X = (int)MAP_SIZE+(int)STEP_SIZE;
	if(Y == (int)MAP_SIZE)    Y = (int)MAP_SIZE+(int)STEP_SIZE;
	int x = X % (int)MAP_SIZE;
	int y = Y %(int) MAP_SIZE;

	if(!pHeightMap) printf("Fail! Heightmap Data is Invalid!");

	return pHeightMap[(x + (y * MAP_SIZE))];
}
	
/*
//Old version w/o multitexturing
void RenderHeightMap(BYTE pHeightMap[])	
{
	if(!pHeightMap) printf("Fail! Heightmap Data is Invalid!");
	
	int MAX = (MAP_SIZE-STEP_SIZE);

	for (int X = 1; X < MAX*2+1; X += STEP_SIZE ) {
		glBegin( GL_TRIANGLE_STRIP );
		for (int Y = 1; Y < MAX*2+1; Y += STEP_SIZE )
		{	
			int heightxy = GetHeight(pHeightMap, X, Y );
			glTexCoord2f((float(X)/float(MAX))*2, (float(Y)/float(MAX))*2);
			glVertex3i(X, heightxy, Y);

			int heightxSy = GetHeight(pHeightMap, X + STEP_SIZE, Y);
			glTexCoord2f((float(X+1)/float(MAX))*2, (float(Y)/float(MAX))*2);
			glVertex3i(X + STEP_SIZE, heightxSy, Y);
		}
		glEnd();
	}
	
}
*/

void RenderHeightMap(BYTE pHeightMap[], GLuint heightMapTexId[])	
{	
	if(!pHeightMap) printf("Fail! Heightmap Data is Invalid!");
	glEnable (GL_TEXTURE_2D);
	
	int MAX = (MAP_SIZE-STEP_SIZE);

	for (int X = 1; X < MAX*2+1; X += STEP_SIZE ) {
		glBegin( GL_TRIANGLE_STRIP );
		for (int Y = 1; Y < MAX*2+1; Y += STEP_SIZE )
		{	
			int heightxy = GetHeight(pHeightMap, X, Y );
			//Pushing height value to fragment shader
			cgSetParameter1i(myCgFragmentParam_heightVal, heightxy);
			cgUpdateProgramParameters(myFragmentProgram);

			glMultiTexCoord2f(GL_TEXTURE0, (float(X)/float(MAX))*2, (float(Y)/float(MAX))*2);
			glMultiTexCoord2f(GL_TEXTURE1, (float(X)/float(MAX))*2, (float(Y)/float(MAX))*2);
			glMultiTexCoord2f(GL_TEXTURE2, (float(X)/float(MAX))*2, (float(Y)/float(MAX))*2);
			glVertex3i(X, heightxy, Y);

			glMultiTexCoord2f(GL_TEXTURE0, (float(X+1)/float(MAX))*2, (float(Y)/float(MAX))*2);
			glMultiTexCoord2f(GL_TEXTURE1, (float(X+1)/float(MAX))*2, (float(Y)/float(MAX))*2);
			glMultiTexCoord2f(GL_TEXTURE2, (float(X+1)/float(MAX))*2, (float(Y)/float(MAX))*2);
			glVertex3i(X + STEP_SIZE, GetHeight(pHeightMap, X + STEP_SIZE, Y), Y);
		}
		glEnd();
	}
	
}

/*
GLuint DrawHeightMap(GLuint heightMapTexId[])	
{
	LoadRawFile("GameLib/bumproll.raw", MAP_SIZE * MAP_SIZE, g_HeightMap);
	
	GLuint list;
	
	list = glGenLists(1);
	glNewList(list, GL_COMPILE);
				RenderHeightMap(g_HeightMap, heightMapTexId);
	glEndList();

  return list;
}*/


void DrawHeightMap(GLuint heightMapTexId[])	
{
	LoadRawFile("GameLib/bumproll.raw", MAP_SIZE * MAP_SIZE, g_HeightMap);

					//Pushing snowy texture to GL_TEXTURE0 unit
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, heightMapTexId[0]);
				cgGLSetTextureParameter(myCgFragmentParam_decal, heightMapTexId[0]);
				checkForCgError("setting decal 2D texture:heightMapTexId for heightmap");
				
					//Pushing dirty texture to GL_TEXTURE1 unit
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, heightMapTexId[1]);
				cgGLSetTextureParameter(myCgFragmentParam_decal1, heightMapTexId[1]);
				checkForCgError("setting decal 2D texture:heightMapTexId for heightmap");
				
					//Pushing grassy texture to GL_TEXTURE2 unit
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, heightMapTexId[2]);
				cgGLSetTextureParameter(myCgFragmentParam_decal2, heightMapTexId[2]);
				checkForCgError("setting decal 2D texture:heightMapTexId for heightmap");
				cgUpdateProgramParameters(myFragmentProgram);					
			
	//GLuint list;
	
	//list = glGenLists(1);
	//glNewList(list, GL_COMPILE);
				RenderHeightMap(g_HeightMap, heightMapTexId);
	//glEndList();

  //return list;
}