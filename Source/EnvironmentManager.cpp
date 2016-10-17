#include "EnvironmentManager.h"
#include "Object_Factory.h"

#define INTERSECTION_EPSILON 1e-4	// Minimum intersect distance (so we don't intersect with ourselves)
#define MAX_REFLECTIONS	800

// Initialize Static Instance Variable
EnvironmentManager* EnvironmentManager::m_pInstance = NULL;

EnvironmentManager::EnvironmentManager()
{
	// Constructor.
}

// Gets the instance of the environment manager.
EnvironmentManager* EnvironmentManager::getInstance()
{
	if ( NULL == m_pInstance )
		m_pInstance = new EnvironmentManager();
	return m_pInstance;
}

// Destructor.
EnvironmentManager::~EnvironmentManager()
{
	purgeEnvironment();
}

void EnvironmentManager::initializeEnvironment(string sFileName)
{
	Object_Factory* pObjFctry = Object_Factory::getInstance();

	purgeEnvironment();
	pObjFctry->loadFromFile(sFileName);
}

// Adds object to back of List
void EnvironmentManager::addObject( Object3D* pNewObject )
{
	m_pObjects.push_back( pNewObject );
}

// Adds a Light to back of List
void EnvironmentManager::addLight( Light* pNewLight )
{
	m_pLights.push_back( pNewLight );
}

// Remove Object from List with given ID
void EnvironmentManager::killObject( long lID )
{
	unsigned int i = 0;

	// Iterate to find Object
	while ( i < m_pObjects.size() && NULL != m_pObjects[i] && lID != m_pObjects[i]->ID() )
		++i;

	// Delete Object and remove it from list.
	if ( i < m_pObjects.size() )
	{
		swap( m_pObjects[i], m_pObjects.back() );
		delete m_pObjects.back();
		m_pObjects.pop_back();
	}
}

// Remove Object from List with given ID
void EnvironmentManager::killLight( long lID )
{
	unsigned int i = 0;

	// Iterate to find Object
	while ( i < m_pLights.size() && NULL != m_pLights[i] && lID != m_pLights[i]->ID() )
		++i;

	// Delete Object and remove it from list.
	if ( i < m_pLights.size() )
	{
		swap( m_pLights[i], m_pLights.back() );
		delete m_pLights.back();
		m_pLights.pop_back();
	}
}

// Outputs all the objects in the environment for debugging.
void EnvironmentManager::debugEnvironment()
{
	cout << "Environment:" << endl;
	for ( vector<Object3D*>::iterator pIter = m_pObjects.begin();
		  pIter != m_pObjects.end();
		  ++pIter )
	{
		cout << "\t" << (*pIter)->getDebugOutput() << endl;
	}

	for ( vector<Light*>::iterator pIter = m_pLights.begin();
		  pIter != m_pLights.end();
		  ++pIter )
	{
		cout << "\t" << (*pIter)->getDebugOutput() << endl;
	}

	cout << endl;
}

// Clears out the entire environment
void EnvironmentManager::purgeEnvironment()
{
	// Clean Up objects
	for ( vector<Object3D*>::iterator pIter = m_pObjects.begin();
		  pIter != m_pObjects.end();
		  ++pIter )
	{
		if ( NULL != (*pIter) )
		{
			delete (*pIter);
		}
	}

	// Clean up Lights
	for ( vector<Light*>::iterator pIter = m_pLights.begin();
		  pIter != m_pLights.end();
		  ++pIter )
	{
		if ( NULL != (*pIter) )
		{
			delete (*pIter);
		}
	}

	// Clear the array of Dangling pointers
	m_pObjects.clear();
	m_pLights.clear();
}

void EnvironmentManager::renderEnvironment()
{
	ShaderManager* pShdrMngr = ShaderManager::getInstance();

	for (vector<Light*>::iterator pLightIter = m_pLights.begin();
		pLightIter != m_pLights.end();
		++pLightIter)
	{
		pShdrMngr->setUniformVec3(eShaderType::LIGHT_SHDR, "lightPosition", &(*pLightIter)->getPosition() );
		pShdrMngr->setUniformVec3(eShaderType::MESH_SHDR, "lightPosition",  &(*pLightIter)->getPosition() );
		pShdrMngr->setUniformVec3(eShaderType::PLANE_SHDR, "lightPosition", &(*pLightIter)->getPosition() );

		(*pLightIter)->draw();
		for (vector<Object3D*>::iterator pIter = m_pObjects.begin();
			pIter != m_pObjects.end();
			++pIter)
		{
			if (NULL != (*pIter))
				(*pIter)->draw();
		}
	}

	m_pLights[0]->draw();
}

/*********************************************************************************\
 * Light Manipulation                                                            *
\*********************************************************************************/
void EnvironmentManager::moveLight(vec3 pMoveVec)
{
	m_pLights[0]->move(pMoveVec);
}