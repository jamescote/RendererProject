#include "Plane.h"

#define NUM_CORNERS 4

// Constructor
Plane::Plane( const vec3* pPosition,
			  const vector<glm::vec3>* pCorners, 
			  long lID ) : Object3D( pPosition, lID )
{
	if ( NUM_CORNERS != pCorners->size() )	// Set up a default Plane
	{
		m_pVertices.push_back( glm::vec3( -1.f, 0.f, -1.f ) );
		m_pVertices.push_back( glm::vec3( -1.f, 0.f, 1.f ) );
		m_pVertices.push_back( glm::vec3( 1.f, 0.f, -1.f ) );
		m_pVertices.push_back( glm::vec3( 1.f, 0.f, 1.f ) );
	}
	else
		m_pVertices.insert( m_pVertices.begin(), pCorners->begin(), pCorners->end() );		

	glGenVertexArrays( 1, &m_iVertexArray );

	m_iVertexBuffer = ShaderManager::getInstance()->genVertexBuffer( m_iVertexArray,
									    							 0, 3, m_pVertices.data(), 
																	 m_pVertices.size() * sizeof( glm::vec3 ) );
}

// Destructor
Plane::~Plane()
{
	glDeleteBuffers( 1, &m_iVertexBuffer );
	glDeleteVertexArrays( 1, &m_iVertexArray );
}

// Overridden Debug Output
string Plane::getDebugOutput()
{
	string sOutput = "";

	sOutput += getType();
	sOutput += "/ID:" + to_string( m_lID );
	sOutput += "/Normal:" + glm::to_string( m_pNormal );

	return sOutput;
}

// Setup OpenGl to draw the Plane using the Plane Shader.
void Plane::draw()
{
	glBindVertexArray( m_iVertexArray );

	glUseProgram( ShaderManager::getInstance()->getProgram( eShaderType::PLANE_SHDR ) );

	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
	CheckGLErrors();

	glUseProgram(0);
	glBindVertexArray( 0 );
}
