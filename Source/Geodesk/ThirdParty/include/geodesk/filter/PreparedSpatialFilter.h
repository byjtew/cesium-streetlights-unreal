// Copyright (c) 2024 Clarisma / GeoDesk contributors
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <geodesk/filter/SpatialFilter.h>
#include <geodesk/geom/index/MCIndex.h>

namespace geodesk {


// TODO: set acceleration flag!

class PreparedSpatialFilter : public SpatialFilter
{
public:
	PreparedSpatialFilter(const Box& bounds, MCIndex&& index) :
		SpatialFilter(bounds),
		index_(std::move(index))
	{
	}

protected:
	static const int MAX_CANDIDATE_MC_LENGTH = 32;
	
	bool anyNodesInPolygon(WayPtr way) const;
	bool anySegmentsCross(WayPtr way) const;
	bool wayIntersectsPolygon(WayPtr way) const;

	MCIndex index_;
};
} // namespace geodesk
