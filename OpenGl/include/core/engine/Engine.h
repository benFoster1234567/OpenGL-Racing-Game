/*
* TODO: I need to ensure proper cache locality when passing data from the Core layer to the Infra Layer
*/


#pragma once
#include "core/assets/AssetPipeline.h"
#include "core/assets/AssetManager.h"
#include "core/input/KeyboardInput.h"
#include "core/input/Keys.h"
#include "../ecs/systems/PhysicsCommandsAndEvents.h"
#include "../ecs/Game.h"
//#include "core/ecs/ECS.h"
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
		float m_deltaTime{};
		float m_aspect{};


	public:
		Game::TestScene m_game;

		AssetManager m_assetManager;
		AssetPipeline m_assetPipeline;
		InputBridge m_inputHandler;

		EngineSystem()
			: m_assetManager{}, m_assetPipeline{}, m_inputHandler{}, m_game(m_assetManager, m_inputHandler)
		{
		}

		void setUpGame() { m_game.setup(); }

		void fillStaticLightVector(std::vector<ECS::StaticPointLightRendererData>& lights);

		std::vector<ECS::StaticPointLightRendererData> getShadowCastingPointlights()
		{
			return m_game.getShadowCastingPointlights();
		}

		EventDispatcher<std::vector<ShaderData*>> m_shaderDispatcher{};
		EventDispatcher<std::vector<MeshData*>> m_meshDispatcher{};
		EventDispatcher<std::vector<TextureData*>> m_textureDispatcher{};

		ECS::Coordinator& getCoordinator() { return m_game.m_coordinator; }
		void createAssetManager();
		void updateDeltaTime(float dt) { m_deltaTime = dt; }
		void updateAspect(float a) { m_aspect = a; }
		void updateMouse(double xpos, double ypos);
		void zeroMouse();
		void updateGame() { m_game.update(m_aspect, m_inputHandler.m_mouseState, m_deltaTime); }
		void updatePhysics() { m_game.updatePhysics(m_deltaTime); }

		ECS::PhysicsEngineCommandBuffer& getPhysicsCommandQueue()
		{
			return m_game.getPhysicsEngineCommands();
		}

		void pollPhysicsEvents(const std::vector<ECS::PhysicsEvent>& eventQueue);

		void updateInputState();
		void setTextureUvTiling(std::string textureName, glm::vec2 uvTiling);


	};
}