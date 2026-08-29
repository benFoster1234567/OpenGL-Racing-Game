#pragma once

#include <vector>
#include <array>
#include <algorithm>
#include <cstring>
#include "GL/glew.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "SparseSet.h"

namespace Engine::Infra
{
	struct StaticPointLightResource
	{
		glm::vec3 position{ 0.0f };
		glm::vec3 color{ 0.0f };
		float radius{ 0.0f };
		float intensity{ 1.0f };
	};

	struct PointlightShadowmapData
	{
		glm::mat4 shadowTransforms[6];
		glm::vec4 lightPosition;
	};

	struct ShadowBlock
	{
		PointlightShadowmapData lights[16];
		int activeLightCount{ 0 };
		int padding[3]{ 0, 0, 0 };
	};

	struct StaticPointLight
	{
		glm::vec4 posRad{ 0.0f, 0.0f, 0.0f, 10.0f };
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
	};

	struct LightBlock
	{
		StaticPointLight lights[100];
		int activeLightCount{ 0 };
		int padding[3]{ 0, 0, 0 };
	};

	class PointlightLoader
	{
	private:
		static constexpr size_t MAX_SHADOW_SRC = 16;
		static constexpr GLuint LIGHT_BINDING_POINT = 0;
		static constexpr GLuint SHADOW_BINDING_POINT = 1;
		static constexpr uint32_t MAX_LIGHTS = 100;

		GLuint emptyVAO{ 0 };
		GLuint pointlightUBO{ 0 };
		GLuint pointShadowmapUBO{ 0 };

		std::vector<StaticPointLightResource> shadowCastingPointlights{};
		std::vector<StaticPointLightResource> pointlights{};

		int activeLightCount{};

		ShadowBlock buildShadowBlock(const std::vector<StaticPointLightResource>& shadowCastedLights, float nnear, float ffar)
		{
			ShadowBlock shadowBlock{};
			shadowCastingPointlights = shadowCastedLights;

			constexpr float aspect = 1.0f; // 1024 / 1024
			const glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, nnear, ffar);

			size_t activeCount = (std::min)(shadowCastedLights.size(), MAX_SHADOW_SRC);

			for (size_t i = 0; i < activeCount; ++i)
			{
				const glm::vec3& lp = shadowCastedLights[i].position;
				PointlightShadowmapData& data = shadowBlock.lights[i];

				data.shadowTransforms[0] = shadowProj * glm::lookAt(lp, lp + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
				data.shadowTransforms[1] = shadowProj * glm::lookAt(lp, lp + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
				data.shadowTransforms[2] = shadowProj * glm::lookAt(lp, lp + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
				data.shadowTransforms[3] = shadowProj * glm::lookAt(lp, lp + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0));
				data.shadowTransforms[4] = shadowProj * glm::lookAt(lp, lp + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0));
				data.shadowTransforms[5] = shadowProj * glm::lookAt(lp, lp + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0));

				data.lightPosition = glm::vec4(lp, 1.0f);
			}

			shadowBlock.activeLightCount = static_cast<int>(activeCount);
			return shadowBlock;
		}

	public:
		PointlightLoader() = default;

		~PointlightLoader()
		{
			if (emptyVAO) glDeleteVertexArrays(1, &emptyVAO);
			if (pointlightUBO) glDeleteBuffers(1, &pointlightUBO);
			if (pointShadowmapUBO) glDeleteBuffers(1, &pointShadowmapUBO);
		}

		PointlightLoader(const PointlightLoader&) = delete;
		PointlightLoader& operator=(const PointlightLoader&) = delete;

		PointlightLoader(PointlightLoader&& other) noexcept
			: emptyVAO(std::exchange(other.emptyVAO, 0)),
			pointlightUBO(std::exchange(other.pointlightUBO, 0)),
			pointShadowmapUBO(std::exchange(other.pointShadowmapUBO, 0)) {
		}

		GLuint getPointlightUbo()
		{
			return pointlightUBO;
		}

		int getActiveLightCount() const
		{
			return activeLightCount;
		}

		void bindLightBufferBase()
		{
			glBindBufferBase(GL_UNIFORM_BUFFER, LIGHT_BINDING_POINT, pointlightUBO);

		}

		void genEmptyVao()
		{
			if (emptyVAO == 0) glGenVertexArrays(1, &emptyVAO);
		}

		void loadStaticPointlights(const std::vector<StaticPointLightResource>& staticLights)
		{
			pointlights = staticLights;
			glEnable(GL_PROGRAM_POINT_SIZE);

			LightBlock uboData{};
			size_t lightsToCopy = (std::min)(pointlights.size(), static_cast<size_t>(MAX_LIGHTS));

			for (size_t i = 0; i < lightsToCopy; ++i)
			{
				const auto& lightCpu = pointlights[i];
				uboData.lights[i] = StaticPointLight{
					.posRad = {lightCpu.position, lightCpu.radius},
					.color = {lightCpu.color, lightCpu.intensity}
				};
			}
			uboData.activeLightCount = static_cast<int>(lightsToCopy);
			activeLightCount = uboData.activeLightCount;

			if (pointlightUBO == 0)
			{
				glGenBuffers(1, &pointlightUBO);
			}

			glBindBuffer(GL_UNIFORM_BUFFER, pointlightUBO);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(LightBlock), &uboData, GL_STATIC_DRAW);
			glBindBufferBase(GL_UNIFORM_BUFFER, LIGHT_BINDING_POINT, pointlightUBO);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

		void loadShadowCastedPointlights(const std::vector<StaticPointLightResource>& shadowCastedLights, float nnear, float ffar)
		{
			ShadowBlock shadowBlock = buildShadowBlock(shadowCastedLights, nnear, ffar);

			if (pointShadowmapUBO == 0)
			{
				glGenBuffers(1, &pointShadowmapUBO);
			}

			glBindBuffer(GL_UNIFORM_BUFFER, pointShadowmapUBO);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(ShadowBlock), &shadowBlock, GL_DYNAMIC_DRAW);
			glBindBufferBase(GL_UNIFORM_BUFFER, SHADOW_BINDING_POINT, pointShadowmapUBO);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

		void updateShadowPoints(const std::vector<StaticPointLightResource>& shadowCastedLights, float nnear, float ffar)
		{
			ShadowBlock shadowBlock = buildShadowBlock(shadowCastedLights, nnear, ffar);

			glBindBuffer(GL_UNIFORM_BUFFER, pointShadowmapUBO);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ShadowBlock), &shadowBlock);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

		bool bindShadowBlockToShader(GLuint shaderId, const char* blockName = "ShadowBlock")
		{
			GLuint blockIndex = glGetUniformBlockIndex(shaderId, blockName);
			bool isValid = (blockIndex != GL_INVALID_INDEX);

			if (isValid)
			{
				glUniformBlockBinding(shaderId, blockIndex, SHADOW_BINDING_POINT);
			}

			return isValid;
		}

		bool bindLightBlockToShader(GLuint shaderId, const char* blockName = "LightBlock")
		{
			GLuint blockIndex = glGetUniformBlockIndex(shaderId, blockName);
			bool isValid = (blockIndex != GL_INVALID_INDEX);

			if (isValid)
			{
				glUniformBlockBinding(shaderId, blockIndex, LIGHT_BINDING_POINT);
			}

			return isValid;
		}
	};
}