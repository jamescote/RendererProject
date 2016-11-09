#pragma once
#include "stdafx.h"
#include "Texture.h"

class Object
{
public:
	~Object();

	// Pure Virtual Intersection Function
	// All inherited classes must implement their own
	//		logic for this function.
	virtual void draw() = 0;
	virtual string getType() = 0;
	virtual string getDebugOutput() = 0;

	void switchTexture( const string* sTexLoc );

	// Getters/Setters
	long ID() const { return m_lID; }								// Get ID for the object																	
	const glm::vec3& getPosition() { return m_pPosition; }			// Retrieve the Position of the Light

protected:
	vec3 m_pPosition;
	long m_lID;
	Texture* m_pTexture;
	string m_sTexName;

	// Protected Constructor, Only available to children and Object Factory
	Object( const vec3* pPos, long lID, const string* sTexName );
	Object( const Object* pCopy );			// Copy Constructor.
};

