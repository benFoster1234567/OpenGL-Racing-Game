//required steps:
/*
* 
*/

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
		std::unique_ptr<Game::MainGame> game{};
	public:

		AssetManager assetManager{};
		AssetPipeline assetPipeline{};
		KeyboardBridge inputHandler{};
		
		EngineSystem() {
			game = std::make_unique<Game::MainGame>(Game::MainGame(assetManager, inputHandler));
		}
		
		//TODO: make a template func for callbacks for different asset types...
		//I think this would make this code less confusing
		EventDispatcher<std::vector<ShaderData*>> shaderDispatcher{};
		EventDispatcher<std::vector<MeshData*>> meshDispatcher{};

		void dispatchAssets();

		void updateDeltaTime(float dt)
		{
			deltaTime = dt;
		}
		void updateAspect(float a) 
		{ 
			aspect = a;
		}


		void setMouse( double xpos, double ypos);
		void updateInputState();
		void createAssetManager();
		
	};
}