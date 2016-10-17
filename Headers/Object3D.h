#pragma once

#include "stdafx.h"
#include "ShaderManager.h"
#define MAX_SPECULAR_EXP 1000.f
#define REFLECTIVITY_MOD 0.0001f

class Object3D
{
public:
	~Object3D();

	// Pure Virtual Intersection Function
	// All inherited classes must implement their own
	//		logic for this function.
	virtual void draw() = 0;
	virtual string getType() = 0;
	virtual string getDebugOutput() = 0;

	// Getters/Setters
	long ID() const { return m_lID; }								// Get ID for the object

protected:
	// Protected Variables
	vector<vec3> m_pVertices;
	vec3 m_pPosition;
	long m_lID;

	GLuint m_iVertexArray, m_iVertexBuffer;

	// Constructors only accessable by Object Factory.
	Object3D( const vec3* pPosition, long lID );
};

