// Copyright (c) 2024 Clarisma / GeoDesk contributors
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once
#include <cstdint>
#include <string>
#include <geodesk/export.h>

namespace geodesk {

class Tags;
class View;

/// \cond internal
///
class GEODESK_API FeatureUtils
{
public:
    static uint64_t count(const View& view);
    static bool isEmpty(const View& view);
    static char* format(char* buf, const char* type, int64_t id);
    static std::string label(const Tags& tags);

private:
    static uint64_t countWorld(const View& view);
    static uint64_t countGeneric(const View& view);
};

// \endcond

} // namespace geodesk