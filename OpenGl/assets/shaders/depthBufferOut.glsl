#ifdef VERTEX_SHADER
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTex;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

out vec2 TexCoords;

void main()
{
    TexCoords = aTex;
    gl_Position = vec4(aPos * 2.0, 0.0, 1.0);
}  

#endif

#ifdef FRAGMENT_SHADER

out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D depthMap;

void main()
{             
    float depthValue = texture(depthMap, TexCoords).r;
    FragColor = vec4(vec3(depthValue), 1.0);
} 

#endif