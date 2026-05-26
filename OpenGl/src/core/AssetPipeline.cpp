#include "core/assets/AssetManager.h"
#include "core/assets/AssetPipeline.h"

void Engine::Core::AssetPipeline::loadAssetManager(AssetManager& am)
{
	while (!queue.empty())
	{
		Engine::Core::ImportCommand icmd = queue.top();
		queue.pop();
		auto vopt = processCommand(icmd);
		if (!vopt.has_value()) 
			continue;
		std::unique_ptr<MeshData> m = std::move(std::get<std::unique_ptr<MeshData>>(vopt.value()));
		std::unique_ptr<TextureData> t = std::move(std::get<std::unique_ptr<TextureData>>(vopt.value()));
		std::unique_ptr<MaterialData> mat = std::move(std::get<std::unique_ptr<MaterialData>>(vopt.value()));
		if (auto p = std::get_if<std::unique_ptr<MeshData>>(&(vopt.value())))
		{
			if (m) am.addMesh(icmd.assetName, std::move(m));
			else if (t) am.addTexture(icmd.assetName, std::move(t));
			else if (mat) am.addMaterial(icmd.assetName, std::move(mat));
		}
	}
}
