
#ifdef VERTEX_SHADER

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNorm;

out vec4 vertexColor;

uniform mat4 Model;
uniform mat4 VP;

void main() // FIXED: Added parentheses
{
    // FIXED: Capitalized P and corrected matrix transformation order
    gl_Position = VP * Model * vec4(aPos, 1.0); 
    vertexColor = vec4(0.0, 0.0, 0.0, 1.0);
}

#endif

#ifdef FRAGMENT_SHADER

out vec4 FragColor;
  
in vec4 vertexColor;

void main()
{
    FragColor = vertexColor;
} 

#endif