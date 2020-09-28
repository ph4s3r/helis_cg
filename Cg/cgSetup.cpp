
#include <stdio.h>
#include "cgSetup.h"

CGparameter myCgVertexParam_modelViewProj,
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

CGprogram myVertexProgram, myFragmentProgram;

//****************************
//Cg Error Check function Begin
//return false on error
bool checkForCgError(const char *situation)
{
	CGerror error;
	const char *string = cgGetLastErrorString(&error);

	if (error != CG_NO_ERROR) {
		printf("%s %s\n",
			situation, string);
		if (error == CG_COMPILER_ERROR) {
			printf("%s\n", cgGetLastListing(cgContext));
			return false;
		}
		else
			return false;
	}
	return true;
}
//Cg Error Check function End
//****************************


//Loading mvp to vertex shader from included files cause invalid parameter handle errors,
//so we need a function in Helis.obj
//Returns false on error
bool injectMVP(void) 
{
	cgGLSetStateMatrixParameter(myCgVertexParam_modelViewProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
	cgUpdateProgramParameters(myVertexProgram);
	if(checkForCgError("problem in setting mvp: ")) return false;

	return true;
}

