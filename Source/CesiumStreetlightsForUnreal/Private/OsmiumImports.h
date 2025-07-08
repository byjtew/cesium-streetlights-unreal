#pragma once

#include "CoreMinimal.h"

THIRD_PARTY_INCLUDES_START
#pragma push_macro("CONSTEXPR")
#undef CONSTEXPR
#pragma push_macro("dynamic_cast")
#undef dynamic_cast
#pragma push_macro("check")
#undef check
#pragma push_macro("PI")
#undef PI

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <osmium/io/any_input.hpp>
#include <osmium/osm/way.hpp>
#include <osmium/osm/node.hpp>
#include <osmium/handler.hpp>
#include <osmium/visitor.hpp>

#pragma pop_macro("PI")
#pragma pop_macro("check")
#pragma pop_macro("dynamic_cast")
#pragma pop_macro("CONSTEXPR")

THIRD_PARTY_INCLUDES_END
