#include "infra/app/Application.h"
#include "infra/engine/ImportFuncs.h"
void Engine::Infra::Application::setupImportCallbacks()
{
	engine.getAssetImporter().registerImportCallback<Core::MeshData>([](const std::string& path, const std::string& name) -> std::unique_ptr<Core::MeshData>
		{
			return std::make_unique<Core::MeshData>(Infra::ImportFuncs::importMeshData(path, name));
		});

}

void Engine::Infra::Application::setupInput()
{

}

void Engine::Infra::Application::updateRenderQueue()
{

}

void Engine::Infra::Application::run()
{


}
