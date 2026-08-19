#pragma once
#include <GL/glew.h>
namespace Engine::Infra
{
	class Quad
	{
	private:
		GLuint VAO{};
		GLuint VBO{};

	public:
		Quad()
		{

		}
        void create()  
        {
            float vertices[] = {
                -0.5f,  0.5f,    0.0f, 1.0f,
                -0.5f, -0.5f,    0.0f, 0.0f,
                 0.5f, -0.5f,    1.0f, 0.0f,

                -0.5f,  0.5f,    0.0f, 1.0f,
                 0.5f, -0.5f,    1.0f, 0.0f,
                 0.5f,  0.5f,    1.0f, 1.0f
            };

            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);

            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        void render(GLuint shaderProgram) {
            glUseProgram(shaderProgram);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }

	};


}