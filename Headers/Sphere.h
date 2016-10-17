#pragma once
#include "Object3D.h"

class Sphere :
	public Object3D
{
public:
	~Sphere();

	// Overridden draw
	void draw();

	// Overridden Type Output
	string getType() { return "Sphere"; }

	// Overridden Debug Output
	string getDebugOutput();
	
private:
	// Constructor for Spheres
	Sphere( const vec3* pPosition, float fRadius, long lID );
	Sphere( const Sphere* pNewSphere );	// Protected Copy Constructor.
	void generateMesh();
	void addCarteseanPoint( float fPhi, float fTheta );

	// List of Normals for our mesh
	vector<vec3> m_pNormals;

	// Buffer information for Normals
	GLuint m_iNormalBuffer;

	// Private Sphere Variables
	float m_fRadius;

	// Declare Object_Factory as a Friend to be sole progenitor of Spheres
	friend class Object_Factory;
};

