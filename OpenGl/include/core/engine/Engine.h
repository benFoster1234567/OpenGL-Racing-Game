
#pragma once

#include "core/assets/AssetPipeline.h"
#include "core/assets/AssetManager.h"
#include "core/input/KeyboardInput.h"
#include "core/input/Keys.h"
#include "core/ecs/ECS.h"
#include "core/Game/Game.h"
namespace Engine::Core
{
	constexpr int MAX_ENTITIES{ 100 };
	struct EntityRenderCommand
	{
		glm::mat4 view;
		glm::mat4 projection;
		glm::mat4 modelTransform;
		ShaderData* shader;
		MeshData* mesh;
	};

	struct GameObjects
	{

		ECS::Entity player{};
		ECS::Entity camera{};
		ECS::Entity grid{};

		float aspect{ 1.0f };
		float deltaTime{ 1.0f };
	};

	class EngineSystem
	{
	private:
		float deltaTime{};
		float aspect{};
		
		Game::TestScene game;
	
	public:

		AssetManager assetManager;
		AssetPipeline assetPipeline;
		InputBridge inputHandler;
	
		EngineSystem() : assetManager{}, assetPipeline{}, inputHandler{}, game(assetManager, inputHandler)
		{
		}

		void setUpGame() { game.setup(); }

		void fillStaticLightVector(std::vector<ECS::StaticPointLightRendererData>& lights);

		EventDispatcher<std::vector<ShaderData*>> shaderDispatcher{};
		EventDispatcher<std::vector<MeshData*>> meshDispatcher{};
		EventDispatcher<std::vector<TextureData*>> textureDispatcher{};

		void createAssetManager();
		void publishAssets();
		void updateDeltaTime(float dt) { deltaTime = dt; }
		void updateAspect(float a) { aspect = a; }
		void updateMouse( double xpos, double ypos);
		void zeroMouse();
		void updateGame() { game.update(aspect, inputHandler.mouseState, deltaTime); }
		void updateInputState();
		void setTextureUvTiling(std::string textureName, glm::vec2 uvTiling);

	};
}