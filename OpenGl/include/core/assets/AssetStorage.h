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
		SparseSet<std::unique_ptr<T>, maxIndex, capacity> m_assets{};

		std::unordered_map<std::string, size_t> m_nameToId{};
		std::unordered_map<size_t, std::string> m_idToName{};

		size_t m_nextAvailableIndex = 0;

	public:
		AssetStorage() = default;

		bool contains(size_t index) const
		{
			return m_assets.contains(index);
		}

		bool contains(const std::string& name)
		{
			auto it = m_nameToId.find(name);
			if (it == m_nameToId.end())
			{
				return false;
			}

			return m_assets.contains(it->second);
		}

		size_t getId(std::string name)
		{

			auto it = m_nameToId.find(name);
			if (it == m_nameToId.end())
			{
				throw std::runtime_error("Asset with name '" + name + "' does not exist.");
			}
			return it->second;
		}

		size_t add(T asset, const std::string& name)
		{
			if (m_nameToId.contains(name))
			{
				throw std::runtime_error("Asset with name " + name + " already exists.");
			}

			size_t newIdx = m_nextAvailableIndex;

			if (m_idToName.contains(newIdx))
			{
				throw std::runtime_error("Asset with index " + std::to_string(newIdx) + " already exists.");
			}

			m_assets.insert(newIdx, std::make_unique<T>(std::move(asset)));

			m_nameToId[name] = newIdx;
			m_idToName[newIdx] = name;

			m_nextAvailableIndex++;
			return newIdx;
		}

		size_t add(std::unique_ptr<T> asset, const std::string& name)
		{
			if (m_nameToId.contains(name))
			{
				throw std::runtime_error("Asset with name " + name + " already exists.");
			}

			size_t newIdx = m_nextAvailableIndex;

			if (m_idToName.contains(newIdx))
			{
				throw std::runtime_error("Asset with index " + std::to_string(newIdx) + " already exists.");
			}

			m_assets.insert(newIdx, std::move(asset));

			m_nameToId[name] = newIdx;
			m_idToName[newIdx] = name;

			m_nextAvailableIndex++;
			return newIdx;
		}

		T* get(size_t index)
		{
			if (!m_assets.contains(index))
			{
				return nullptr;
			}

			return m_assets.get(index).get();
		}

		const T* get(size_t index) const
		{
			if (!m_assets.contains(index))
			{
				return nullptr;
			}

			return m_assets.get(index).get();
		}

		T* get(const std::string& name)
		{
			auto it = m_nameToId.find(name);
			if (it == m_nameToId.end())
			{
				return nullptr;
			}

			return get(it->second);
		}

		const T* get(const std::string& name) const
		{
			auto it = m_nameToId.find(name);
			if (it == m_nameToId.end())
			{
				return nullptr;
			}

			return get(it->second);
		}

		std::string getName(size_t index) const
		{
			auto it = m_idToName.find(index);
			return it != m_idToName.end() ? it->second : "";
		}

		std::vector<T*> getRawPointerList()
		{
			std::vector<T*> list{};

			for (const auto& v : m_assets)
			{
				list.push_back(v.get());
			}

			return list;
		}

		size_t size()
		{
			return m_assets.size();
		}

	};
}