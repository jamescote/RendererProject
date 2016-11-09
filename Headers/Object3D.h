#pragma once

#include "stdafx.h"
#include "ShaderManager.h"
#include "Object.h"

#define MAX_SPECULAR_EXP 1000.f
#define REFLECTIVITY_MOD 0.0001f

class Object3D : public Object
{
public:
	~Object3D();

	// Pure Virtual Intersection Function
	// All inherited classes must implement their own
	//		logic for this function.
	virtual void draw() = 0;
	virtual string getType() = 0;
	virtual string getDebugOutput() = 0;

protected:
	// Protected Variables
	vector<vec3> m_pVertices;

	GLuint m_iVertexArray, m_iVertexBuffer, m_iTextureBuffer;

	vector<vec2> m_pUVs;

	virtual void calculateUVs() = 0;

	// Constructors only accessable by Object Factory.
	Object3D( const vec3* pPosition, long lID, const string* sTexName);
	Object3D( const Object3D* pCopy );
};

