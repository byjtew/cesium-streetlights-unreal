﻿#pragma once

#include "Modules/ModuleManager.h"

class FCesiumStreetlightsForUnrealModule : public IModuleInterface
{
public:
  virtual void StartupModule() override;
  virtual void ShutdownModule() override;
};
