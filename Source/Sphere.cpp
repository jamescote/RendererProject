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
				long lID, const string* sTexName )
	: Object3D( pPos, lID, sTexName )
{
	ShaderManager* pShdrMngr = ShaderManager::getInstance();

	m_fRadius = fRadius;
	generateMesh();
	calculateUVs();

	// Setup Mesh information in GPU
	glGenVertexArrays( 1, &m_iVertexArray );

	m_iVertexBuffer = pShdrMngr->genVertexBuffer( m_iVertexArray, 0, 3, m_pVertices.data(), m_pVertices.size() * sizeof( vec3 ) );
	m_iNormalBuffer = pShdrMngr->genVertexBuffer( m_iVertexArray, 1, 3, m_pNormals.data(), m_pNormals.size() * sizeof( vec3 ) );
	m_iTextureBuffer = pShdrMngr->genVertexBuffer( m_iVertexArray, 2, 2, m_pUVs.data(), m_pUVs.size() * sizeof( vec2 ) );
}

// Destructor
Sphere::~Sphere()
{
	// Delete Vertex Array and Buffers
	glDeleteBuffers( 1, &m_iVertexBuffer );
	glDeleteBuffers( 1, &m_iNormalBuffer );
	glDeleteBuffers( 1, &m_iTextureBuffer );
	glDeleteVertexArrays( 1, &m_iVertexArray );
}

// Draw the Sphere
void Sphere::draw()
{
	glBindVertexArray( m_iVertexArray );

	if ( NULL != m_pTexture )
		m_pTexture->bindTexture( eShaderType::MESH_SHDR, "mySampler" );

	glUseProgram( ShaderManager::getInstance()->getProgram( eShaderType::MESH_SHDR ) );

	glPointSize( 5.f );
	glDrawArrays( GL_TRIANGLE_STRIP, 0, m_pVertices.size() );
	glPointSize( 1.f );

	if ( NULL != m_pTexture )
		m_pTexture->unbindTexture();

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
	float fCurrParallel, fNextParallel, fCurrMeridian;

	for ( unsigned int p = 0; p < MAX_PHI_CUTS; ++p )
	{
		fCurrParallel = PI * p / MAX_PHI_CUTS;
		fNextParallel = PI * (p + 1) / MAX_PHI_CUTS;

		for ( unsigned int m = 0; m < MAX_THETA_CUTS; ++m )
		{
			fCurrMeridian = (2.f * PI * m) / MAX_THETA_CUTS;
			addCarteseanPoint( fCurrParallel, fCurrMeridian );
			addCarteseanPoint( fNextParallel, fCurrMeridian );
		}
	}
}

// Generates a Caresean point and normal for that point based on a given spherical coord
void Sphere::addCarteseanPoint( float fPhi, float fTheta )
{
	float fPhi_Rads = fPhi;// *PI / 180.f;
	float fTheta_Rads = fTheta;// *PI / 180.f;
	vec3 pPoint;

	pPoint.x = m_fRadius * sin( fPhi_Rads );
	pPoint.y = pPoint.x * sin( fTheta_Rads );
	pPoint.x *= cos( fTheta_Rads );
	pPoint.z = m_fRadius * cos( fPhi_Rads );

	mat4 mLookAtTranslation = translate( mat4( 1.f ), m_pPosition + vec3( 0.0, m_fRadius, 0.0 ) );
	vec4 mTranslatedPos = mLookAtTranslation * vec4( pPoint, 1.f );
	m_pVertices.push_back( vec3( mTranslatedPos ) );
	m_pNormals.push_back( normalize( vec3( mTranslatedPos ) - m_pPosition ) );
}

void Sphere::calculateUVs()
{
	vec2 pUV;
	vec3 pToOrigin;

	for ( vector<vec3>::iterator pIter = m_pVertices.begin();
		 pIter != m_pVertices.end();
		 ++pIter )
	{
		pToOrigin = normalize( m_pPosition - (*pIter) );

		pUV.x = 0.5f + (atan2( pToOrigin.z, pToOrigin.x ) / 2.f * PI);
		pUV.y = 0.5f - asin( pToOrigin.y ) / PI;

		m_pUVs.push_back( pUV );
	}
}
