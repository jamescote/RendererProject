#include "ShaderManager.h"

/////////////
// Defines //
/////////////

// Singleton Variable initialization
ShaderManager* ShaderManager::m_pInstance = NULL;

// Different Vertex Shaders required for each assignment
const string m_sVShaderNames[MAX_SHDRS] = {
	"Shaders/light.vert",
	"Shaders/mesh.vert",
	"Shaders/plane.vert"
};

// Different Fragment Shaders required for each assignment
const string m_sFShaderNames[MAX_SHDRS] = {
	"Shaders/light.frag",
	"Shaders/mesh.frag",
	"Shaders/plane.frag"
};

// Public - Not a singleton
// Designed mainly to manage different shaders between assignments.  
// Could make Assignment Classes that each have their own shader managers: TODO.
ShaderManager::ShaderManager()
{
	m_bInitialized = false;
}

// Get the Singleton ShaderManager Object.  Initialize it if NULL.
ShaderManager* ShaderManager::getInstance()
{
	if (NULL == m_pInstance)
		m_pInstance = new ShaderManager();

	return m_pInstance;
}

// Destructor - Kill any shaders that we've been using.
ShaderManager::~ShaderManager()
{
	// unbind any shader programs
	glUseProgram(0);
}

/*******************************************************************\
 * Set up Shaders												   *
\*******************************************************************/

// Inializes shaders. 
bool ShaderManager::initializeShaders()
{
	GLint iVariableLoc = -1;

	// Initialize Shaders
	m_bInitialized = true;
	for ( int eIndex = LIGHT_SHDR; eIndex < MAX_SHDRS; eIndex++ )
		m_bInitialized &= m_pShader[eIndex].initializeShader( m_sVShaderNames[eIndex], m_sFShaderNames[eIndex] );

	// return False if not all Shaders Initialized Properly
	return m_bInitialized;
}


/*******************************************************************************\
* Shader manipulation														   *
\*******************************************************************************/

// Binds a creates a buffer on the GPU.  Sets the data into the buffer and returns the location of the buffer.
GLuint ShaderManager::genVertexBuffer( GLuint iVertArray,
									   GLuint iSpecifiedIndex,
									   GLint iChunkSize,
									   const void* pData, GLsizeiptr pSize )
{
	GLuint iVertexBufferLoc;

	glBindVertexArray( iVertArray );

	glGenBuffers( 1, &iVertexBufferLoc );
	glBindBuffer( GL_ARRAY_BUFFER, iVertexBufferLoc );
	glBufferData( GL_ARRAY_BUFFER, pSize, pData, GL_STATIC_DRAW );
	glVertexAttribPointer( iSpecifiedIndex, iChunkSize, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( iSpecifiedIndex );

	glBindVertexArray( 0 );
	
	return iVertexBufferLoc;
}

// Binds and creates an Element Array Buffer on the GPU.  Sets the data into the buffer and returns the location.
GLuint ShaderManager::genIndicesBuffer( GLuint iVertArray, 
										const void* pData, GLsizeiptr pSize )
{
	GLuint iIndicesBufferLoc;

	glBindVertexArray( iVertArray );

	glGenBuffers( 1, &iIndicesBufferLoc );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, iIndicesBufferLoc );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, pSize, pData, GL_STATIC_DRAW );

	glBindVertexArray( 0 );

	return iIndicesBufferLoc;
}

// given a glm 4x4 Matrix, a specifed shader and a variablename, attempt to set the given matrix into that uniform variable.
void ShaderManager::setUnifromMatrix4x4( eShaderType eType, string sVarName, const mat4* pResultingMatrix )
{
	GLint iVariableLocation;
	GLint iProgram;
	
	if ( eType < eShaderType::MAX_SHDRS && eType >= 0 )
	{
		iProgram = getProgram( eType );

		glUseProgram( iProgram );
		iVariableLocation = glGetUniformLocation( iProgram, sVarName.c_str() );
		if ( ERR_CODE != iVariableLocation )
			glUniformMatrix4fv( iVariableLocation, 1, GL_FALSE, value_ptr( *pResultingMatrix ) );
		glUseProgram( 0 );

		#ifdef DEBUG
			CheckGLErrors();
		#endif // DEBUG
	}
}

// given a glm vec3 set it as the unifrom light position in the mesh shader
void ShaderManager::setUniformVec3( eShaderType eType, string sVarName, const glm::vec3* pResultingVector )
{
	GLint iVariableLocation;
	GLint iProgram;

	if ( eType < eShaderType::MAX_SHDRS && eType >= 0 )
	{
		iProgram = getProgram( eType );

		glUseProgram( iProgram );
		iVariableLocation = glGetUniformLocation( iProgram, sVarName.c_str() );
		if ( ERR_CODE != iVariableLocation )
			glUniform3fv( iVariableLocation, 1, glm::value_ptr( *pResultingVector ) );
		glUseProgram( 0 );

		#ifdef DEBUG
			CheckGLErrors();
		#endif // DEBUG
	}
}

void ShaderManager::setUniformFloat(eShaderType eType, string sVarName, float fVal)
{
	GLint iVariableLocation;
	GLint iProgram;

	if (eType < eShaderType::MAX_SHDRS && eType >= 0)
	{
		iProgram = getProgram(eType);

		glUseProgram(iProgram);
		iVariableLocation = glGetUniformLocation(iProgram, sVarName.c_str());
		if (ERR_CODE != iVariableLocation)
			glUniform1f(iVariableLocation, fVal);
		glUseProgram(0);
		
		#ifdef DEBUG
			CheckGLErrors();
		#endif // DEBUG
	}
}