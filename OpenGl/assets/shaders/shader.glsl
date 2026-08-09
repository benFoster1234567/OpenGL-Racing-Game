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
 
struct Material 
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 emission;
    float shininess;
};
 
layout (std140) uniform LightBlock {
    StaticPointLight lights[MAX_LIGHTS];
    int activeLightCount;
} ub;

highp float intensity = 1.5;

uniform mat4 view; 
uniform Material material;
 
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

float noise(vec2 co) 
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
    highp vec3 N = normalize(vertexNormal);
    highp mat4 invView = inverse(view);
    highp vec3 cameraPosWorld = invView[3].xyz; 
    highp vec3 E = normalize(cameraPosWorld - vertexPosition);
    highp vec4 ambientProduct = vec4(material.ambient, 1.0);
    highp float shininess = material.shininess;
    highp vec4 fColor = ambientProduct;
     for (int i = 0; i < ub.activeLightCount; i++)
    {
        StaticPointLight curLight = ub.lights[i];
        highp vec3 lightPosWorld = curLight.posRad.xyz;
        highp float radius = curLight.posRad.w;
        highp vec3 fL = lightPosWorld - vertexPosition;
        highp float d2 = sqDist(lightPosWorld, vertexPosition);
        highp float d = sqrt(d2);
        highp vec3 L = fL / (d + 0.0001); 
        highp vec3 H = normalize(L + E);
        highp float Kd = max(dot(L, N), 0.0);
        highp vec4 diffuse = Kd * curLight.color * vec4(material.diffuse, 1.0);
        highp float Ks = pow(max(dot(N, H), 0.0), shininess);
        highp vec4 specular = Ks * curLight.color * vec4(material.specular, 1.0);
        if (dot(L, N) <= 0.0) specular = vec4(0.0);

        float attenuation = 1.0 / (d2 + 1.0); 
        
        float window = clamp(1.0 - (d / radius), 0.0, 1.0);
        window = window * window;
        
        fColor.xyz += intensity * attenuation * window * (diffuse.xyz + specular.xyz);
    }

    fColor.w = 1.0;
    fColor += (noise(gl_FragCoord.xy) - 0.5) / 255.0;

    // Maps linear space calculations into 8-bit monitor friendly color space
    //FragColor.rgb = pow(fColor.rgb, vec3(1.0 / 2.2));
    FragColor.rgb = fColor.rgb;
    FragColor.a = 1.0;
}
#endif