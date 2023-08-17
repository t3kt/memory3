#include "Settings.h"

using tekt::FloatOpts;
using tekt::IntOpts;
using tekt::ParBuilder;
using tekt::ParamGroup;
using tekt::Settings;

namespace Page {
  const std::string sim = "Simulation";
  const std::string obs = "Observers";
  const std::string occ = "Occurrences";
  const std::string forces = "Forces";
}

SimulationParams::SimulationParams()
  : ParamGroup(Page::sim),
  enabled{ "Enable", "Enable", false },
  reset{ "Reset", "Reset", false },
  resetNow{ PulseNames::reset, "Reset" } {
  add(enabled);
  add(reset);
  add(resetNow);
}

SimulationSettings::SimulationSettings() {
  add(simulation);
}
