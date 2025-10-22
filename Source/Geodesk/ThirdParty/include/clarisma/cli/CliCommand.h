// Copyright (c) 2024 Clarisma / GeoDesk contributors
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <string_view>

namespace clarisma {

class CliCommand
{
public:
	virtual ~CliCommand() = default;
	virtual int run(char* argv[]);
	virtual bool setParam(int number, std::string_view value) { return false; };
	virtual int setOption(std::string_view name, std::string_view value) { return -1; };

	static std::string_view getCommand(char* argv[]);

private:
	// bool configure(char* argv[]);
	// void badOption(std::string_view name, std::string_view reason);
};

} // namespace clarisma
