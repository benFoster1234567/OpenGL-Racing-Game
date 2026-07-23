//required steps:
/*
* bind import callbacks
* import assets/create assetManager
* setup ecs systems
* subscribe to RenderDispatcher system so that the render commands are created after every iteration
* in the loop:
* call update aspect
* call update mouse
* call updateSystems - this will update the components and send the render data to the renderer for each entity bound.
* 
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
		Game::MainGame game;
	public:

		AssetManager assetManager;
		AssetPipeline assetPipeline;
		InputBridge inputHandler;
	
		EngineSystem() : assetManager{}, assetPipeline{}, inputHandler{}, game(assetManager, inputHandler)
		{
		}

		void setUpGame()
		{
			game.setup();
		}
		
		EventDispatcher<std::vector<ShaderData*>> shaderDispatcher{};
		EventDispatcher<std::vector<MeshData*>> meshDispatcher{};

		void createAssetManager();
		void publishAssets();

		void updateDeltaTime(float dt)
		{
			deltaTime = dt;
		}

		void updateAspect(float a) 
		{ 
			aspect = a;
		}


		void updateMouse( double xpos, double ypos);

		void zeroMouse();

		void updateGame()
		{
			game.update(aspect, inputHandler.mouseState);
		}

		void updateInputState();
		
	};
}