#pragma once

#include <vector>
#include <algorithm>

template <typename OrderNodeType>
class RenderingOrder final {
private:
	std::vector<OrderNodeType> nodes;

public:
	RenderingOrder() = default;

	void release();
	void clear();

	void reserve(size_t size);

	void pushRenderingNode(const OrderNodeType& node);

	void sort();
};

template<typename OrderNodeType>
inline void RenderingOrder<OrderNodeType>::release() {
	nodes = std::vector<OrderNodeType>();
}

template<typename OrderNodeType>
inline void RenderingOrder<OrderNodeType>::clear() {
	nodes.clear();
}

template<typename OrderNodeType>
inline void RenderingOrder<OrderNodeType>::reserve(size_t size) {
	nodes.reserve(size);
}

template<typename OrderNodeType>
inline void RenderingOrder<OrderNodeType>::pushRenderingNode(const OrderNodeType& node) {
	nodes.push_back(node);
}

template<typename OrderNodeType>
inline void RenderingOrder<OrderNodeType>::sort() {
	std::stable_sort(nodes.begin(), nodes.end(), OrderNodeType::sortPredicate);
}
