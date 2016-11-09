#pragma once
#include "Object3D.h"
#include "TriMesh.h"

//////////////////////////////////////////////////////////////////
// Name: Mesh.h
// Class: Drawable 3D Object that loads a Mesh generated from an external source.
//			Uses the TriMesh interface to load the mesh
// Written by: James Cote
//////////////////////////////////
class Mesh :
	public Object3D
{
public:
	~Mesh();

	// Overloaded Virtual Functions from Object3D
	void draw();
	string getType() { return "Mesh"; }
	string getDebugOutput();

private:
	// Private Constructor and Copy Constructor to restrict usage to Object_Factory
	Mesh( const glm::vec3* pPosition, const string* sFileName, long lID, const string* sTexName );
	Mesh( const Mesh* pCopy );

	// read the Mesh using the TriMesh functionality
	void readMesh( string sFileName );

	// Indices for Faces of Mesh and Additional Buffer Addresses on the GPU for
	//	Indices and Normals
	vector<unsigned int> m_pIndices;
	GLuint m_iNormalBuffer, m_iIndicesBuffer;

	// Mesh Object contains vertices, normals and indices of Mesh
	trimesh::TriMesh* m_pMesh;

	// Inherited from Parent
	void calculateUVs();

	// Friend Class: Object_Factory to create Meshes.
	friend class Object_Factory;
};

