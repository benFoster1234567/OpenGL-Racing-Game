#ifdef VERTEX_SHADER

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vertexNormal;   
out vec3 vertexPosition; 
out vec2 texCoord;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    gl_Position = projection * view * worldPos;

    // Transform vertex normal by model orientation matrix
    vertexNormal = mat3(model) * aNorm; 
    vertexPosition = worldPos.xyz;    
    texCoord = aTex;
}

#endif

#ifdef FRAGMENT_SHADER
 
#define MAX_LIGHTS 100
struct StaticPointLight {
    vec4 posRad; 
    vec4 color;  
};
 
struct Material 
{
    sampler2D ambient;
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    float shininess;
};
 
layout (std140) uniform LightBlock {
    StaticPointLight lights[MAX_LIGHTS];
    int activeLightCount;
} ub;

uniform mat4 view; 
uniform Material material;
 
in vec3 vertexNormal;
in vec3 vertexPosition;
in vec2 texCoord;

out vec4 FragColor;

float attenuate(float d, float r)
{
    float a = 1 - pow(d/r, 4);
    return (a*a) / ((d*d)+1);
}

void main()
{
    vec2 uTiling = vec2(10.0,10.0);
    vec2 tiledUV = texCoord * uTiling;

    vec3 ambientColor  = texture(material.ambient, tiledUV).rgb;
    vec3 diffuseColor  = texture(material.diffuse, tiledUV).rgb;
    vec3 specularColor = texture(material.specular,tiledUV).rgb;
    
    vec3 normal = normalize(vertexNormal);

    mat4 invView = inverse(view);
    vec3 cameraPosWorld = invView[3].xyz;
    vec3 viewDir = normalize(cameraPosWorld - vertexPosition);

    vec3 colorOut = ambientColor * 0.1; // Base ambient contribution

    for (int i = 0; i < ub.activeLightCount; i++)
    {
        StaticPointLight light = ub.lights[i];
        vec3 lightPos   = light.posRad.xyz;
        float radius    = light.posRad.w;
        vec3 lightColor = light.color.rgb;

        vec3 lightDir = lightPos - vertexPosition;
        
        float distance = length(lightDir);

        float attenuation = attenuate(distance, radius);
        
        vec3 L = normalize(lightDir);
        vec3 E = normalize(viewDir);
        vec3 H = normalize(L + E);

        float Kd = max(dot(normal, L), 0.0);
        float Ks = pow(max(dot(normal, H), 0.0), material.shininess);

        vec3 diffuse = Kd * diffuseColor;
        vec3 specular = Ks * specularColor;

        colorOut += (diffuse + specular) * attenuation;
    }

    FragColor.rgb = pow(colorOut, vec3(1.0));
    FragColor.a = 1.0;
}
#endif