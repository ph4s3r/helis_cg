
#include "heightMap.h"

BYTE g_HeightMap[MAP_SIZE*MAP_SIZE];			

void LoadRawFile(LPSTR strName, int nSize, unsigned char *pHeightMap)
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

int GetHeight(unsigned char pHeightMap, int X, int Y)
{
	if(X == MAP_SIZE)X = MAP_SIZE+STEP_SIZE;
	if(Y == MAP_SIZE)Y = MAP_SIZE+STEP_SIZE;
	int x = X % MAP_SIZE;
	int y = Y % MAP_SIZE;

	if(!&pHeightMap) printf("Fail! Heightmap Data is Invalid!");

	return &pHeightMap[x + (y * MAP_SIZE)];
}
	
//Modified to unsigned char
/*
void RenderHeightMap(BYTE pHeightMap[])	
{

	if(!pHeightMap) printf("Fail! Heightmap Data is Invalid!");
	
	int MAX = (MAP_SIZE-STEP_SIZE);

	for (int X = 1; X < MAX*2+1; X += STEP_SIZE ) {
		glBegin( GL_TRIANGLE_STRIP );
		for (int Y = 1; Y < MAX*2+1; Y += STEP_SIZE )
		{					
			glTexCoord2f((float(X)/float(MAX))*2, (float(Y)/float(MAX))*2);
			glVertex3i(X, GetHeight(pHeightMap, X, Y ), Y);

			glTexCoord2f((float(X+1)/float(MAX))*2, (float(Y)/float(MAX))*2);
			glVertex3i(X + STEP_SIZE, GetHeight(pHeightMap, X + STEP_SIZE, Y), Y);
		}
		glEnd();
	}
	
}*/

void RenderHeightMap(unsigned char &pHeightMap)	
{

	if(!&pHeightMap) printf("Fail! Heightmap Data is Invalid!");
	
	int MAX = (MAP_SIZE-STEP_SIZE);

	for (int X = 1; X < MAX*2+1; X += STEP_SIZE ) {
		glBegin( GL_TRIANGLE_STRIP );
		for (int Y = 1; Y < MAX*2+1; Y += STEP_SIZE )
		{					
			glTexCoord2f((float(X)/float(MAX))*2, (float(Y)/float(MAX))*2);
			glVertex3i(X, GetHeight(&pHeightMap, X, Y ), Y);

			glTexCoord2f((float(X+1)/float(MAX))*2, (float(Y)/float(MAX))*2);
			glVertex3i(X + STEP_SIZE, GetHeight(&pHeightMap, X + STEP_SIZE, Y), Y);
		}
		glEnd();
	}
	
}

GLuint DrawHeightMap()	
{
	//LoadRawFile("GameLib/bumproll.raw", MAP_SIZE * MAP_SIZE, g_HeightMap);

		//New SOIL based heightmap loading
	int hmwidth = 512;
	int hmheight = 512;
	int hmchannels = 1; //greyscale

	unsigned char *g_HeightMap = SOIL_load_image
	(
		"GameLib/bumproll.raw",
		&hmwidth, &hmheight, &hmchannels,
		SOIL_LOAD_L
	);
	
	GLuint list;
	
	list = glGenLists(1);
	glNewList(list, GL_COMPILE);
				RenderHeightMap(*g_HeightMap);
	glEndList();

  return list;
}