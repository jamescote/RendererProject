#include "Light.h"
#include "EnvironmentManager.h"

const float LIGHT_SIZE = 30.f;

// Constructor
Light::Light(const glm::vec3* pPos,
			 const glm::vec3* pColor,
			 long lID, const string* sTexName ) : Object( pPos, lID, sTexName )
{
	m_pColor		= (*pColor);
	EnvironmentManager::getInstance()->addLight( this );

	glGenVertexArrays(1, &m_iVertexArray);

	m_iVertexBuffer = ShaderManager::getInstance()->genVertexBuffer(m_iVertexArray,
																	0, 3, &m_pPosition,
																	sizeof(m_pPosition));
}

// Destructor
Light::~Light()
{
	glDeleteBuffers(1, &m_iVertexBuffer);
	glDeleteVertexArrays(1, &m_iVertexArray);
}

void Light::draw()
{
	ShaderManager* pShdrMngr = ShaderManager::getInstance();

	if ( NULL != m_pTexture )
	{
		m_pTexture->bindTexture( eShaderType::LIGHT_SHDR, "gSampler" );
	}

	glBindVertexArray(m_iVertexArray);
	glUseProgram(pShdrMngr->getProgram(eShaderType::LIGHT_SHDR));

	glPointSize(20.f);
	glDrawArrays(GL_POINTS, 0, 1);
	glPointSize( 1.f );

	if ( NULL != m_pTexture )
	{
		m_pTexture->unbindTexture();
	}

	glUseProgram(0);
	glBindVertexArray(0);
}
