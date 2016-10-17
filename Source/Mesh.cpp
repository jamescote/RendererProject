#include "Mesh.h"

Mesh::Mesh( const glm::vec3* pPosition, string sFileName, long lID ) 
	: Object3D( pPosition, lID ) 
{
	readMesh( sFileName );

	ShaderManager* pShdrMngr = ShaderManager::getInstance();

	// Setup Mesh information in GPU
	glGenVertexArrays( 1, &m_iVertexArray );

	m_iVertexBuffer = pShdrMngr->genVertexBuffer(m_iVertexArray, 0, 3, m_pMesh->vertices.data(), m_pMesh->vertices.size() * sizeof(trimesh::point) );
	m_iNormalBuffer = pShdrMngr->genVertexBuffer( m_iVertexArray, 1, 3, m_pMesh->normals.data(), m_pMesh->normals.size() * sizeof(trimesh::vec) );

	m_iIndicesBuffer = pShdrMngr->genIndicesBuffer( m_iVertexArray, m_pIndices.data(), m_pIndices.size() * sizeof( unsigned int ) );
}

// Delete any buffers that we initialized
Mesh::~Mesh()
{
	glDeleteBuffers( 1, &m_iNormalBuffer );
	glDeleteBuffers( 1, &m_iIndicesBuffer );
	glDeleteBuffers( 1, &m_iVertexBuffer );
	glDeleteVertexArrays( 1, &m_iVertexArray );
	if (NULL != m_pMesh)
		delete m_pMesh;
}

// Load the Mesh from a given file name
//  Result: Stores the mesh variables into a set of vertices
void Mesh::readMesh( string sFileName )
{
	m_pMesh = trimesh::TriMesh::read(sFileName);

	m_pMesh->need_bbox();
	m_pMesh->need_faces();
	m_pMesh->need_normals();
	m_pMesh->need_bsphere();

	for ( unsigned int i = 0; i < m_pMesh->faces.size(); i++ )
	{
		m_pIndices.push_back( m_pMesh->faces[i][0] );
		m_pIndices.push_back( m_pMesh->faces[i][1] );
		m_pIndices.push_back( m_pMesh->faces[i][2] );
	}
}

// draws the Mesh by setting up the Shader
void Mesh::draw()
{
	ShaderManager* pShdrMngr = ShaderManager::getInstance();

	glBindVertexArray( m_iVertexArray );

	glUseProgram( pShdrMngr->getProgram( eShaderType::MESH_SHDR ) );

	glDrawElements( GL_TRIANGLES, m_pMesh->faces.size() * 3, GL_UNSIGNED_INT, 0 );

	glUseProgram(0);
	glBindVertexArray( 0 );
}

// Data Dump for Debugging
string Mesh::getDebugOutput()
{
	string sReturnVal = "";

	sReturnVal += getType();
	sReturnVal += "/ID:" + to_string( m_lID );
	
	sReturnVal += "/Normals:";
	for ( unsigned int i = 0; i < m_pMesh->normals.size(); ++i )
	{
		sReturnVal += "[" + to_string( m_pMesh->normals[i][0] );	// x
		sReturnVal += ", " + to_string( m_pMesh->normals[i][1] );	// y
		sReturnVal += ", " + to_string( m_pMesh->normals[i][2] );	// z
		sReturnVal += "]";
	}

	sReturnVal += "/Vertices:";
	for ( unsigned int i = 0; i < m_pMesh->vertices.size(); ++i )
	{
		sReturnVal += "[" + to_string( m_pMesh->vertices[i][0] );	// x
		sReturnVal += ", " + to_string( m_pMesh->vertices[i][1] );	// y
		sReturnVal += ", " + to_string( m_pMesh->vertices[i][2] );	// z
		sReturnVal += "]";
	}

	return sReturnVal;
}