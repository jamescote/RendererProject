#pragma once

/* INCLUDES */
#include "stdafx.h"
#include "Shader.h"

// Enum for Shaders
enum eShaderType
{
	LIGHT_SHDR = 0,
	MESH_SHDR,
	PLANE_SHDR,
	MAX_SHDRS
};

// Class: Shader Manager
// Purpose: Manages all the Shaders used by every Assignment.  Manages Uniform variable
//			manipulation and properly initializes and destroys created Shaders.
// Current Issues: Uniform variables hard-coded.
// TODO: (Low Priority) Scan through all shader code for uniform variables and set up
//						management of those variables dynamically.  Too much work...
// Written by: James Coté
class ShaderManager
{
public:
	static ShaderManager* getInstance();
	~ShaderManager();

	// Initialize Shaders
	bool initializeShaders();

	// Get the specified program for using shaders for rendering
	GLuint getProgram(eShaderType eType) { return m_pShader[eType].getProgram(); }

	// Setup Buffers and Arrays in GPU
	GLuint genVertexBuffer( GLuint iVertArray,
							GLuint iSpecifiedIndex, 
							GLint iChunkSize, 
							const void* pData, GLsizeiptr pSize );
	GLuint genIndicesBuffer( GLuint iVertArray, 
							 const void* pData, GLsizeiptr pSize );

	// Shader Uniform Variable Manipulation 
	void setUnifromMatrix4x4( eShaderType eType, string sVarName, const mat4* pResultingMatrix );
	void setUniformVec3( eShaderType eType, string sVarName, const glm::vec3* pLightPos );
	void setUniformFloat(eShaderType eType, string sVarName, float fVal);
	
	// For resetting all shaders to default values
	void resetAllShaders();

private:
	// Singleton Implementation
	ShaderManager();
	static ShaderManager* m_pInstance;

	// Should only be initialized once.
	bool m_bInitialized;

	// Shader Variables
	Shader m_pShader[MAX_SHDRS];

	// Enum for different Uniform Shader Variables.  Hard-coded for now
	// TODO: load from file. Low Priority
	enum eShaderVars
	{
		MODEL_VIEW = 0,
		PROJECTION,
		LIGHT_POS,
		MAX_VARS
	};
};

