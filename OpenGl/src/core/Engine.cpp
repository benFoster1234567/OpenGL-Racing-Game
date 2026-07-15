#include "core/engine/Engine.h"
#include <glm/ext/quaternion_trigonometric.hpp>


Engine::Core::EntityRenderCommand Engine::Core::EngineSystem::createRenderCommand(ECS::Entity entity, ECS::Entity camera)
{
	ECS::CameraComponent& cameraComp = ecsManager.components.getComponent<ECS::CameraComponent>(camera);
	ECS::TransformComponent& transform = ecsManager.components.getComponent<ECS::TransformComponent>(entity);
	ECS::MeshComponent& mesh = ecsManager.components.getComponent<ECS::MeshComponent>(entity);
	ECS::ShaderComponent& shader = ecsManager.components.getComponent<ECS::ShaderComponent>(entity);

	return EntityRenderCommand{
		.view = cameraComp.view,
		.projection = cameraComp.projection,
		.modelTransform = transform.getTransformMatrix(),
		.shader = shader.shaderData,
		.mesh = mesh.meshData,
	};
}

void Engine::Core::EngineSystem::onKey(KeyCode k, bool pressed)
{
	inputHandler.setKey(k, pressed);
}

void Engine::Core::EngineSystem::updateInputState()
{
	//std::cout << "updating input state\n";
	inputHandler.updateKeyboard();
}

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
	ECS::Entity grid = gameObjects.grid;

	entityListOut.emplace_back(createRenderCommand(player, camera));
	entityListOut.emplace_back(createRenderCommand(grid, camera));
}

//this is where we program our game - use system functions
void Engine::Core::EngineSystem::setupEcs(glm::mat4 view, glm::mat4 projection)
{
	MeshData* meshData = nullptr;
	MeshData* gridData = nullptr;

	ShaderData* shaderData = nullptr;
	gameObjects.player = ecsManager.createEntity();
	gameObjects.camera = ecsManager.createEntity();
	gameObjects.grid = ecsManager.createEntity();

	ECS::CameraComponent cameraComp{};
	cameraComp.view = view;
	cameraComp.projection = projection;

	assetManager.getMesh(meshData, "bunny");
	ECS::MeshComponent meshComp{ meshData };

	assetManager.getShader(shaderData, "shader");
	ECS::ShaderComponent shaderComp{ shaderData };

	assetManager.getMesh(gridData, "grid");
	ECS::MeshComponent gridComp{ gridData };

	ECS::TransformComponent transform{};
	transform.position.y = -1.0f;

	ecsManager.addComponent(gameObjects.camera, cameraComp);
	ecsManager.addComponent(gameObjects.player, meshComp);
	ecsManager.addComponent(gameObjects.player, shaderComp);
	ecsManager.addComponent(gameObjects.player, ECS::TransformComponent{});
	ecsManager.addComponent(gameObjects.grid, transform);
	ecsManager.addComponent(gameObjects.grid, gridComp);
	ecsManager.addComponent(gameObjects.grid, shaderComp);

	auto rotateQuad = [&](float degrees, glm::vec3 axis) -> glm::quat
		{
			float radians = glm::radians(degrees);
			glm::quat q = glm::angleAxis(radians, glm::normalize(axis));
			return q;
		};

	auto rotateObject = ecsManager.systems.registerSystem([&](ECS::Entity entity, ECS::ComponentRegistry& components)
		{
			auto deltaTime = gameObjects.deltaTime;
			auto& transform = components.getComponent<ECS::TransformComponent>(entity);
			transform.rotation *= rotateQuad(deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
		});

	auto rotateCameraByKeys = ecsManager.systems.registerSystem([&](ECS::Entity entity, ECS::ComponentRegistry& components)
		{
			auto& cameraInfo = components.getComponent<ECS::CameraComponent>(entity);
			cameraInfo.projection = glm::perspective(glm::radians(45.0f), gameObjects.aspect, 0.1f, 100.0f);

			if (inputHandler.keyHeld(int(KeyCode::A)))
			{
				cameraInfo.rotate(-30.0f* gameObjects.deltaTime, { 0,1,0 });
			}
			else if (inputHandler.keyHeld(int(KeyCode::D)))
			{
				cameraInfo.rotate(30.0f * gameObjects.deltaTime, { 0,1,0 });
			}
		});

auto rotateCameraByMouse = ecsManager.systems.registerSystem([&](ECS::Entity entity, ECS::ComponentRegistry& components)   
	{    
		std::cout << "moving mouse!" << inputHandler.inputState.mousePos.x << ", " << inputHandler.inputState.mousePos.y << "\n";

		auto mouseDelta = inputHandler.inputState.mouseDelta;
		if (mouseDelta.x == 0.0f && mouseDelta.y == 0.0f) return;

		auto& camera = components.getComponent<ECS::CameraComponent>(entity);
    
		// 1. Extract the current Right and Up vectors directly from the View matrix columns/rows
		// For a standard row-major or column-major view matrix, these are the camera-space axes in world coordinates.
		glm::vec3 cameraRight = glm::vec3(camera.view[0][0], camera.view[1][0], camera.view[2][0]); 
		glm::vec3 worldUp     = glm::vec3(0.0f, 1.0f, 0.0f); // Keep a fixed world up to prevent roll

		// 2. Calculate the incremental rotation quaternions based on mouse movement
		float sensitivity = 0.001f; // Adjust this value to fit your engine's scaling
		glm::quat yawRotation   = glm::angleAxis(-mouseDelta.x * sensitivity, worldUp);
		glm::quat pitchRotation = glm::angleAxis(-mouseDelta.y * sensitivity, cameraRight);
		glm::quat combinedRotation = yawRotation * pitchRotation;

		// 3. Extract the current camera position from the inverse view matrix
		glm::mat4 invView = glm::inverse(camera.view);
		glm::vec3 position = glm::vec3(invView[3]);

		// 4. Extract and rotate the current target orientation forward vector
		glm::vec3 forward = -glm::vec3(camera.view[0][2], camera.view[1][2], camera.view[2][2]);
		forward = combinedRotation * forward;

		// 5. Rebuild the LookAt View matrix with the new forward direction
		camera.view = glm::lookAt(position, position + forward, worldUp);
	});

	ecsManager.attachSystem(gameObjects.camera, rotateCameraByMouse);
	ecsManager.attachSystem(gameObjects.player, rotateObject);
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

void Engine::Core::InputState::updateKeyState(std::bitset<512> inputData)
{
	previousFrameInputData = currentFrameInputData;
	currentFrameInputData = inputData;
}

void Engine::Core::InputState::updateMouseState(glm::vec2 currentMouse)
{
	glm::vec2 lastMouse = mousePos;
	mousePos = currentMouse;

	mouseDelta = currentMouse - lastMouse;

}
