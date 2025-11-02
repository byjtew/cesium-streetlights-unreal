// Copyright (c) 2024 Clarisma / GeoDesk contributors
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <cstddef>

namespace clarisma {

class FileSystem
{
public:
	static size_t getBlockSize(const char* path);
	static size_t getAvailableDiskSpace(const char* path);
};

} // namespace clarisma
