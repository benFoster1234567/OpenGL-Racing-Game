#pragma once
#include "MeshData.h"

class GpuMesh
{
private:
	GLuint VBO = 0, VAO = 0, EBO = 0;
public:
	std::shared_ptr<MeshData> meshData;
	void genBuffers();
	void draw();
};