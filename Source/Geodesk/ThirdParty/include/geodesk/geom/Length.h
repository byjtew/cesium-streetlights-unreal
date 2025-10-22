// Copyright (c) 2024 Clarisma / GeoDesk contributors
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <geodesk/feature/WayPtr.h>
#include <geodesk/feature/RelationPtr.h>

namespace geodesk {

///
/// \cond lowlevel
///
class GEODESK_API Length
{
public:
	static double ofWay(WayPtr way);
	static double ofRelation(FeatureStore* store, RelationPtr relation);

private:
	static double ofRelation(FeatureStore* store, RelationPtr rel, RecursionGuard& guard);
};

// \endcond

} // namespace geodesk
