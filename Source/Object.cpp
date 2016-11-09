#include "Object.h"
#include "TextureManager.h"

// Default Construtor to initialize position and ID
Object::Object( const vec3* pPos, long lID, const string* sTexName )
{
	m_pPosition = *pPos;
	m_lID = lID;
	m_sTexName = *sTexName;
	m_pTexture = TextureManager::getInstance()->loadTexture( sTexName, lID );
}

// Copy Constructor
Object::Object( const Object* pCopy )
{
	m_pPosition = pCopy->m_pPosition;
	m_lID = pCopy->m_lID;
}

// Destructor: unload Texture.
Object::~Object()
{
	TextureManager::getInstance()->unloadTexture( &m_sTexName, m_lID );
}

// Changes the Texture of the Object.  Given a Texture Location, it will attempt to
// Load the new texture first to see if it can be loaded.  If it can, it will unload its
// current texture and store the new texture.
void Object::switchTexture( const string* sTexLoc )
{
	TextureManager* pTxMngr = TextureManager::getInstance();
	Texture* pNewTexture = pTxMngr->loadTexture( sTexLoc, m_lID );

	if ( NULL != pNewTexture )
	{
		pTxMngr->unloadTexture( &m_sTexName, m_lID );
		m_sTexName = *sTexLoc;
		m_pTexture = pNewTexture;
	}
}
