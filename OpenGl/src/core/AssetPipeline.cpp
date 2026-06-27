#include "core/assets/AssetManager.h"
#include "core/assets/AssetPipeline.h"

void Engine::Core::AssetPipeline::processCommand(ImportCommand cmd, AssetManager& am)
{
	auto it = import.find(cmd.typeId);
	if (it != import.end())
	{
		am.addAsset(cmd.assetName, it->second(cmd.path, cmd.assetName));
	}
}

void Engine::Core::AssetPipeline::refreshAssetManager(AssetManager& am)
{

	while (!queue.empty())
	{
		Engine::Core::ImportCommand icmd = queue.top();
		queue.pop();
		processCommand(icmd, am);
	}
}
