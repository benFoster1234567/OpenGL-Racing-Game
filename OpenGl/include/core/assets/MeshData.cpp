#include "MeshData.h"

void Engine::Core::MeshData::printPoints()
{
	for (const auto& a : attributes)
	{
		std::cout << "attribute index [" << a.index << "]\n";
		for (int i{}; i < a.data.size(); i++)
		{
			std::cout << a.data[i];
			if (i == a.data.size() - 1)
				std::cout << "\n\n";
			else if (i >= 1000) 
			{
				std::cout << "...\n\n";
				break;
			}
			else
				std::cout << ", ";
		}
	}
}
