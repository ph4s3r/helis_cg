	
#include "heightMap_multitext.h"

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
	

void RenderHeightMap(BYTE pHeightMap[])	
{
	if(!pHeightMap) printf("Fail! Heightmap Data is Invalid!");
	
	int MAX = (MAP_SIZE-STEP_SIZE);

	for (int X = 1; X < MAX*2+1; X += STEP_SIZE ) {
		glBegin( GL_TRIANGLE_STRIP );
		for (int Y = 1; Y < MAX*2+1; Y += STEP_SIZE )
		{	
			int heightxy = GetHeight(pHeightMap, X, Y );
			//Texture Coord bound by height
			if(heightxy < 100) glMultiTexCoord2f(GL_TEXTURE0, (float(X)/float(MAX))*2, (float(Y)/float(MAX))*2);
			else if(heightxy >= 100 && heightxy < 150)  glMultiTexCoord2f(GL_TEXTURE1, (float(X)/float(MAX))*2, (float(Y)/float(MAX))*2);
			else glMultiTexCoord2f(GL_TEXTURE2, (float(X)/float(MAX))*2, (float(Y)/float(MAX))*2);
			glVertex3i(X, heightxy, Y);

			//Texture Coord bound by height
			if(heightxy < 100) glMultiTexCoord2f(GL_TEXTURE0, (float(X+1)/float(MAX))*2, (float(Y)/float(MAX))*2);
			else if(heightxy >= 100 && heightxy < 150) glMultiTexCoord2f(GL_TEXTURE1, (float(X+1)/float(MAX))*2, (float(Y)/float(MAX))*2);
			else glMultiTexCoord2f(GL_TEXTURE2, (float(X+1)/float(MAX))*2, (float(Y)/float(MAX))*2);
			glVertex3i(X + STEP_SIZE, GetHeight(pHeightMap, X + STEP_SIZE, Y), Y);
		}
		glEnd();
	}
	
}

GLuint DrawHeightMap()	
{
	LoadRawFile("GameLib/bumproll.raw", MAP_SIZE * MAP_SIZE, g_HeightMap);
	
	GLuint list;
	
	list = glGenLists(1);
	glNewList(list, GL_COMPILE);
				RenderHeightMap(g_HeightMap);
	glEndList();

  return list;
}