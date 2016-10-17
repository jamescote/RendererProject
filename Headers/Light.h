#pragma once
#include "stdafx.h"

// Infinitely small point that emits a light source.
//	Managed by the Environment Manager.
class Light 
{
public:
	~Light();

	// Get ID of the Light
	long ID() { return m_lID; }

	// Virtual inherited functions
	void draw();
	string getType() { return "Light"; }

	// Retrieve the Position of the Light
	const glm::vec3& getPosition() { return m_pPosition; }

	// Light Manipulation
	void move(vec3 pMoveVec);

	// Ouptut Debug information for Light
	string getDebugOutput()
	{
		string sOutput = "";

		sOutput += "Light";
		sOutput += "/ID:" + to_string( m_lID );
		sOutput += "/Position:" + glm::to_string(m_pPosition);
		sOutput += "/Color:" + glm::to_string(m_pColor);

		return sOutput;
	}

private:
	// Protected Constructors, only accessible by the Object Factory.
	Light( const glm::vec3* pPos,
		   const glm::vec3* pColor,
		   long lID );
	Light( const Light* newLight );

	// Private Variables
	vec3 m_pPosition, m_pColor;
	long m_lID;
	GLuint m_iVertexArray, m_iVertexBuffer;
	

	// Shake hands with the Object Factory so that Lights can only be made from the Object Factory.
	friend class Object_Factory;
};

