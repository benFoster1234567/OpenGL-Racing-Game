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


uniform samplerCubeArray depthMap;


struct Material 
{
    sampler2D ambient;
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    float shininess;
};
 
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

uniform mat4 view; 
uniform Material material;
uniform float far_plane;
uniform vec2 uvScale;


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
    closestDepth = texture(depthMap, vec4(fragToLight + offset, index)).r; 
    return closestDepth;
}

int getShadowDataIndex(int pointLightIndex) // returns where the shadow data is located in the shadow block
{
    for (int i = 0; i  < 4; i++)
    {
        if (ubShadow.shadowSource[i].lightIndex == pointLightIndex)
        {
            return i;
        }
    }
    return -1;
}

float shadowCalculation(vec3 fragPos, vec3 lightPos, float index, vec3 worldNormal)
{
    int idxInt = int(index);

    float shadowIndex = float(getShadowDataIndex(idxInt));

    if (shadowIndex < 0)
    {
        return 0;
    }

    vec3 sampleOffsetDirections[20] = vec3[]
    (
       vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
       vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
       vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
       vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
       vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
    );   

    float normalBiasAmount = 0.1; // TUNE THIS: Increase if acne persists, decrease if Peter Panning occurs
    vec3 biasedFragPos = fragPos + (worldNormal * normalBiasAmount);

    vec3 fragToLight = biasedFragPos - lightPos; 
    float currentDepth = length(fragToLight);
   
    float depthBias = 0.05; 

    float shadow  = 0.0;
    int samples = 20;
    float diskRadius = 0.05;
    
    for(int i = 0; i < samples; i++)
    {
        float closestDepth = getClosestDepth(fragToLight, sampleOffsetDirections[i] * diskRadius, shadowIndex);
        closestDepth *= far_plane; 
        
        if(currentDepth - depthBias > closestDepth)
            shadow += 1.0;
    }

    return shadow / float(samples);
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

    mat3 TBN_World = mat3(T, B, N);
    vec3 worldNormal = normalize(TBN_World * normal);

    mat3 TBN = transpose(mat3(T, B, N));
    
    vec3 cameraPosWorld = inverse(view)[3].xyz;
    vec3 viewDir = normalize(TBN * (cameraPosWorld - vertexPosition));

    vec3 colorOut = vec3(0.0);

    for (int i = 0; i < ubLight.activeLightCount; i++)
    {
        StaticPointLight light = ubLight.lights[i];
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

        int shadowMatIndex = getShadowDataIndex(i);
        
        float index = float(i);
        float shadow  = shadowCalculation(vertexPosition, lightPos, index, worldNormal);

        colorOut += (1.0 - shadow) * (diffuse + specular) * attenuation * intensity;
    }
    


    colorOut += ambientColor;

    FragColor = vec4(colorOut, 1.0);
}
#endif


