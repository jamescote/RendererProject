#pragma once

#include "stdafx.h"
#include "Object3D.h"
#include "Light.h"

// Solely Generates Objects and assigns IDs to them.
class Object_Factory
{
public:
	static Object_Factory* getInstance();
	~Object_Factory();

	// Creation Functions
	Object3D* createSphere( const glm::vec3* pStartPos,
							float fRadius );
	Object3D* createPlane( const glm::vec3* pPosition,
						   const glm::vec3* pPos1,
						   const glm::vec3* pPos2,
						   const glm::vec3* pPos3,
						   const glm::vec3* pPos4 );
	Object3D* createTriangle( const glm::vec3* pPosition,
							  const glm::vec3* pPos1,
							  const glm::vec3* pPos2,
							  const glm::vec3* pPos3 );
	Light* createLight( const glm::vec3* pPos,
						const glm::vec3* pColor );
	Object3D* createMesh( const glm::vec3* pPos,
						  string sLocation );

	void loadFromFile( string sFileName );

private:
	// Singleton Implementation
	Object_Factory();
	static Object_Factory* m_pInstance;

	long m_lNextID;
	long getNewID() { return ++m_lNextID; }
};

