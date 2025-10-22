// Copyright (c) 2024 Clarisma / GeoDesk contributors
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once
#include <string_view>

namespace geodesk {

/// @brief The OSM type of a feature: `NODE`, `WAY` or `RELATION`.
///
enum class FeatureType
{
    NODE = 0,
    WAY = 1,
    RELATION = 2
};

inline std::string_view typeName(FeatureType type)
{
    switch(type)
    {
    case FeatureType::NODE:
        return "node";
    case FeatureType::WAY:
        return "way";
    case FeatureType::RELATION:
        return "relation";
    default:
        return "invalid";
    }
}

} // namespace geodesk
