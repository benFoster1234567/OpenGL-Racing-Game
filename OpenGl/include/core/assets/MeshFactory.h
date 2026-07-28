#pragma once
#include "MeshData.h"
#include "AssetManager.h"

namespace Engine::Core
{
	class MeshFactory
	{
	private:
		static MeshData* addPlane(AssetManager& am);
		static MeshData* addCube(AssetManager& am);
		static MeshData* addSphere(AssetManager& am);
		static MeshData* addCylinder(AssetManager& am);
	public:
		static void loadPrimitives(AssetManager& am);
	};
}