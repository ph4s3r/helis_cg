
#pragma once
//Cg
#include "Cg\cg.h"
#include "Cg\cgGL.h"

//******************************
//Cg Environment Vars Init Begin

//needs to be statics in order to be reachable by glm.cpp's changed material Cg params passing
static CGcontext cgContext;
extern CGprogram myVertexProgram, myFragmentProgram;
static CGprofile cgVertexProfile, cgFragmentProfile;
extern CGparameter myCgVertexParam_modelViewProj,
			myCgVertexParam_modelView,
			myCgVertexParam_modelViewH,
			myCgFragmentParam_globalAmbient,
			myCgFragmentParam_lightColor,
			myCgFragmentParam_lightPosition,
			myCgFragmentParam_eyePosition,
			myCgFragmentParam_Ke,
			myCgFragmentParam_Ka,
			myCgFragmentParam_Kd,
			myCgFragmentParam_Ks,
			myCgFragmentParam_shininess,
			myCgFragmentParam_decal,
			myCgFragmentParam_decal1,
			myCgFragmentParam_decal2,
			myCgFragmentParam_tex_env_mode,
			myCgFragmentParam_heightVal;
static const char *myVertexProgramFileName = "Cg/TPNF.cg",
	*myVertexProgramName = "special_vertex",
	*myFragmentProgramFileName = "Cg/TPNF.cg",
	*myFragmentProgramName = "special_fragment";

static const float myGlobalAmbient[3] = { 0.05f, 0.58f, 0.74f };  /* Dim */
static const float myLightColor[3] = { 0.95f, 0.95f, 0.95f };     /* White */
static const float lightPosition[4] = { 1500.0f, -75.0f, -3000.0f, 0.0f };	//4. coord 0 -> directed lightsource	(1 -> spotlight)

//Cg Environment Vars Init End
//****************************

bool checkForCgError(const char *situation);

extern bool injectMVP(void);

//void InitCg(void);
