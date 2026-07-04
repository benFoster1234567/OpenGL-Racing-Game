#include "core/engine/Engine.h"
#include <glm/ext/quaternion_trigonometric.hpp>


void Engine::Core::EngineSystem::createAssetManager()
{
	int initialShaders = assetManager.shaderMap.size();
	int initialMeshes = assetManager.meshMap.size();
	int initialTextures = assetManager.textureMap.size();

	assetPipeline.refreshAssetManager(assetManager);

	int addedShaders = assetManager.shaderMap.size() - initialShaders;
	int addedMeshes = assetManager.meshMap.size() - initialMeshes;
	int addedTextures = assetManager.textureMap.size() - initialTextures;

	if (addedShaders == 0 && addedMeshes == 0 && addedTextures == 0)
	{
		throw std::runtime_error("Nothing was imported during asset refresh!");
	}
}

void Engine::Core::EngineSystem::fillEntityRenderList(std::vector<EntityRenderCommand>& entityListOut)
{
	ECS::Entity player = gameObjects.player;
	ECS::Entity camera = gameObjects.camera;

	ECS::CameraComponent& cameraComp = ecsManager.components.getComponent<ECS::CameraComponent>(camera);
	ECS::TransformComponent& playerTransform = ecsManager.components.getComponent<ECS::TransformComponent>(player);
	ECS::MeshComponent& playerMesh = ecsManager.components.getComponent<ECS::MeshComponent>(player);
	ECS::ShaderComponent& playerShader = ecsManager.components.getComponent<ECS::ShaderComponent>(player);

	entityListOut.emplace_back(EntityRenderCommand{
		.view = cameraComp.view,
		.projection = cameraComp.projection,
		.modelTransform = playerTransform.getTransformMatrix(),
		.shader = playerShader.shaderData,
		.mesh = playerMesh.meshData,
	});

}

//this is where we program our game ->
void Engine::Core::EngineSystem::setupEcs(glm::mat4 view, glm::mat4 projection)
{
	MeshData* meshData = nullptr;
	ShaderData* shaderData = nullptr;
	gameObjects.player = ecsManager.createEntity();
	gameObjects.camera = ecsManager.createEntity();
	
	ECS::CameraComponent cameraComp{};
	cameraComp.view = view;
	cameraComp.projection = projection;

	assetManager.getMesh(meshData, "bunny");
	ECS::MeshComponent meshComp{meshData};

	assetManager.getShader(shaderData, "shader");
	ECS::ShaderComponent shaderComp{ shaderData };

	ecsManager.addComponent(gameObjects.camera, cameraComp);
	ecsManager.addComponent(gameObjects.player, meshComp);
	ecsManager.addComponent(gameObjects.player, shaderComp);
	ecsManager.addComponent(gameObjects.player, ECS::TransformComponent{});

	auto rotateQuad = [&](float degrees, glm::vec3 axis) -> glm::quat
	{
		float radians = glm::radians(degrees);
		glm::quat q = glm::angleAxis(radians, glm::normalize(axis));
		return q;
	};

	ecsManager.addSystem(gameObjects.player, [&](ECS::Entity entity, ECS::ComponentRegistry& components)
		{
			auto deltaTime = gameObjects.deltaTime;
			auto& transform = components.getComponent<ECS::TransformComponent>(entity);
			transform.rotation *= rotateQuad(deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
		});

	ecsManager.addSystem(gameObjects.camera, [&](ECS::Entity entity, ECS::ComponentRegistry& components)
		{
			auto& cameraInfo = components.getComponent<ECS::CameraComponent>(entity);
			cameraInfo.projection = glm::perspective(glm::radians(45.0f), gameObjects.aspect, 0.1f, 100.0f);
		});
}

void Engine::Core::EngineSystem::updateComponents()
{
	ecsManager.updateSystems();
}

void Engine::Core::EngineSystem::dispatchAssets()
{
	std::vector<ShaderData*> shaderList{};
	std::vector<MeshData*> meshList{};
	
	assetManager.shaderList(shaderList);
	if (shaderList.empty()) std::cout << "shader list empty!";
	else shaderDispatcher.invoke(shaderList);

	assetManager.meshList(meshList);
	if (meshList.empty()) std::cout << "mesh list empty!";
	else meshDispatcher.invoke(meshList);

}
