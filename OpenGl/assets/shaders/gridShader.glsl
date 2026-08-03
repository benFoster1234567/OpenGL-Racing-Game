#ifdef VERTEX_SHADER

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    gl_Position = projection * view * worldPos;
}

#endif

#ifdef FRAGMENT_SHADER

#define MAX_LIGHTS 100

//struct StaticPointLight {
//    vec4 posRad; 
//    vec4 color;  
//};
//
//layout (std140) uniform LightBlock {
//    StaticPointLight lights[MAX_LIGHTS];
//    int activeLightCount;
//} ub;

uniform mat4 view; 

out vec4 FragColor;

void main()
{
   vec4 fColor = vec4(1.0,1.0,1.0,1.0);
   FragColor = fColor;
}

#endif