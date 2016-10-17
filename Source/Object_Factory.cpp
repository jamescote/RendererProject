#include "Object_Factory.h"
#include "Triangle.h"
#include "Sphere.h"
#include "Plane.h"
#include "Mesh.h"
#include "Light.h"
#include <sstream>
#include <iterator>

/***********\
 * Defines *
\***********/
#define MAX_CHARS_PER_LINE 256
#define COMMENT_CHAR '#'

// Singleton Declaration
Object_Factory* Object_Factory::m_pInstance = NULL;

// Private Constructor
Object_Factory::Object_Factory()
{
	m_lNextID = 0;
}

// Returns the singleton instance of the Object Factory
Object_Factory* Object_Factory::getInstance()
{
	if ( NULL == m_pInstance )
		m_pInstance = new Object_Factory();

	return m_pInstance;
}


Object_Factory::~Object_Factory()
{
	// Nothing to Destruct
}

// Creation Functions
// Create a Sphere given a starting position, color and radius
Object3D* Object_Factory::createSphere( const glm::vec3* pStartPos,
										float fRadius )
{
	return new Sphere( pStartPos,
					   fRadius,
					   getNewID() );
}

// Create a Plane given a normal, a position on the plane and a color
Object3D* Object_Factory::createPlane( const glm::vec3* pPosition,
									   const glm::vec3* pPos1,
									   const glm::vec3* pPos2,
									   const glm::vec3* pPos3,
									   const glm::vec3* pPos4 )
{
	vector<glm::vec3> vCorners;
	vCorners.push_back( *pPos1 );
	vCorners.push_back( *pPos2 );
	vCorners.push_back( *pPos3 );
	vCorners.push_back( *pPos4 );

	return new Plane( pPosition,
					  &vCorners,				  
					  getNewID() );
}

// create a Triangle given 3 positions and a color.
Object3D* Object_Factory::createTriangle( const glm::vec3* pPosition,
										  const glm::vec3* pPos1,
										  const glm::vec3* pPos2,
										  const glm::vec3* pPos3 )
{
	vector<glm::vec3> vCorners;
	vCorners.push_back( *pPos1 );
	vCorners.push_back( *pPos2 );
	vCorners.push_back( *pPos3 );

	return new Triangle( pPosition,
						 &vCorners,
						 getNewID() );
}

Light* Object_Factory::createLight( const glm::vec3* pPos,
									const glm::vec3* pColor )
{
	return new Light( pPos,
					  pColor,
					  getNewID() );
}

Object3D* Object_Factory::createMesh( const glm::vec3* pPos,
								      string sLocation )
{
	return new Mesh( pPos,
					 sLocation,
					 getNewID() );
}

/**************************************************************************\
 * File Reading                                                           *
\**************************************************************************/

void Object_Factory::loadFromFile( string sFileName )
{
	ifstream inFile;
	string sBuffer;
	vector< string > sData;
	string sIndicator, sParser;
	glm::vec3 vPosition, vIn1, vIn2, vIn3, vIn4;

	// Open File
	inFile.open( sFileName );

	// Parse File if opened properly
	if ( inFile.good() )
	{
		// Parse to eof
		while ( !inFile.eof() )
		{
			// Get next line to parse
			getline( inFile, sBuffer );

			// Determine if we care about the line
			if ( "" != sBuffer && COMMENT_CHAR != sBuffer[0] )
			{
				// Determine keyword for the segment
				istringstream sStream( sBuffer );
				sStream >> sIndicator;

				// Store Data for the segment
				sData.clear();
				do
				{
					getline( inFile, sBuffer );	// Next line of Data
					sStream.str( sBuffer );		// Parse next Line
					sStream.clear();			// Clear Stream Flags
					while ( getline( sStream, sParser, ' ') )	// Extrapolate Data
						if ( "" != sParser )					// Avoid Garbage
							sData.push_back( sParser );
				} while ( "}" != sData.back() );				// Repeat until end delimiter

				if ( "sphere" == sIndicator )			// Parse Sphere
				{
					vPosition = glm::vec3( stof( sData[0] )/*X*/, stof( sData[1] )/*Y*/, stof( sData[2] )/*Z*/ );	// Position of Sphere
					createSphere( &vPosition, stof( sData[3] ) /*Radius of Sphere*/ );
				}
				else if ( "plane" == sIndicator )		// Parse Plane
				{
					vPosition	= glm::vec3( stof( sData[0] )/*X*/, stof( sData[1] )/*Y*/, stof( sData[2] )/*Z*/ );		// Position of Plane
					vIn1		= glm::vec3( stof( sData[3] )/*X*/, stof( sData[4] )/*Y*/, stof( sData[5] )/*Z*/ );		// First Corner of Plane
					vIn2		= glm::vec3( stof( sData[6] )/*X*/, stof( sData[7] )/*Y*/, stof( sData[8] )/*Z*/ );		// Second Corner of Plane
					vIn3		= glm::vec3( stof( sData[9] )/*X*/, stof( sData[10] )/*Y*/, stof( sData[11] )/*Z*/ );	// Third Corner of Plane
					vIn4		= glm::vec3( stof( sData[12] )/*X*/, stof( sData[13] )/*Y*/, stof( sData[14] )/*Z*/ );	// Fourth Corner of Plane
					createPlane( &vPosition, &vIn1, &vIn2, &vIn3, &vIn4 );
				}
				else if ( "triangle" == sIndicator )	// Parse Triangle
				{
					vPosition	= glm::vec3( stof( sData[0] )/*X*/, stof( sData[1] )/*Y*/, stof( sData[2] )/*Z*/ );		// Position of Triangle
					vIn1		= glm::vec3( stof( sData[3] )/*X*/, stof( sData[4] )/*Y*/, stof( sData[5] )/*Z*/ );		// Point 1 of Triangle
					vIn2		= glm::vec3( stof( sData[6] )/*X*/, stof( sData[7] )/*Y*/, stof( sData[8] )/*Z*/ );		// Point 2 of Triangle
					vIn3		= glm::vec3( stof( sData[9] )/*X*/, stof( sData[10] )/*Y*/, stof( sData[11] )/*Z*/ );	// Point 3 of Triangle
					createTriangle( &vPosition, &vIn1, &vIn2, &vIn3 );
				}
				else if ( "light" == sIndicator )		// Parse Light
				{
					vPosition = glm::vec3( stof( sData[0] )/*X*/, stof( sData[1] )/*Y*/, stof( sData[2] )/*Z*/ );
					glm::vec3 pColor( stof( sData[3] )/*R*/, stof( sData[4] )/*G*/, stof( sData[5] )/*B*/ );
					createLight( &vPosition, &pColor );
				}
				else if ( "mesh" == sIndicator )
				{
					vPosition = glm::vec3( stof( sData[0] )/*X*/, stof( sData[1] )/*Y*/, stof( sData[2] )/*Z*/ );	// Position of Mesh
					sData[3] = sData[3].substr(sData[3].find_first_not_of("\t"), sData[3].find_last_not_of("\t"));
					createMesh( &vPosition, sData[3] );
				}

				// Debugging
#ifdef DEBUG
				cout << sIndicator << endl;
				for ( unsigned int d = 0; d < sData.size(); ++d )
					cout << sData[d] << endl;
				cout << "\t\t END LINE" << endl;
#endif

			}
		}
	}

	// Close File
	inFile.close();
}
