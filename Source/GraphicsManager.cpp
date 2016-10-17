#include "GraphicsManager.h"
#include "ShaderManager.h"
#include "Object_Factory.h"
#include "EnvironmentManager.h"

// Singleton Variable initialization
GraphicsManager* GraphicsManager::m_pInstance = NULL;

// Constructor - Private, only accessable within the Graphics Manager
GraphicsManager::GraphicsManager(GLFWwindow* rWindow)
{
	// Initialize and Get Shader and Environment Managers
	m_pShaderMngr	= ShaderManager::getInstance();
	m_pEnvMngr		= EnvironmentManager::getInstance();

	m_pWindow = rWindow;
	int iHeight, iWidth;
	glfwGetWindowSize(m_pWindow, &iWidth, &iHeight);

	// Set up Camera
	m_pCamera = new Camera( iHeight, iWidth );
}

// Singleton Implementations
// Requires Window to initialize 
GraphicsManager* GraphicsManager::getInstance(GLFWwindow *rWindow)
{
	if (NULL == m_pInstance)
		m_pInstance = new GraphicsManager(rWindow);

	return m_pInstance;
}

// Destruct Shaders, Buffers, Arrays and other GL stuff.
GraphicsManager::~GraphicsManager()
{
	// Destruct Camera
	if ( NULL != m_pCamera )
		delete m_pCamera;

	// Let go of Window Handle
	m_pWindow = NULL;

	// Let go of Manager Handles
	if ( NULL != m_pEnvMngr )
		delete m_pEnvMngr;

	if ( NULL != m_pShaderMngr )
		delete m_pShaderMngr;
}

// Intended to be called every cycle, or when the graphics need to be updated
bool GraphicsManager::renderGraphics()
{
	// call function to draw our scene
	RenderScene();

	// scene is rendered to the back buffer, so swap to front for display
	glfwSwapBuffers(m_pWindow);

	// check for Window events
	glfwPollEvents();

	return !glfwWindowShouldClose(m_pWindow);
}

// --------------------------------------------------------------------------
// Rendering function that draws our scene to the frame buffer
// Copied from Boilercode Program
// Will be replaced with functions in Graphic objects.
void GraphicsManager::RenderScene()
{
	mat4 pModelViewMatrix = m_pCamera->getToCameraMat();
	mat4 pProjectionMatrix = m_pCamera->getPerspectiveMat();
	GLfloat color[] = { 0.3215f, 0.3411f, 0.4352f, 1.0f };
	const GLfloat zero = 1.0f;

	glClearBufferfv(GL_COLOR, 0, color);
	glClearBufferfv(GL_DEPTH, 0, &zero);
	glEnable(GL_DEPTH_TEST);
	
	// Set camera information in Shaders before rendering
	m_pShaderMngr->setUnifromMatrix4x4( eShaderType::MESH_SHDR, "modelview", &pModelViewMatrix );
	m_pShaderMngr->setUnifromMatrix4x4( eShaderType::LIGHT_SHDR, "modelview", &pModelViewMatrix );
	m_pShaderMngr->setUnifromMatrix4x4( eShaderType::PLANE_SHDR, "modelview", &pModelViewMatrix );

	m_pShaderMngr->setUnifromMatrix4x4( eShaderType::MESH_SHDR, "projection", &pProjectionMatrix );
	m_pShaderMngr->setUnifromMatrix4x4( eShaderType::LIGHT_SHDR, "projection", &pProjectionMatrix );
	m_pShaderMngr->setUnifromMatrix4x4( eShaderType::PLANE_SHDR, "projection", &pProjectionMatrix );

	m_pEnvMngr->renderEnvironment();

	glDisable(GL_DEPTH_TEST);
}

// Function initializes shaders and geometry.
// contains any initializion requirements in order to start drawing.
bool GraphicsManager::initializeGraphics( string sFileName )
{
	// Locals
	bool bError = false;

	// Shaders
	if (!m_pShaderMngr->initializeShaders())
	{
		cout
			<< "Couldn't initialize shaders." << endl;
		bError = true;
	}
	else
		m_pEnvMngr->initializeEnvironment(sFileName);

	return bError; 
}

/*******************************************************************************\
 * Camera Manipulation                                                         *
\*******************************************************************************/

void GraphicsManager::rotateCamera(vec2 pDelta)
{
	m_pCamera->orbit(pDelta);
}

void GraphicsManager::zoomCamera(float fDelta)
{
	m_pCamera->zoom(fDelta);
}

/*******************************************************************************\
* Shader Manipulation                                                          *
\*******************************************************************************/

// Set rc, gc or bc in the Mesh Shader.
void GraphicsManager::setRGBVal(eRGB eType, float fVal)
{
	string sVarName;

	// Set User-defined uniform variable name.
	switch (eType)
	{
	case RED:
		sVarName = "rc";
		break;
	case GREEN:
		sVarName = "gc";
		break;
	case BLUE:
		sVarName = "bc";
		break;
	default:
		sVarName = "";
	}

	if( fVal >= 0.0f && fVal <= 1.0f && 
		eType < RGB_MAX && eType >= 0 )
		m_pShaderMngr->setUniformFloat( eShaderType::MESH_SHDR, sVarName, fVal);
}

// Set beta in the Mesh Shader.
void GraphicsManager::setBeta(float fVal)
{
	if (fVal >= 0.0f && fVal <= 1.0f )
		m_pShaderMngr->setUniformFloat(eShaderType::MESH_SHDR, "beta", fVal);
}

// Set alpha in the Mesh Shader.
void GraphicsManager::setAlpha(float fVal)
{
	if (fVal >= 0.0f && fVal <= 1.0f)
		m_pShaderMngr->setUniformFloat(eShaderType::MESH_SHDR, "alpha", fVal);
}

// Set b in the Mesh Shader.
void GraphicsManager::setBVal(float fVal)
{
	if (fVal >= 0.0f && fVal <= 1.0f)
		m_pShaderMngr->setUniformFloat(eShaderType::MESH_SHDR, "b", fVal);
}

// Set y in the Mesh Shader.
void GraphicsManager::setYVal(float fVal)
{
	if (fVal >= 0.0f && fVal <= 1.0f)
		m_pShaderMngr->setUniformFloat(eShaderType::MESH_SHDR, "y", fVal);
}

