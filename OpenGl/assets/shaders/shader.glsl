#ifdef VERTEX_SHADER

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 posOut; 

void main() 
{
    posOut = projection * view * model * vec4(aPos, 1.0);
    gl_Position = posOut;
}

#endif

#ifdef FRAGMENT_SHADER
in vec4 posOut;    
out vec4 FragColor;

void main() 
{
    FragColor = posOut;
}

#endif