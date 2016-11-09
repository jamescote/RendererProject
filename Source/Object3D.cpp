#include "Object3D.h"
#include "EnvironmentManager.h"

// Default Constructor.
// Initializes to the center of the world and adds
//		itself to the EnvironmentManager
Object3D::Object3D( const vec3* pPosition, long lID, const string* sTexName )
	: Object( pPosition, lID, sTexName )
{
	EnvironmentManager::getInstance()->addObject( this );
}

Object3D::Object3D( const Object3D* pCopy ) : Object( pCopy )
{
	m_pVertices		= pCopy->m_pVertices;
	m_iVertexArray	= pCopy->m_iVertexArray;
	m_iVertexBuffer = pCopy->m_iVertexBuffer;
}

Object3D::~Object3D()
{
	// Nothing to Destruct.
}
