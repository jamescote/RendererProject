#pragma once
#include "stdafx.h"
#include <unordered_map>
#include "Texture.h"

// Forward Declaration
class Object;

class TextureManager
{
public:
	static TextureManager* getInstance();
	~TextureManager();

	Texture* loadTexture(const string *sFileName, long lID);
	void unloadTexture(const string *sFileName, long lID);

private:
	static TextureManager* pInstance;
	TextureManager();
	TextureManager( const TextureManager* pCopy );
	struct TextureContainer
	{
		Texture* pTexture;
		vector<long> lUserIDs;
	};
	unordered_map<string, TextureContainer> m_pTextureCache;
};

