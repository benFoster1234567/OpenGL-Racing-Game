#ifdef VERTEX_SHADER

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

vec4 pos; 
out vec4 normOut;

void main() 
{
    pos = projection * view * model * vec4(aPos, 1.0);
    gl_Position = pos;
    normOut = vec4(aNorm, 1.0);
}

#endif

#ifdef FRAGMENT_SHADER
in vec4 normOut;    
out vec4 FragColor;

void main() 
{
    FragColor = normOut;
}

#endif