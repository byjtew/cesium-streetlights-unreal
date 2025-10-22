// Copyright (c) 2024 Clarisma / GeoDesk contributors
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

// Export.h
#pragma once

#if defined(_WIN32) || defined(__CYGWIN__)
    #ifdef GEODESK_SHARED
        #ifdef GEODESK_EXPORTS
            #ifdef __GNUC__
                #define GEODESK_API __attribute__((dllexport))
            #else
                #define GEODESK_API __declspec(dllexport)
            #endif
        #else
            #ifdef __GNUC__
                #define GEODESK_API __attribute__((dllimport))
            #else
                #define GEODESK_API __declspec(dllimport)
            #endif
        #endif
    #else
        #define GEODESK_API
    #endif
#else
    #if __GNUC__ >= 4
        #define GEODESK_API __attribute__((visibility("default")))
    #else
        #define GEODESK_API
    #endif
#endif
