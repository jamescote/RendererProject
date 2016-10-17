#include "Object3D.h"
#include "EnvironmentManager.h"

// Default Constructor.
// Initializes to the center of the world and adds
//		itself to the EnvironmentManager
Object3D::Object3D( const vec3* pPosition, long lID )
{
	m_lID				= lID;
	m_pPosition			= *pPosition;
	EnvironmentManager::getInstance()->addObject( this );
}

Object3D::~Object3D()
{
	// Nothing to Destruct.
}
