#include "Light.h"
#include "EnvironmentManager.h"

const float LIGHT_SIZE = 30.f;

// Constructor
Light::Light(const glm::vec3* pPos,
	const glm::vec3* pColor,
	long lID)
{
	m_pColor		= (*pColor);
	m_pPosition		= (*pPos);
	m_lID			= lID;
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

	glBindVertexArray(m_iVertexArray);

	glUseProgram(pShdrMngr->getProgram(eShaderType::LIGHT_SHDR));

	glPointSize(30.f);
	glDrawArrays(GL_POINTS, 0, 1);

	glUseProgram(0);
	glBindVertexArray(0);
}

// Moves the Light in direction of pMoveVec
// Could add Collision as possible extension.
void Light::move(vec3 pMoveVec)
{
	m_pPosition += pMoveVec;
}

