#version 430 core

uniform float b = 1.0;		// Determine maxmimum "blueness" in temperature shift
uniform float y = 1.0;		// Determine maximmum "yellowness" in temperature shift
uniform float alpha = 0.5;	// Amount of which the Color is visible in the Cool Temperature
uniform float beta = 0.5;	// Amount of which the Color is visible in the Warm Temperature
uniform float rc = 0.0;		// Red Value of the Object's Color
uniform float gc = 1.0;		// Green Value of the Object's Color	
uniform float bc = 0.0;		// Blue Value of the Object's Color

out vec4 color;

in vec3 N;
in vec3 L;
in vec3 V;

void main(void)
{    
	vec3 vObjColor = vec3( rc, gc, bc );
	vec3 kCool = vec3( 0.0, 0.0, b) + (alpha * vObjColor);
	vec3 kWarm = vec3( y, y, 0.0 ) + (beta * vObjColor);
	
	vec3 white = vec3( 1.0, 1.0, 1.0 );
	
	vec3 R = reflect(-L, N);
	
	float e = 5.0;
	
	// Implementing Gooch Shading:
	//		Formula: I = (( 1 - (L.N))/2) * kCool + 
	//					  (1 - (1 - (L.N))/2) * kWarm
	float fDotCalc = clamp((1.0 - dot(N,L)) / 2.0, 0.0, 1.0);
	vec3 diffuse = (fDotCalc * kCool) + ((1 - fDotCalc) * kWarm);
	float specular = pow(max( dot(R,V), 0.0 ), e);
	
    color = vec4(diffuse + white*specular, 1.0);
}
