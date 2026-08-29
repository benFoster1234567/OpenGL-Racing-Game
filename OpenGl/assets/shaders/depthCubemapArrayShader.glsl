#ifdef VERTEX_SHADER

layout (location = 0) in vec3 aPos;

uniform mat4 model;

void main()
{
    gl_Position = model * vec4(aPos, 1.0);
}  

#endif

#ifdef GEOMETRY_SHADER

layout (triangles) in;
layout (triangle_strip, max_vertices=288) out;

struct StaticPointlightData
{
    mat4 shadowTransform;
    vec4 lightPosition;
}

layout (std140) uniform ShadowBlock {
    StaticPointlightData lights[MAX_LIGHTS];
    int activeLightCount;
} ub;

uniform mat4 shadowMatrices[96];
uniform int numLights;
out vec4 FragPos;

void main()
{
    unsigned int numFaces = 6 * numLights;  

    for(int face = 0; face < numFaces; ++face)
    {
        gl_Layer = face; 
        for(int i = 0; i < 3; ++i)
        {
            FragPos = gl_in[i].gl_Position;
            gl_Position = ub.lights[face] * FragPos;
            EmitVertex();
        }    
        EndPrimitive();
    }
}  

#endif

#ifdef FRAGMENT_SHADER

in vec4 FragPos;

uniform vec3 lightPos;
uniform float far_plane;

void main()
{
    float lightDistance = length(FragPos.xyz - lightPos);
    lightDistance = lightDistance / far_plane;
    gl_FragDepth = lightDistance;
}  
#endif