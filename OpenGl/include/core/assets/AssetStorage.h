#pragma once

#include "SparseSet.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

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

		bool contains(const std::string& name)
		{
			auto it = nameToId.find(name);
			if (it == nameToId.end())
			{
				return false;
			}

			return assets.contains(it->second);
		}

		size_t getId(const std::string& name)
		{
			auto it = nameToId.find(name);
			if (it == nameToId.end())
			{
				throw std::runtime_error("Asset with name '" + name + "' does not exist.");
			}
			return it->second;
		}

		size_t add(T asset, const std::string& name)
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

			assets.insert(newIdx, std::make_unique<T>(std::move(asset)));

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

		T* get(size_t index)
		{
			if (!assets.contains(index))
			{
				return nullptr;
			}

			return assets.get(index).get();
		}

		const T* get(size_t index) const
		{
			if (!assets.contains(index))
			{
				return nullptr;
			}

			return assets.get(index).get();
		}

		T* get(const std::string& name)
		{
			auto it = nameToId.find(name);
			if (it == nameToId.end())
			{
				return nullptr;
			}

			return get(it->second);
		}

		const T* get(const std::string& name) const
		{
			auto it = nameToId.find(name);
			if (it == nameToId.end())
			{
				return nullptr;
			}

			return get(it->second);
		}

		std::string getName(size_t index) const
		{
			auto it = idToName.find(index);
			return it != idToName.end() ? it->second : "";
		}

		std::vector<T*> getRawPointerList()
		{
			std::vector<T*> list{};

			for (const auto& v : assets)
			{
				list.push_back(v.get());
			}

			return list;
		}

		size_t size()
		{
			return assets.size();
		}

	};
}