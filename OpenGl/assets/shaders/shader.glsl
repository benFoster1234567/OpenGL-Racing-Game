#ifdef VERTEX_SHADER

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 normOut;
out vec3 vertexNormal;   
out vec3 vertexPosition; 

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    gl_Position = projection * view * worldPos;

    normOut = vec4(aNorm, 1.0);
    vertexNormal = mat3(model) * aNorm; 
    vertexPosition = worldPos.xyz;      
}

#endif

#ifdef FRAGMENT_SHADER

#define MAX_LIGHTS 100
struct StaticPointLight {
    vec4 posRad; 
    vec4 color;  
};

layout (std140) uniform LightBlock {
    StaticPointLight lights[MAX_LIGHTS];
    int activeLightCount;
} ub;

uniform mat4 view; 

in vec4 normOut;    
in vec3 vertexNormal;
in vec3 vertexPosition;

out vec4 FragColor;

float sqDist(vec3 a, vec3 b)
{
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float dz = a.z - b.z;
    return dx*dx + dy*dy + dz*dz;
}

void main()
{
    vec3 N = normalize(vertexNormal);

    mat4 invView = inverse(view);
    vec3 cameraPosWorld = invView[3].xyz; 

    vec3 E = normalize(cameraPosWorld - vertexPosition);
    
    vec4 ambientProduct = vec4(0.0, 0.0, 0.0, 1.0);
    float shininess = 10.0;
    vec4 fColor = ambientProduct;

    for (int i = 0; i < ub.activeLightCount; i++)
    {
        StaticPointLight curLight = ub.lights[i];
        vec3 lightPosWorld = ub.lights[i].posRad.xyz;
        float radius = ub.lights[i].posRad.w;

        vec3 fL = lightPosWorld - vertexPosition;
        vec3 L = normalize(fL);

        vec3 H = normalize(L + E);

        float Kd = max(dot(L, N), 0.0);
        vec4 diffuseProduct = ub.lights[i].color;
        vec4 diffuse = Kd * diffuseProduct;

        float Ks = pow(max(dot(N, H), 0.0), shininess);
        vec4 specularProduct = vec4(1.0, 1.0, 1.0, 1.0);
        vec4 specular = Ks * specularProduct;

        if (dot(L, N) <= 0.0) {
            specular = vec4(0.0);
        }

        
        float invDistSquared = 1/sqDist(curLight.posRad.xyz, vertexPosition);

        fColor.xyz += invDistSquared*(diffuse.xyz + specular.xyz);
    }

    fColor.w = 1.0;
    FragColor = fColor;
}

#endif