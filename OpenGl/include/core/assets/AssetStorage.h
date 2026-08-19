#pragma once
#include "SparseSet.h"
#include <unordered_map>
namespace Engine::Core
{
	template<typename T, size_t maxIndex, size_t capacity>
	class AssetStorage
	{
	private:
		SparseSet<std::unique_ptr<T>, maxIndex, capacity> assets{};
		
		std::unordered_map<std::string, size_t> nameToId{};
		std::unordered_map<size_t, std::string> idToName{};

		size_t nextAvailableIndex = 0;
	public:
		AssetStorage() = default;

		bool contains(size_t index) const 
		{ 
			return assets.contains(index); 
		}

		bool contains(const std::string& name) const
		{
			if (!nameToId.contains())
			{
				return false;
			}

			size_t index = nameToId[name];
			return assets.contains(index);
		}

		size_t add(T asset, std::string name)
		{
			if (nameToId.contains(name))
			{
				throw std::runtime_error("Asset with name " + name + " already exists.");
			}

			size_t newIdx = nextAvailableIndex;

			if (idToName.contains(newIdx))
			{
				throw std::runtime_error("Asset with index " + newIdx + " already exists.");
			}

			std::unique_ptr<T> assetPtr = std::make_unique<T>(asset);
			
			assets.insert(newIdx, std::move(assetPtr));
			
			nameToId[name] = newIdx;
			idToName[newIdx] = name;
			
			nextAvailableIndex++;
			return newIdx;
		}

		size_t add(std::unique_ptr<T> asset, const std::string& name)
		{
			if (nameToId.contains(name))
			{
				throw std::runtime_error("Asset with name " + name + " already exists.");
			}

			size_t newIdx = nextAvailableIndex;

			if (idToName.contains(newIdx))
			{
				throw std::runtime_error("Asset with index " + std::to_string(newIdx) + " already exists.");
			}

			assets.insert(newIdx, std::move(asset));

			nameToId[name] = newIdx;
			idToName[newIdx] = name;

			nextAvailableIndex++;
			return newIdx;
		}

		T* get(const size_t& index)
		{
			if (!assets.contains(index))
			{
				return nullptr;
			}

			return assets.get(index).get();
		}

		T* get(const std::string& name)
		{
			if (!nameToId.contains(name))
			{
				return nullptr;
			}

			size_t id = nameToId[name];
			
			if (!assets.contains(id))
			{
				return nullptr;
			}

			return assets.get(id).get();
		}

		std::vector<T*> getRawPointerList()
		{
			std::vector<T*> list{};

			for (const auto v& : assets)
			{
				T* a = v.get();
				list.push_back(a);
			}

			return list;
		}

	};
}
