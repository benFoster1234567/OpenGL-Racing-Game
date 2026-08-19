#ifdef VERTEX_SHADER

layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}  

#endif

#ifdef FRAGMENT_SHADER

void main()
{

}

#endif