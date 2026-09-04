#define MULTI_LIGHT

#ifdef VERTEX_SHADER

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec4 aTang;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vertexPosition; 
out vec2 texCoord;
out vec3 vTangent;
out vec3 vBitangent;
out vec3 vNormal;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    vertexPosition = worldPos.xyz;    
    texCoord = aTex;

    mat3 normalMatrix = transpose(inverse(mat3(model)));

    vec3 N = normalize(normalMatrix * aNorm);
    vec3 T = normalize(normalMatrix * aTang.xyz);
    
    T = normalize(T - dot(T, N) * N);

    vec3 B = cross(N, T) * aTang.w;

    vTangent   = T;
    vBitangent = B;
    vNormal    = N;

    gl_Position = projection * view * worldPos;
}

#endif
#ifdef FRAGMENT_SHADER
 
#define MAX_LIGHTS 100

struct StaticPointLight {
    vec4 posRad; 
    vec4 color;  
};

#ifdef SINGLE_LIGHT
    uniform samplerCube depthMap;
#endif

#ifdef MULTI_LIGHT
    uniform samplerCubeArray depthMap;
#endif

struct Material 
{
    sampler2D ambient;
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    float shininess;
};
 
layout (std140, binding = 0) uniform LightBlock {
    StaticPointLight lights[MAX_LIGHTS];
    int activeLightCount;
} ub;

#ifdef MULTI_LIGHT

    struct ShadowData
    {
        mat4 shadowTransforms[6];
        vec4 lightPosition;
    };

    layout (std140, binding = 1) uniform ShadowBlock {
        ShadowData lights[4];
        int activeLightCount;
    } ub1;

#endif

uniform mat4 view; 
uniform Material material;
uniform float far_plane;
uniform vec2 uvScale;

#ifdef SINGLE_LIGHT
    uniform vec3 lightPosition;
#endif

in vec3 vertexPosition;
in vec2 texCoord;
in vec3 vTangent;
in vec3 vBitangent;
in vec3 vNormal;

out vec4 FragColor;

float attenuate(float d, float r)
{
    float dOverR = clamp(d / r, 0.0, 1.0);
    float a = 1.0 - dOverR*dOverR*dOverR*dOverR;
    return (a*a) / ((d*d)+1.0);
}

float getClosestDepth(vec3 fragToLight, vec3 offset, float index)
{
    float closestDepth = 0;
    
    #ifdef SINGLE_LIGHT
       
       closestDepth = texture(depthMap, fragToLight + offset).r;

    #endif
    #ifdef MULTI_LIGHT

        closestDepth = texture(depthMap, vec4(fragToLight + offset, index)).r; 

    #endif

    return closestDepth;
}


float shadowCalculation(vec3 fragPos, vec3 lightPos, float index)
{
    vec3 sampleOffsetDirections[20] = vec3[]
    (
       vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
       vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
       vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
       vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
       vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
    );   

    vec3 fragToLight = fragPos - lightPos; 
    float currentDepth = length(fragToLight);
    float shadow  = 0.0;
    float bias    = 0.05; 
    int samples = 20;
    
    float diskRadius = 0.05;
    for(int i = 0; i < samples; i++)
    {
        float closestDepth = getClosestDepth(fragToLight, sampleOffsetDirections[i] * diskRadius, index);
        closestDepth *= far_plane; 
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }

    return shadow / float(samples);
}

float getShadow(vec3 fragPos)
{
     float shadow = 0;
    
    #ifdef SINGLE_LIGHT
       
       shadow = shadowCalculation(lightPosition, fragPos, 0);

    #endif
    #ifdef MULTI_LIGHT

        for (int i = 0; i < ub1.activeLightCount; i++)
        {
            vec3 lightPos = ub1.lights[i].lightPosition.xyz;
            float index = float(i);
            shadow += shadowCalculation(fragPos, lightPos, index);
        }

    #endif

    return shadow;
}



void main()
{
    vec2 uTiling = uvScale;
    vec2 tiledUV = texCoord * uTiling;

    vec3 ambientColor  = texture(material.diffuse, tiledUV).rgb * 0.05;
    vec3 diffuseColor  = texture(material.diffuse, tiledUV).rgb;
    vec3 specularColor = texture(material.specular, tiledUV).rgb;

    vec3 normal = normalize(texture(material.normal, tiledUV).xyz * 2.0 - 1.0);

    vec3 T = normalize(vTangent);
    vec3 B = normalize(vBitangent);
    vec3 N = normalize(vNormal);

    mat3 TBN = transpose(mat3(T, B, N));
    
    vec3 cameraPosWorld = inverse(view)[3].xyz;
    vec3 viewDir = normalize(TBN * (cameraPosWorld - vertexPosition));

    vec3 colorOut = vec3(0.0);

    for (int i = 0; i < ub.activeLightCount; i++)
    {
        StaticPointLight light = ub.lights[i];
        vec3 lightPos   = light.posRad.xyz;
        float radius    = light.posRad.w;
        vec3 lightColor = light.color.rgb;
        float intensity = light.color.a; 

        vec3 lightDir = TBN * (lightPos - vertexPosition);
        float distance = length(lightDir);

        float attenuation = attenuate(distance, radius);
        
        vec3 L = normalize(lightDir);
        vec3 E = normalize(viewDir);
        vec3 H = normalize(L + E);

        float Kd = max(dot(normal, L), 0.0);
        float Ks = pow(max(dot(normal, H), 0.0), material.shininess);

        vec3 diffuse  = Kd * diffuseColor * lightColor;
        vec3 specular = Ks * specularColor * lightColor;

        float index = float(i);
        float shadow  = shadowCalculation(vertexPosition, lightPos, index);

        colorOut += (1.0 - shadow) * (diffuse + specular) * attenuation * intensity;
    }
    
    vec3 colorFractional = (colorOut - ( ub1.activeLightCount / ub.activeLightCount) * colorOut ) + (colorOut * getShadow(vertexPosition));

    colorOut += ambientColor;

    FragColor = vec4(colorOut, 1.0);
}
#endif


