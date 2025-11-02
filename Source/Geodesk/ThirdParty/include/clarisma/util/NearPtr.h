// Copyright (c) 2024 Clarisma / GeoDesk contributors
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <cassert>
#include <cstddef>

namespace clarisma {

/// A pointer that can reference a limited address range, in exchange
/// for a smaller storage footprint. A `NearPtr` always has to be
/// resolved against a base pointer.
/// @tparam T the type referenced by the NearPtr
/// @tparam U the type used to store the NearPtr (signed or unsigned
/// int32, int16 or even int8)
/// @tparam Shift a shift to apply to the NearPtr, giving it a greater
///   range if a coarser alignment can be guaranteed
///
template<typename T, typename U, int Shift>
class NearPtr
{
public:
    NearPtr(T* p, void* base) :
        data_(static_cast<U>((bytePtr(p) - bytePtr(base)) >> Shift))
    {
        assert(p == *this(base));
    }

    NearPtr(const NearPtr& other) = default;

    T* operator()(void* base) const noexcept
    {
        return reinterpret_cast<T*>(bytePtr(base) + (data_ << Shift));
    }

private:
    static std::byte* bytePtr(void *p)
    {
        return reinterpret_cast<std::byte*>(p);
    }

    U data_;
};

} // namespace clarisma
