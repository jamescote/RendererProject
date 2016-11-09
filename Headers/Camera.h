#pragma once
#include "stdafx.h"

#define ZOOM_MAX			100.f

// Camera Class
// Positions a Camera in the world at some spherical coordinates 
class Camera
{
public:
	Camera( int iHeight, int iWidth );			// Default Constructor
	Camera( Camera& pCopy );					// Overloaded Copy Constructor.
	~Camera();

	// Updating Functions
	void updateHxW( int iHeight, int iWidth );
	void update() { m_bUpdated = true; }

	// Set up Camera Matrices
	mat4 getToCameraMat();
	mat4 getPerspectiveMat();
	vec3 getCameraWorldPos();

	/// Camera Manipulation Functions
	void orbit( vec2 pDelta );
	void zoom( float fDelta );

private:
	vec3 m_vSPos, m_vWorldLookAt;
	bool m_bUpdated;
	int m_iHeight, m_iWidth;
	float m_fAspectRatio;

	vec3 getCartesianPos();
};

