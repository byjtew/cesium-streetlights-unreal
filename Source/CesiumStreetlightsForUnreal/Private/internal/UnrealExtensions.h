#pragma once

#include "CoreMinimal.h"

namespace unreal_ext
{
  /**
   * 
   * @param radius in meters
   * @return {Segments, Thickness}
   */
  static std::tuple<int32, float> DebugSphereDrawParametersBySize(const double radius)
  {
    return {
      FMath::Min<int32>(128, FMath::Lerp<int32>(24, 64, radius / 5e3)),
      FMath::Lerp<float>(32, 1024, radius / 5e3),
    };
  }
}
