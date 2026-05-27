#pragma once
#include<glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

struct CameraInfo
{
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
};