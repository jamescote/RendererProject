#version 430 core

uniform mat4 modelview;
uniform mat4 projection;
uniform vec3 lightPosition;

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;

//attributes in camera coordinates
out vec3 N;
out vec3 L;
out vec3 V;

void main(void)
{
	vec4 lightCameraSpace = modelview * vec4(lightPosition, 1.0);
	lightCameraSpace /= lightCameraSpace.w;
	
    float scale = 4.0;    
    vec3 positionModelSpace = vertex*scale + vec3(0.0, -0.42, 0.0);
    vec4 positionCameraSpace = modelview * vec4(positionModelSpace, 1.0);
	vec3 P = positionCameraSpace.xyz/positionCameraSpace.w;
	
	// create the Normal Matrix to correct Normal into camera space
	mat3 normalMatrix = transpose(inverse(mat3(modelview)));
	N = normalize( normalMatrix * normal );
	
	L = normalize(lightCameraSpace.xyz - P);
	
	V = normalize( - P);
	
    gl_Position = projection * positionCameraSpace;    
}
