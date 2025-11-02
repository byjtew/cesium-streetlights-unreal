// Copyright (c) 2024 Clarisma / GeoDesk contributors
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <vector>
#include <geodesk/geom/index/RTree.h>
#include <geodesk/geom/Box.h>
#include <clarisma/alloc/Arena.h>

namespace geodesk {

class HilbertTreeBuilder
{
public:
	typedef RTree<const void>::Node Node;

	explicit HilbertTreeBuilder(clarisma::Arena* arena);
	const Node* buildNodes(const BoundedItem* items, size_t itemCount, 
		int maxItemsPerNode, Box totalBounds);

	template <typename IT>
	RTree<IT> build(const BoundedItem* items, size_t itemCount,
		int maxItemsPerNode, Box totalBounds)
	{
		return RTree<IT>(
			reinterpret_cast<const typename RTree<IT>::Node*>(
				buildNodes(items, itemCount, maxItemsPerNode, totalBounds)));
	}

private:
	typedef std::pair<uint32_t, const BoundedItem*> HilbertItem;
	
	static size_t calculateTotalNodeCount(size_t itemCount, int maxItemsPerNode);
	HilbertItem* buildHilbertIndex(const BoundedItem* items, size_t itemCount, const Box& totalBounds);

	clarisma::Arena& arena_;
	clarisma::Arena ownArena_;
};

} // namespace geodesk
