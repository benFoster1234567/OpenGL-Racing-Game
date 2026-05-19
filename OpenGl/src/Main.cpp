#ifndef ENABLE_TESTS

#ifndef BUILD
#define BUILD
#include <iostream>

#include "core/ecs/Entity.h"
#include <GL/glew.h>
#define GLAD_GL_IMPLEMENTATION
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> // use gtc header for glm::ortho

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>


static const char* 
vertex_shader_text =
"#version 330\n"
"uniform mat4 MVP;\n"
"in vec3 vCol;\n"
"in vec2 vPos;\n"
"out vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";

static const char* fragment_shader_text =
"#version 330\n"
"in vec3 color;\n"
"out vec4 fragment;\n"
"void main()\n"
"{\n"
"    fragment = vec4(color, 1.0);\n"
"}\n";

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    

}

int main(void)
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL Triangle", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE; // Enable modern OpenGL techniques
    if (glewInit() != GLEW_OK) {
        // Handle error
        exit(1);
    }
    glfwSwapInterval(1);

    // NOTE: OpenGL error checks have been omitted for brevity

    //GLuint vertex_buffer;
    //glGenBuffers(1, &vertex_buffer);
    //glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);

    const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);

    const GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    const GLint mvp_location = glGetUniformLocation(program, "MVP");
    const GLint vpos_location = glGetAttribLocation(program, "vPos");
    const GLint vcol_location = glGetAttribLocation(program, "vCol");

    //GLuint vertex_array;
    //glGenVertexArrays(1, &vertex_array);
    //glBindVertexArray(vertex_array);
    //glEnableVertexAttribArray(vpos_location);
    //glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
    //    sizeof(Vertex), (void*)offsetof(Vertex, pos));
    //glEnableVertexAttribArray(vcol_location);
    //glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
    //    sizeof(Vertex), (void*)offsetof(Vertex, col));

    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        const float ratio = width / (float)height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 m, p, mvp;
        m = glm::mat4(1);
        p = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
        mvp = p * m; // compute MVP before uploading

        glUseProgram(program);
        //glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (GLfloat*)&mvp[0]);
        //glBindVertexArray(vertex_array);
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}


#endif
#endif