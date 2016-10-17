#include "Sphere.h"

#define PI 3.14159265f
#define SLICE_SIZE 10.f
#define MAX_THETA_DEGS 360.f
#define MAX_PHI_DEGS 180.f
#define MAX_THETA_CUTS (int)(MAX_THETA_DEGS / SLICE_SIZE)
#define MAX_PHI_CUTS (int)(MAX_PHI_DEGS / SLICE_SIZE)

// Constructor for Spheres
Sphere::Sphere( const vec3* pPos,
				float fRadius,
				long lID ) 
	: Object3D( pPos, lID )
{
	ShaderManager* pShdrMngr = ShaderManager::getInstance();

	m_fRadius = fRadius;
	generateMesh();

	// Setup Mesh information in GPU
	glGenVertexArrays( 1, &m_iVertexArray );

	m_iVertexBuffer = pShdrMngr->genVertexBuffer( m_iVertexArray, 0, 3, m_pVertices.data(), m_pVertices.size() * sizeof( vec3 ) );
	m_iNormalBuffer = pShdrMngr->genVertexBuffer( m_iVertexArray, 1, 3, m_pNormals.data(), m_pNormals.size() * sizeof( vec3 ) );
}

// Destructor
Sphere::~Sphere()
{
	// Delete Vertex Array and Buffers
	glDeleteBuffers( 1, &m_iVertexBuffer );
	glDeleteBuffers( 1, &m_iNormalBuffer );
	glDeleteVertexArrays( 1, &m_iVertexArray );
}

// Draw the Sphere
void Sphere::draw()
{
	glBindVertexArray( m_iVertexArray );

	glUseProgram( ShaderManager::getInstance()->getProgram( eShaderType::MESH_SHDR ) );

	glDrawArrays( GL_TRIANGLE_STRIP, 0, m_pVertices.size() );

	glUseProgram(0);
	glBindVertexArray( 0 );
}

// Overridden Debug Output
string Sphere::getDebugOutput()
{
	string sOutput = "";

	sOutput += getType();
	sOutput += "/ID:" + to_string( m_lID );
	sOutput += "/Position:" + to_string( m_pPosition );
	sOutput += "/Radius:" + to_string( m_fRadius );

	return sOutput;
}

// calculates a mesh of vertices and UV coords for the sphere
void Sphere::generateMesh()
{
	float fCurrParallel, fCurrMeridian;

	for ( unsigned int p = 0; p < MAX_PHI_CUTS; ++p )
	{
		fCurrParallel = PI * (p + 1) / MAX_PHI_CUTS;

		for ( unsigned int m = 0; m < MAX_THETA_CUTS; ++m )
		{
			fCurrMeridian = 2.0f * PI * m / MAX_THETA_CUTS;
			addCarteseanPoint( fCurrParallel, fCurrMeridian );
		}
	}
}

// Generates a Caresean point and normal for that point based on a given spherical coord
void Sphere::addCarteseanPoint( float fPhi, float fTheta )
{
	float fSinPhi = m_fRadius * sin( fPhi );
	vec3 pNewPoint( fSinPhi* cos( fTheta ),		// rsin(Phi)cos(Theta)
					fSinPhi * sin( fTheta ),	// rsin(Phi)sin(Theta)
					m_fRadius * cos( fPhi ) );	// rcos(Phi)
	vec3 pNewNormal;
	mat4 pTranslationMatrix = translate( m_pPosition );	// translate point to position of sphere
	
	// Translate Point and add it to list of vertices
	pNewPoint = vec3( vec4(pNewPoint, 1.0f) * pTranslationMatrix );
	pNewNormal = normalize(pNewPoint - m_pPosition);

	// Store calculated vertices
	m_pVertices.push_back( pNewPoint );
	m_pNormals.push_back( pNewNormal );
}