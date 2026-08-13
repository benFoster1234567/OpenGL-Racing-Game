#pragma once
template <typename T, size_t SparseMax = 256, size_t Capacity = 128>
class SparseSet
{
private:
	std::vector<size_t> sparse;
	std::vector<T> dense;
	std::vector<size_t> denseToSparse;
	size_t size{ 0 };
public:
	SparseSet()
	{
		sparse.resize(SparseMax, Capacity);
		dense.reserve(Capacity);
		denseToSparse.reserve(Capacity);
	}
	void insert(size_t index, const T& value)
	{
		if (index >= SparseMax || size >= Capacity)
		{
			throw std::out_of_range("Index out of bounds or capacity exceeded");
		}
		if (sparse[index] != Capacity)
		{
			throw std::runtime_error("Element already exists at this index");
		}
		dense.push_back(value);
		denseToSparse.push_back(index);
		sparse[index] = size;
		size++;
	}

	void remove(size_t index)
	{
		if (index >= SparseMax || sparse[index] == Capacity)	
		{
			throw std::out_of_range("Index out of bounds or element does not exist");
		}
		size_t denseIndex = sparse[index];
		size_t lastSparseIndex = denseToSparse[size - 1];

		dense[denseIndex] = dense[size - 1];
		sparse[lastSparseIndex] = denseIndex;
		denseToSparse[denseIndex] = lastSparseIndex;
		
		sparse[index] = Capacity;
		dense.pop_back();
		denseToSparse.pop_back();
		size--;
	}

	T& get(size_t index)
	{
		if (index >= SparseMax || sparse[index] == Capacity)
		{
			throw std::out_of_range("Index out of bounds or element does not exist");
		}
		return dense[sparse[index]];
	}

	const T& get(size_t index) const
	{
		if (index >= SparseMax || sparse[index] == Capacity)
		{
			throw std::out_of_range("Index out of bounds or element does not exist");
		}
		return dense[sparse[index]];
	}
};