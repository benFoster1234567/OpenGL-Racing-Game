#define MULTI_LIGHT

#ifdef SINGLE_LIGHT
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
layout (triangle_strip, max_vertices=18) out;

uniform mat4 shadowMatrices[6];

out vec4 FragPos;

void main()
{
    for(int face = 0; face < 6; ++face)
    {
        gl_Layer = face; 
        for(int i = 0; i < 3; ++i)
        {
            FragPos = gl_in[i].gl_Position;
            gl_Position = shadowMatrices[face] * FragPos;
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
#endif

#ifdef MULTI_LIGHT
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
layout (triangle_strip, max_vertices=72) out;

struct StaticPointlightData
{
    mat4 shadowTransforms[6];
    vec4 lightPosition;
};

layout (std140, binding = 1) uniform ShadowBlock {
    StaticPointlightData lights[4];
    int activeLightCount;
} ub;

out vec4 FragPos;
flat out int LightIndex; // we needed this for proper fragment shader.

void main()
{
    unsigned int numFaces = 6 * ub.activeLightCount;  

    for ( int l = 0 ; l < ub.activeLightCount ; l++ )
    {
        for(int face = 0; face < 6; ++face)
        {
            gl_Layer = l * 6 + face;
            LightIndex = l;
            for(int i = 0; i < 3; ++i)
            {
                FragPos = gl_in[i].gl_Position;
                gl_Position = ub.lights[l].shadowTransforms[face] * FragPos;
                EmitVertex();
            }    
            EndPrimitive();
        }
    }
}  

#endif

#ifdef FRAGMENT_SHADER

in vec4 FragPos;
flat in int LightIndex; // Needed for memory saving

struct StaticPointlightData
{
    mat4 shadowTransforms[6];
    vec4 lightPosition;
};

layout (std140, binding = 1) uniform ShadowBlock {
    StaticPointlightData lights[4];
    int activeLightCount;
} ub;

uniform float far_plane;

void main()
{
    vec3 lightPosition = ub.lights[LightIndex].lightPosition.xyz;

    float lightDistance = length(FragPos.xyz - lightPosition);
    lightDistance = lightDistance / far_plane;
    gl_FragDepth = lightDistance;
}  
#endif
#endif