
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

struct PointlightShadowmapData
{
    mat4 shadowTransforms[6];
    int lightIndex;
};

layout (std140, binding = 1) uniform ShadowBlock {
    PointlightShadowmapData shadowSource[4];
    int activeLightCount;
} ub;

out vec4 FragPos;
flat out int LightIndex; // we needed this for proper fragment shader.

void main()
{
    int numFaces = 6 * ub.activeLightCount;  

    for ( int l = 0 ; l < ub.activeLightCount ; l++ )
    {
        for(int face = 0; face < 6; ++face)
        {
            gl_Layer = l * 6 + face;
            LightIndex = l;
            for(int i = 0; i < 3; ++i)
            {
                FragPos = gl_in[i].gl_Position;
                gl_Position = ub.shadowSource[l].shadowTransforms[face] * FragPos;
                EmitVertex();
            }    
            EndPrimitive();
        }
    }
}  

#endif

#ifdef FRAGMENT_SHADER
#define MAX_LIGHTS 100
in vec4 FragPos;
flat in int LightIndex; // Needed for memory saving


uniform float far_plane;

struct StaticPointLight {
    vec4 posRad; 
    vec4 color;  
};

layout (std140, binding = 0) uniform LightBlock {
    StaticPointLight lights[MAX_LIGHTS];
    int activeLightCount;
} ubLight;

struct PointlightShadowmapData
{
    mat4 shadowTransforms[6];
    int lightIndex;
};

layout (std140, binding = 1) uniform ShadowBlock {
    PointlightShadowmapData shadowSource[4];
    int activeLightCount;
} ubShadow;


void main()
{
    int lightIdx = ubShadow.shadowSource[LightIndex].lightIndex;
    vec3 lightPosition = ubLight.lights[lightIdx].posRad.xyz;

    float lightDistance = length(FragPos.xyz - lightPosition);
    lightDistance = lightDistance / far_plane;
    gl_FragDepth = lightDistance;
}  
#endif
