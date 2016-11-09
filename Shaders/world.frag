#version 430 core

out vec4 color;

in vec3 vVert;

void main(void)
{    
    color = vec4( vVert, 1.0 );
}
