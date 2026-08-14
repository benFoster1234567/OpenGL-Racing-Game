#ifdef VERTEX_SHADER

#define MAX_LIGHTS 100

struct StaticPointLight {
    vec4 posRad; 
    vec4 color;  
};

layout (std140, binding = 0) uniform LightBlock {
    StaticPointLight lights[MAX_LIGHTS];
    int activeLightCount;
} ub;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 pos = ub.lights[gl_VertexID].posRad;

    gl_Position = projection * view * vec4(pos.xyz, 1.0);
    gl_PointSize = 12.0; 
}

#endif

#ifdef FRAGMENT_SHADER

out vec4 FragColor;

void main()
{
    vec2 circCoord = gl_PointCoord - vec2(0.5);
    if (dot(circCoord, circCoord) > 0.25) {
        discard; // Discards pixels outside circle radius
    }

    FragColor = vec4(1,1,1,1);
}

#endif