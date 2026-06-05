#include "core/engine/Engine.h"

Engine::Core::AssetPipeline& Engine::Core::Engine::getAssetImporter()
{
	return assetPipeline;
}

void Engine::Core::Engine::createAssetManager()
{
	assetPipeline.loadAssetManager(assetManager);
}
