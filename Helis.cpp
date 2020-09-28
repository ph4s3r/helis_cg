/*
*** Helicopter game by Peter Karacsonyi ***
*/

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
// MsWindows-on ez is kell
#include <windows.h>
#endif // Win32 platform

#include <iostream>
//Texture reading with Simple OpenGL Image Lib.
#include "texturing.h"
//obj parsing & drawing with Nate Robins' glm
#include "glm.h"
//Rob Bateman's Keyboard handling
#include "keys.h"
//Simplified Heightmap Drawing based on "hegyivadasz jatek"
#include "heightMap.h"
//Simple SkyBox
#include "SkyBox.h"
//My cG setup
#include "Cg/cgSetup.h"

using namespace std;

#ifndef NUMOFHELIPARTS
#define NUMOFHELIPARTS 3
#endif

#ifndef NUMOFOTHERMODELS
#define NUMOFOTHERMODELS 2
#endif

int w_width = 1024;
int w_height = 768;
int time;	//GLUT_ELAPSED_TIME
float PI = 3.141592654;
float RAD = 180/PI;
float PI_of_180 = 0.017453292;

GLuint heli_model_list[NUMOFHELIPARTS];
GLuint other_model_list[NUMOFOTHERMODELS];

GLfloat    smoothing_angle = 90.0;	/* smoothing angle */
GLfloat    scale[NUMOFHELIPARTS];	/* scaling factor: computed by glm for manual 'un-scale' (if necessary) */

GLuint 	   heliTexId;	//Storage for Heli texture
GLuint     heightMapTexId[3];	//Storage for HeightMap texture

GLuint skyTexId[6];			//Storage for SkyBox textures

glm::vec3 eyeposition;		/*Eye in Object Space*/

// ********************* Cg Init Begin********************* \\

void InitCg( void ) {
	cgContext = cgCreateContext();
	checkForCgError("creating context");
	cgGLSetDebugMode(CG_TRUE);
	cgSetParameterSettingMode(cgContext, CG_DEFERRED_PARAMETER_SETTING);

	cgVertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
	cgGLSetOptimalOptions(cgVertexProfile);
	checkForCgError("selecting vertex profile");

	myVertexProgram =
		cgCreateProgramFromFile(
		cgContext,              /* Cg runtime context */
		CG_SOURCE,                /* Program in human-readable form */
		myVertexProgramFileName,  /* Name of file containing program */
		cgVertexProfile,        /* Profile: OpenGL ARB vertex program */
		myVertexProgramName,      /* Entry function name */
		NULL);                    /* No extra compiler options */
	checkForCgError("creating vertex program from file");
	cgGLLoadProgram(myVertexProgram);
	checkForCgError("loading vertex program");

#define GET_VERTEX_PARAM(name) \
	myCgVertexParam_##name = \
	cgGetNamedParameter(myVertexProgram, #name); \
	checkForCgError("could not get " #name " parameter");

	GET_VERTEX_PARAM(modelViewProj);
	GET_VERTEX_PARAM(modelView);
	GET_VERTEX_PARAM(modelViewH);

	cgFragmentProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
	cgGLSetOptimalOptions(cgFragmentProfile);
	checkForCgError("selecting fragment profile");

	myFragmentProgram =
		cgCreateProgramFromFile(
		cgContext,              /* Cg runtime context */
		CG_SOURCE,                /* Program in human-readable form */
		myFragmentProgramFileName,
		cgFragmentProfile,      /* Profile: latest fragment profile */
		myFragmentProgramName,    /* Entry function name */
		NULL); /* No extra compiler options */
	checkForCgError("creating fragment program from string");
	cgGLLoadProgram(myFragmentProgram);
	checkForCgError("loading fragment program");

#define GET_FRAGMENT_PARAM(name) \
	myCgFragmentParam_##name = \
	cgGetNamedParameter(myFragmentProgram, #name); \
	checkForCgError("could not get " #name " parameter");

	GET_FRAGMENT_PARAM(globalAmbient);
	GET_FRAGMENT_PARAM(lightColor);
	GET_FRAGMENT_PARAM(lightPosition);
	GET_FRAGMENT_PARAM(eyePosition);
	GET_FRAGMENT_PARAM(Ke);
	GET_FRAGMENT_PARAM(Ka);
	GET_FRAGMENT_PARAM(Kd);
	GET_FRAGMENT_PARAM(Ks);
	GET_FRAGMENT_PARAM(shininess);
	GET_FRAGMENT_PARAM(tex_env_mode);
	GET_FRAGMENT_PARAM(heightVal);

	myCgFragmentParam_decal =
    cgGetNamedParameter(myFragmentProgram, "decal");
    checkForCgError("getting decal parameter");

	myCgFragmentParam_decal1 =
    cgGetNamedParameter(myFragmentProgram, "decal1");
    checkForCgError("getting decal parameter");

	myCgFragmentParam_decal2 =
    cgGetNamedParameter(myFragmentProgram, "decal2");
    checkForCgError("getting decal parameter");

	/* Set light source color and position parameters once. */
	cgSetParameter3fv(myCgFragmentParam_globalAmbient, myGlobalAmbient);
	checkForCgError("setting myGlobalAmbient");
	cgSetParameter3fv(myCgFragmentParam_lightColor, myLightColor);
	checkForCgError("setting myLightColor");
	/*  Object-Space Light-Position */
	cgSetParameter3fv(myCgFragmentParam_lightPosition, lightPosition);
	checkForCgError("setting lightPosition");
	cgUpdateProgramParameters(myFragmentProgram);
}
// ********************* Cg Init End ********************* \\

class Avatar {

	//Sajat modellezesi koordinatarendszer tengelyei
	glm::vec3 Head;	//Roll
	glm::vec3 Pitch;	//Pitch
	glm::vec3 Up;		//Yaw
	//Vilagkoordinatarendszerben hasznalhato vektorok
	glm::vec3 position, velocity;
	//Orientacios tengelyekhez kepesti 
	//modellezesi koordinatarendszerben 
	//relativ elmozdulas, annak sebessege
	float Head_pos, Pitch_pos, Up_pos;
	float Head_vel, Pitch_vel, Up_vel;
	//Atmeneti Roll vektor, kiszamitva a kameraallasbol
	glm::vec3 newRoll;
	glm::vec3 newPitch;

public:
	Avatar(glm::vec3 spawn) {
		position = spawn;
		Head = glm::vec3(0.0f, 0.0f, 1.0f);
		Pitch = glm::vec3(-1.0f, 0.0f, 0.0f);
		Up = newRoll = velocity = glm::vec3(0.0f, 1.0f, 0.0f);
		Head_pos = Pitch_pos = Head_vel = Pitch_vel = Up_vel = 0.0f;
		//Head to:
		Up_pos = 135.0f;
	}

	void Respawn() {
		position = glm::vec3(1500.0f, 400.0f, 1300.0f);
		Head = glm::vec3(0.0f, 0.0f, 1.0f);
		Pitch = glm::vec3(-1.0f, 0.0f, 0.0f);
		Up = newRoll = velocity = glm::vec3(0.0f, 1.0f, 0.0f);
		Head_pos = Pitch_pos = Head_vel = Pitch_vel = Up_vel = 0.0f;
		//Head to:
		Up_pos = 135.0f;
	}

	void SetCameraTransform() {

		float lx = -cos((Up_pos-90)*PI_of_180);
		float lz = -sin((Up_pos-90)*PI_of_180);
		newRoll = glm::vec3(position.x+lx, -1.0, position.z+lz);
		newPitch = glm::vec3(position.x-cos((Up_pos-180.0f)*PI_of_180), -1.0, position.y-sin((Up_pos-180.0f)*PI_of_180));

		//brand new glm solution
		glMatrixMode(GL_PROJECTION);
		float aspect = (w_height == 0) ? 1.0 : w_width / w_height;
		glm::mat4 glm_Proj = glm::mat4(1.0f)*glm::perspective(90.0f, aspect, 0.01f, 12500.0f);
		glLoadMatrixf(&glm_Proj[0][0]);

		glMatrixMode(GL_MODELVIEW);
		eyeposition = glm::vec3(position.x+lx, position.y-0.6f, position.z+lz);
		glm::mat4 glm_LookAt = glm::mat4(1.0f)*glm::lookAt(eyeposition, glm::vec3(position.x, position.y, position.z), glm::vec3(0.0, -1.0, 0.0));
		glLoadMatrixf(&glm_LookAt[0][0]);

		//Passing mvp and eyepos to vertex shader
		cgGLSetStateMatrixParameter(myCgVertexParam_modelViewProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
		cgUpdateProgramParameters(myVertexProgram);
		GLfloat eyepos3f[3] = { eyeposition.x, eyeposition.y, eyeposition.z };
		//Passing Object-SPace Eye-Position to fragment shader
		cgSetParameter3fv(myCgFragmentParam_eyePosition, eyepos3f);
		checkForCgError("setting eyePosition");
		//Update shader params
		cgUpdateProgramParameters(myFragmentProgram);
		
	}

	void Move(const int control) {
		/*

		Controls stored in 'int' variables

		**(Pitch)
		**	cyclic->up			8 (KEY UP)
		**	cyclic->down		5 (KEY DOWN)
		**	cyclic->left		4 (KEY LEFT)
		**	cyclic->right		6 (KEY RIGHT)

		**(Turn)
		**	yaw->left			7 (KEY A)
		**	yaw->right			9 (KEY D)

		**(Accel)
		**	collective->up		0 (KEY W)
		**	collective->down	2 (KEY S)

		*/
		if(control==2) position.y += 1.0f;
		if(control==0) position.y -= 1.0f;

		if(control==4) {
			glm::vec3 P_V = Pitch;
			if(fabs(Head_vel) <= 7.0f) Head_vel -= 0.8;
			if(fabs(glm::length(velocity)) <= 1.0f) velocity += P_V*0.16f;
		}
		if(control==6) {
			glm::vec3 P_V = Pitch;
			if(fabs(Head_vel) <= 7.0f) Head_vel += 0.8;
			if(fabs(glm::length(velocity)) <= 1.0f) velocity -= P_V*0.16f;
		}

		if(control==9) Up_vel -= 2.6f;
		if(control==7) Up_vel += 2.6f;

		if(control==8) {
			glm::vec3 H_V = Head;
			H_V = glm::normalize(H_V);
			if(fabs(Pitch_vel) <= 5.0f) Pitch_vel -= 0.4f;
			if(fabs(glm::length(velocity)) <= 1.0f) velocity -= H_V;
		}
		if(control==5) {
			glm::vec3 H_V = Head;
			H_V = glm::normalize(H_V);
			if(fabs(Pitch_vel) <= 5.0f) Pitch_vel += 0.4f;
			if(fabs(glm::length(velocity)) <= 1.0f) velocity += H_V;
		}

	}

	void AnimateMe(float dt) {

		position += velocity*dt;

		if(abs(Pitch_pos) <= 8.0f) Pitch_pos += Pitch_vel*dt;
		if(abs(Head_pos) <= 8.0f) Head_pos += Head_vel*dt;
		Up_pos += Up_vel*dt;

		//Orriranyu vektor ujragondolasa
		Head.x = 1.0f;
		Head.y = newRoll.y;
		Head.z = newRoll.z;
		Pitch.x = newPitch.x;
		Pitch.y = newPitch.y;
		Pitch.z = newPitch.z;

		Head_vel*= 0.6f;
		Head_pos*= 0.8f;
		Up_vel*= 0.6f;
		Pitch_vel*= 0.6f;
		Pitch_pos*= 0.8f;
		velocity*=0.89f;
	}

	void DrawHeli(float dt) {

		glActiveTexture(GL_TEXTURE0);
		glEnable (GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, heliTexId);
			
		cgGLSetTextureParameter(myCgFragmentParam_decal, heliTexId);
		checkForCgError("setting decal 2D texture for heli");
		cgUpdateProgramParameters(myFragmentProgram);

		glPushMatrix();

		glm::mat4 glm_ModelMatrix;
		//Our heli's proper place & orientation
		glm_ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, position.z));
		glm_ModelMatrix = glm::rotate(glm_ModelMatrix, 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		glm_ModelMatrix = glm::rotate(glm_ModelMatrix, Pitch_pos, glm::vec3(Pitch.x, Pitch.y, Pitch.z));
		glm_ModelMatrix = glm::rotate(glm_ModelMatrix, Up_pos, glm::vec3(Up.x, Up.y, Up.z));
		glm_ModelMatrix = glm::rotate(glm_ModelMatrix, Head_pos, glm::vec3(Head.x, Head.y, Head.z));
		//Drawing helicopter body
		//Cheating on distances: making our vehicle smaller:)
		glm_ModelMatrix = glm::scale(glm_ModelMatrix, glm::vec3(0.6f,0.6f,0.6f));
		glMultMatrixf(&glm_ModelMatrix[0][0]);
		cgGLSetStateMatrixParameter(myCgVertexParam_modelViewProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
		cgSetMatrixParameterfr(myCgVertexParam_modelViewH, &glm_ModelMatrix[0][0]);
		cgSetMatrixParameterfr(myCgVertexParam_modelView, &glm_ModelMatrix[0][0]);
		cgUpdateProgramParameters(myVertexProgram);

		float zero[3] = {0.0,  0.0,  0.0};
		float heli_specular[3] = {0.9f, 0.9f, 0.9f};
		float heli_diff[3] = {0.7f, 0.7f, 0.7f};
		cgSetParameter3fv(myCgFragmentParam_Ke, zero);
		cgSetParameter3fv(myCgFragmentParam_Ka, zero);
		cgSetParameter3fv(myCgFragmentParam_Kd, heli_diff);
		cgSetParameter3fv(myCgFragmentParam_Ks, heli_specular);
		cgSetParameter1f(myCgFragmentParam_shininess, 10.0f);
		cgSetParameter1i(myCgFragmentParam_tex_env_mode, (int)0);		//ENV:Modulate
		cgUpdateProgramParameters(myFragmentProgram);

		glCallList(heli_model_list[0]);

		//Animation
		float period = (int)dt % 360;

		//Drawing helicopter main rotor
		glm_ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.35f, 0.25f));
		glMultMatrixf(&glm_ModelMatrix[0][0]);
		glPushMatrix();
			glm_ModelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.8f, 0.8f, 0.8f));
			glm_ModelMatrix = glm::rotate(glm_ModelMatrix, period, glm::vec3(0.0f, 1.0f, 0.0f));
			glMultMatrixf(&glm_ModelMatrix[0][0]);
			cgGLSetStateMatrixParameter(myCgVertexParam_modelViewProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
			cgUpdateProgramParameters(myVertexProgram);
			glCallList(heli_model_list[2]);
		glPopMatrix();

		//Drawing helicopter back rotor
		glm_ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.04f, -0.22f, -1.1f));
		glm_ModelMatrix = glm::scale(glm_ModelMatrix, glm::vec3(0.18, 0.18, 0.18));
		glMultMatrixf(&glm_ModelMatrix[0][0]);

		glPushMatrix();
			glm_ModelMatrix = glm::rotate(glm::mat4(1.0f), period, glm::vec3(1.0f, 0.0f, 0.0f));
			glMultMatrixf(&glm_ModelMatrix[0][0]);
			cgGLSetStateMatrixParameter(myCgVertexParam_modelViewProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
			cgUpdateProgramParameters(myVertexProgram);
			glCallList(heli_model_list[1]);
		glPopMatrix();

		glPopMatrix();
		cgSetParameter1i(myCgFragmentParam_tex_env_mode, (int)1);	//ENV:Replace
		glDisable(GL_TEXTURE_2D);
	}
	

};

void SetUpObjModels(int numModel, char **model_file, GLMmodel **models) {

	models[numModel] = glmReadOBJ(model_file[numModel]);
	scale[numModel] = glmUnitize(models[numModel]);
	glmFacetNormals(models[numModel]);
	glmVertexNormals(models[numModel], smoothing_angle);

	//Clean Display Lists
	if (heli_model_list[numModel]) glDeleteLists(heli_model_list[numModel], 1);

	/*
	*	glm Rendering options:
	*             GLM_FLAT     -  render with facet normals
	*             GLM_SMOOTH   -  render with vertex normals
	*             GLM_TEXTURE  -  render with texture coords
	*             GLM_COLOR    -  render with colors (color material)
	*             GLM_MATERIAL -  render with materials
	*             GLM_COLOR and GLM_MATERIAL should not both be specified.
	*             GLM_FLAT and GLM_SMOOTH should not both be specified.
	*/

	heli_model_list[numModel] = glmList(models[numModel], GLM_SMOOTH | GLM_TEXTURE );

}

void LoadObjModels() {

	GLMmodel* models[NUMOFHELIPARTS];
	char* model_file[NUMOFHELIPARTS];

	for(int i = 0; i < NUMOFHELIPARTS; i++) {
		models[i] = NULL;
		model_file[i] = NULL;
	}
	//Enter model paths manually
	model_file[0] = "GameLib/apache/body_0_newmat.obj";
	model_file[1] = "GameLib/apache/backrotor.obj";
	model_file[2] = "GameLib/apache/mainrotor.obj";

	//Function to load models one by one

	for(int i = 0; i < NUMOFHELIPARTS; i++) {
		SetUpObjModels(i, model_file, models);
	}

}

Avatar Myself(glm::vec3(1500.0f, 200.0f, 1300.0f));
// SkyBox
SkyBox Sky = SkyBox(512);

void onDisplay() {

	//Background
	glClearColor(0.15f, 0.58f, 0.74f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//SkyBox Drawing
	glPushMatrix();
		
		//The sky is always behind everything else:)
	glDisable(GL_DEPTH_TEST);
		//A never ending sky: translating with our  position
		glm::mat4 glm_ModelMatrixs = glm::translate(glm::mat4(1.0f), glm::vec3(eyeposition.x-3072.0f, 5072.0f, eyeposition.z-3072.0f));
		glm_ModelMatrixs = glm::scale(glm_ModelMatrixs, glm::vec3(6.0f, 6.0f, 6.0f));
		glMultMatrixf(&glm_ModelMatrixs[0][0]);
		cgGLSetStateMatrixParameter(myCgVertexParam_modelViewProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
		checkForCgError("setting mvp for skybox");
		//Do not set modelView mtx for fog generation, cause it is too far.. its modelview is the same as the last uploaded 
		cgUpdateProgramParameters(myVertexProgram);
				//materials
				float Emissive[3] = {0.4,  0.35,  0.35};
				float mydiff1[3] = {1.0,  0.9,  0.9};
				float myamb1[3] = {1.0,  0.9,  0.9};
				float myspec1[3] = {0.0,  0.0,  0.0};
				cgSetParameter3fv(myCgFragmentParam_Ke, Emissive);
				cgSetParameter3fv(myCgFragmentParam_Ka, myamb1);
				cgSetParameter3fv(myCgFragmentParam_Kd, mydiff1);
				cgSetParameter3fv(myCgFragmentParam_Ks, myspec1);
				cgSetParameter1f(myCgFragmentParam_shininess, 0.0f);
				cgSetParameter1i(myCgFragmentParam_tex_env_mode, (int)1);	//ENV:Replace
				cgUpdateProgramParameters(myFragmentProgram);
		glCallList(other_model_list[0]);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);

	//Heightmap Drawing
		float HeightMapScale = 5.0f;
	glPushMatrix();
				//mvp
				glm::mat4 glm_ModelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3((float)HeightMapScale, (float)HeightMapScale*0.6, (float)HeightMapScale));
				glMultMatrixf(&glm_ModelMatrix[0][0]);
				cgGLSetStateMatrixParameter(myCgVertexParam_modelViewProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
				checkForCgError("setting mvp for heightmap");
				cgGLSetStateMatrixParameter(myCgVertexParam_modelView, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_IDENTITY);
				checkForCgError("setting mv for heightmap");
				cgUpdateProgramParameters(myVertexProgram);
				//materials
				cgSetParameter3fv(myCgFragmentParam_Ke, Emissive);
				cgSetParameter3fv(myCgFragmentParam_Ka, myamb1);
				cgSetParameter3fv(myCgFragmentParam_Kd, mydiff1);
				cgSetParameter3fv(myCgFragmentParam_Ks, myspec1);
				cgSetParameter1f(myCgFragmentParam_shininess, 0.0f);
				cgSetParameter1i(myCgFragmentParam_tex_env_mode, (int)1);	//ENV:Replace
				cgUpdateProgramParameters(myFragmentProgram);	
				//draw
				DrawHeightMap(heightMapTexId);
				//glCallList(other_model_list[1]);
	glPopMatrix();

	Myself.SetCameraTransform();
	Myself.DrawHeli(time);

	glutSwapBuffers();

}



void onInitialization( ) {

	InitCg();

	  cgGLBindProgram(myVertexProgram);
	  checkForCgError("binding vertex program");

	  cgGLEnableProfile(cgVertexProfile);
	  checkForCgError("enabling vertex profile");

	  cgGLBindProgram(myFragmentProgram);
	  checkForCgError("binding fragment program");

	  cgGLEnableProfile(cgFragmentProfile);
	  checkForCgError("enabling fragment profile");

	  cgGLEnableTextureParameter(myCgFragmentParam_decal);
	  checkForCgError("enable decal texture");

	  cgGLEnableTextureParameter(myCgFragmentParam_decal1);
	  checkForCgError("enable decal texture1");

	  cgGLEnableTextureParameter(myCgFragmentParam_decal2);
	  checkForCgError("enable decal texture2");

	//Trivial backface culling
	glEnable(GL_CULL_FACE);
	//Z-buffer
	glEnable(GL_DEPTH_TEST);
	//Indicates the quality of color, texture coordinate, and fog coordinate interpolation. 
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//Loading Heli model parts
	LoadObjModels();

	heliTexId = SOIL_LoadTexDirectly("GameLib/028.jpg", SOIL_FLAG_TEXTURE_REPEATS);

	heightMapTexId[0] = SOIL_LoadTexDirectly("GameLib/snow.bmp", SOIL_FLAG_TEXTURE_REPEATS);
	heightMapTexId[1] = SOIL_LoadTexDirectly("GameLib/dirt.jpg", SOIL_FLAG_TEXTURE_REPEATS);
	heightMapTexId[2] = SOIL_LoadTexDirectly("GameLib/grass.png", SOIL_FLAG_TEXTURE_REPEATS);

	skyTexId[0] = SOIL_LoadTexDirectly("GameLib/miramar/3.png", NULL);
	skyTexId[1] = SOIL_LoadTexDirectly("GameLib/miramar/2.png", NULL);
	skyTexId[2] = SOIL_LoadTexDirectly("GameLib/miramar/1.png", NULL);
	skyTexId[3] = SOIL_LoadTexDirectly("GameLib/miramar/4.png", NULL);
	skyTexId[4] = SOIL_LoadTexDirectly("GameLib/miramar/felso.png", NULL);
	skyTexId[5] = SOIL_LoadTexDirectly("GameLib/miramar/also.png", NULL);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, heightMapTexId[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, heightMapTexId[1]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, heightMapTexId[2]);

	other_model_list[0] = Sky.SkyBoxList(skyTexId);
	//other_model_list[1] = DrawHeightMap(heightMapTexId);
}

void onMouse(int button, int state, int x, int y) {

}

void menu(int item){

}

void onIdle( ) {
	static long old_time = 0;
	time = glutGet(GLUT_ELAPSED_TIME);
	long time = glutGet(GLUT_ELAPSED_TIME); // time elapsed since starting this program in msec
	float dt = 0.01 * (time - old_time);
	Myself.AnimateMe(dt);
	old_time = time;
	if(IsKeyPressed(KEY_A)) {
		Myself.Move(7);
	}
	if(IsKeyPressed(KEY_D)) {
		Myself.Move(9);
	}
	if(IsKeyPressed(KEY_W)) {
		Myself.Move(0);
	}
	if(IsKeyPressed(KEY_S)) {
		Myself.Move(2);
	}
	if(IsKeyPressed(KEY_UP)) {
		Myself.Move(8);
	}
	if(IsKeyPressed(KEY_DOWN)) {
		Myself.Move(5);
	}
	if(IsKeyPressed(KEY_LEFT)) {
		Myself.Move(4);
	}
	if(IsKeyPressed(KEY_RIGHT)) {
		Myself.Move(6);
	}
	if(IsKeyPressed(KEY_SPACE)) {
		Myself.Respawn();
	}
	if(IsKeyPressed(27)) {
		exit(EXIT_SUCCESS);
		//Free resources allocated for a single program
		cgDestroyProgram(myVertexProgram);										
		cgDestroyProgram(myFragmentProgram);
		cgGLDisableProfile(cgVertexProfile);
		checkForCgError("disabling vertex profile");
		cgGLDisableProfile(cgFragmentProfile);
		checkForCgError("disabling fragment profile");

		cgGLDisableTextureParameter(myCgFragmentParam_decal);
	    checkForCgError("disabling decal texture");

		cgDestroyContext(cgContext);										//Free all the resources allocated for a context
	}
	glutPostRedisplay();
}

int main(int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitWindowSize(w_width, w_height);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow("Helis");

	GLenum glew_err = glewInit();

	fprintf(stdout, "Using OpenGL %s\n", (glGetString(GL_VERSION)));
	if (GLEW_OK != glew_err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_err));
	}
	fprintf(stdout, "Using GLEW %s\n", glewGetString(GLEW_VERSION));
	

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	onInitialization();

	glutIgnoreKeyRepeat(0);
	glutDisplayFunc(onDisplay);
	glutMouseFunc(onMouse);
	glutIdleFunc(onIdle);
	InitKeyboard();

	glutCreateMenu(menu);
	glutAddMenuEntry("*** Heli Menu ***", ' ');
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();

	return 0;
}

