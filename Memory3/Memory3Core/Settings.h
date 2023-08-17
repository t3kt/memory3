#pragma once

#include <string>
#include "../TektTDCommon/CHOP_ApiWrapper.h"
#include "../TektTDCommon/TDParameters.h"
#include "../TektTDCommon/TDSettings.h"

using tekt::BoolParameter;
using tekt::PulseParameter;

namespace PulseNames {
  const std::string reset = "Resetpulse";
}

class SimulationParams : public tekt::ParamGroup {
public:
  SimulationParams();

  BoolParameter enabled;
  BoolParameter reset;
  PulseParameter resetNow;
};

class SimulationSettings : public tekt::Settings {
public:
  SimulationSettings();

  SimulationParams simulation;
};
