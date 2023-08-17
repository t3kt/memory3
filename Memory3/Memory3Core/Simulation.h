#pragma once

#include "Settings.h"
#include "SimulationState.h"

class Simulation : public SimulationState {
public:
  void update(const TD::OP_Inputs& inputs);

  SimulationSettings& settings() { return _settings; }

  void reset();

  std::int32_t totalOutputItems() const {
    return static_cast<std::int32_t>(_observers.size() + _occurrences.size() + _connections.size());
  }
  bool isAtObserverLimit() const {
    return _observers.full();
  }
  bool isAtOccurrenceLimit() const {
    return _occurrences.full();
  }

private:
  SimulationSettings _settings {};
};

