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
	Object3D* createSphere( vector< string > sData, int iLength );
	Object3D* createPlane( vector< string > sData, int iLength );
	Object3D* createTriangle( vector< string > sData, int iLength );
	Light* createLight( vector< string > sData, int iLength );
	Object3D* createMesh( vector< string > sData, int iLength );
	Object3D* createMesh( const vec3* pPos, const string* sLocation, const string* sTexLocation );

	void loadFromFile( string sFileName );

private:
	// Singleton Implementation
	Object_Factory();
	Object_Factory( Object_Factory* pCopy );
	static Object_Factory* m_pInstance;

	long m_lNextID;
	long getNewID() { return ++m_lNextID; }
	void outputError( const string* sName, vector<string> sData );
};

