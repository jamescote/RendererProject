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
#define MAX_SPHERE_PARAMS 5
#define MAX_PLANE_PARAMS 16
#define MAX_TRI_PARAMS 13
#define MAX_LIGHT_PARAMS 7
#define MAX_MESH_PARAMS 5
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
Object3D* Object_Factory::createSphere( vector< string > sData, int iLength )
{
	Sphere* pReturnSphere = NULL;
	vec3 vPosition;
	string sTexLocation = "";
	
	// Create the Sphere if we're given the correct # of values
	if ( iLength == MAX_SPHERE_PARAMS )
	{
		vPosition = glm::vec3( stof( sData[ 0 ] )/*X*/, stof( sData[ 1 ] )/*Y*/, stof( sData[ 2 ] )/*Z*/ );	// Position of Sphere

		if ( string::npos != sData[ 4 ].find( "\t" ) )
			sTexLocation = sData[ 4 ].substr( sData[ 4 ].find_first_not_of( "\t" ), sData[ 4 ].find_last_not_of( "\t" ) );
		else
			sTexLocation = sData[ 4 ];

		pReturnSphere = new Sphere( &vPosition,
								    stof( sData[3] ),	// Radius of Sphere
									getNewID(),			// Unique ID
								    &sTexLocation );			// TextureName (if applicable)
	}

	return pReturnSphere;
}

// Create a Plane given a normal, a position on the plane and a color
Object3D* Object_Factory::createPlane( vector< string > sData, int iLength )
{
	vec3 pPosition;
	vector<glm::vec3> vCorners;
	Plane* pReturnPlane = NULL;
	string sTexLocation = "";

	if ( iLength == MAX_PLANE_PARAMS )
	{
		pPosition = glm::vec3( stof( sData[ 0 ] )/*X*/, stof( sData[ 1 ] )/*Y*/, stof( sData[ 2 ] )/*Z*/ );		// Position of Plane
		vCorners.push_back( glm::vec3( stof( sData[ 3 ] )/*X*/, stof( sData[ 4 ] )/*Y*/, stof( sData[ 5 ] )/*Z*/ ) );		// First Corner of Plane 
		vCorners.push_back( glm::vec3( stof( sData[ 6 ] )/*X*/, stof( sData[ 7 ] )/*Y*/, stof( sData[ 8 ] )/*Z*/ ) );		// Second Corner of Plane
		vCorners.push_back( glm::vec3( stof( sData[ 9 ] )/*X*/, stof( sData[ 10 ] )/*Y*/, stof( sData[ 11 ] )/*Z*/ ) );		// Third Corner of Plane
		vCorners.push_back( glm::vec3( stof( sData[ 12 ] )/*X*/, stof( sData[ 13 ] )/*Y*/, stof( sData[ 14 ] )/*Z*/ ) );		// Fourth Corner of Plane

		if ( string::npos != sData[ 15 ].find( "\t" ) )
			sTexLocation = sData[ 15 ].substr( sData[ 15 ].find_first_not_of( "\t" ), sData[ 15 ].find_last_not_of( "\t" ) );
		else
			sTexLocation = sData[ 15 ];

		pReturnPlane = new Plane( &pPosition,
								  &vCorners,
								  getNewID(),
								  &sTexLocation );
	}

	return pReturnPlane;
}

// create a Triangle given 3 positions and a color.
Object3D* Object_Factory::createTriangle( vector< string > sData, int iLength )
{
	vec3 pPosition;
	vector<glm::vec3> vCorners;
	Triangle* pReturnTri = NULL;
	string sTexLocation = "";

	if ( MAX_TRI_PARAMS == iLength )
	{
		pPosition = glm::vec3( stof( sData[ 0 ] )/*X*/, stof( sData[ 1 ] )/*Y*/, stof( sData[ 2 ] )/*Z*/ );		// Position of Triangle

		vCorners.push_back( glm::vec3( stof( sData[ 3 ] )/*X*/, stof( sData[ 4 ] )/*Y*/, stof( sData[ 5 ] )/*Z*/ ) );		// Point 1 of Triangle
		vCorners.push_back( glm::vec3( stof( sData[ 6 ] )/*X*/, stof( sData[ 7 ] )/*Y*/, stof( sData[ 8 ] )/*Z*/ ) );		// Point 2 of Triangle
		vCorners.push_back( glm::vec3( stof( sData[ 9 ] )/*X*/, stof( sData[ 10 ] )/*Y*/, stof( sData[ 11 ] )/*Z*/ ) );		// Point 3 of Triangle

		if ( string::npos != sData[ 12 ].find( "\t" ) )
			sTexLocation = sData[ 12 ].substr( sData[ 12 ].find_first_not_of( "\t" ), sData[ 12 ].find_last_not_of( "\t" ) );
		else
			sTexLocation = sData[ 12 ];

		pReturnTri = new Triangle( &pPosition,
								   &vCorners,
								   getNewID(),
								   &sTexLocation );
	}

	return pReturnTri;
}

Light* Object_Factory::createLight( vector< string > sData, int iLength )
{
	vec3 pPosition, pColor;
	Light* pReturnLight = NULL;
	string sTexLocation = "";

	if ( MAX_LIGHT_PARAMS == iLength )
	{
		pPosition = vec3( stof( sData[ 0 ] )/*X*/, stof( sData[ 1 ] )/*Y*/, stof( sData[ 2 ] )/*Z*/ );
		pColor = vec3( stof( sData[ 3 ] )/*R*/, stof( sData[ 4 ] )/*G*/, stof( sData[ 5 ] )/*B*/ );

		if ( string::npos != sData[ 6 ].find( "\t" ) )
			sTexLocation = sData[ 6 ].substr( sData[ 6 ].find_first_not_of( "\t" ), sData[ 6 ].find_last_not_of( "\t" ) );
		else
			sTexLocation = sData[ 6 ];

		pReturnLight = new Light( &pPosition,
								  &pColor,
								  getNewID(),
								  &sTexLocation );
	}

	return pReturnLight;
}

Object3D* Object_Factory::createMesh( vector< string > sData, int iLength )
{
	vec3 pPosition;
	string sLocation, sTexLocation = "";
	Mesh* pReturnMesh = NULL;

	if ( MAX_MESH_PARAMS == iLength )
	{
		pPosition = glm::vec3( stof( sData[ 0 ] )/*X*/, stof( sData[ 1 ] )/*Y*/, stof( sData[ 2 ] )/*Z*/ );	// Position of Mesh
		sLocation = sData[ 3 ].substr( sData[ 3 ].find_first_not_of( "\t" ), sData[ 3 ].find_last_not_of( "\t" ) );

		if( string::npos != sData[4].find("\t") )
			sTexLocation = sData[ 4 ].substr( sData[ 4 ].find_first_not_of( "\t" ), sData[ 4 ].find_last_not_of( "\t" ) );
		else
			sTexLocation = sData[ 4 ];

		pReturnMesh = new Mesh( &pPosition,
								&sLocation,
								getNewID(),
								&sTexLocation );
	}

	return pReturnMesh;
}

Object3D* Object_Factory::createMesh( const vec3* pPos, 
									  const string* sLocation, 
									  const string* sTexLocation )
{
	Mesh* pNewMesh = NULL;
	if( sLocation->find(".ply") != string::npos )
		pNewMesh = new Mesh( pPos,
							 sLocation,
							 getNewID(),
							 sTexLocation );

	return pNewMesh;
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
	Object* pResultingObject = NULL;

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

				// remove "}" from end of Data List
				sData.pop_back();

				if ( "sphere" == sIndicator )			// Parse Sphere
				{
					if ( MAX_SPHERE_PARAMS > sData.size() )	// No Texture Specified
						sData.push_back( "" );

					pResultingObject = createSphere( sData, sData.size() );
				}
				else if ( "plane" == sIndicator )		// Parse Plane
				{
					if ( MAX_PLANE_PARAMS > sData.size() )	// No Texture Specified
						sData.push_back( "" );

					pResultingObject = createPlane( sData, sData.size() );
				}
				else if ( "triangle" == sIndicator )	// Parse Triangle
				{
					if ( MAX_TRI_PARAMS > sData.size() )
						sData.push_back( "" );

					pResultingObject = createTriangle( sData, sData.size() );
				}
				else if ( "light" == sIndicator )		// Parse Light
				{
					if ( MAX_LIGHT_PARAMS > sData.size() )
						sData.push_back( "" );

					pResultingObject = createLight( sData, sData.size() );
				}
				else if ( "mesh" == sIndicator )		// Parse Mesh
				{
					if ( MAX_MESH_PARAMS > sData.size() )
						sData.push_back( "" );

					pResultingObject = createMesh( sData, sData.size() );
				}

				if ( NULL == pResultingObject )
					outputError( &sIndicator, sData );

				pResultingObject = NULL; // Don't do anything with the Object
									     // Handled by the Environment Manager

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

// outputError - Outputs information about an error creating an object.  Usually if 
//					there were an incorrect number of parameters.
// Params: sName - The Name of the Object trying to be loaded
//		   sData - The Data read in when trying to load.
void Object_Factory::outputError( const string* sName, vector<string> sData )
{
	cout << "Error creating " << sName << " with the following data:" << endl;
	cout << "{";

	// Output Data
	for ( vector<string>::iterator pIter = sData.begin();
		 pIter != sData.end();
		 ++pIter )
	{
		cout << *pIter;
		if ( pIter != sData.end() - 1 )
			cout << ", ";
	}
	cout << "}" << endl;
}
