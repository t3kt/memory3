#pragma once

#include "Entity.h"
#include "EntityBuffer.h"
#include "ConnectionManager.h"

class SimulationStateBase {
public:
  virtual ~SimulationStateBase() = default;
  virtual bool empty() const = 0;
  // TODO: readInputs
  virtual const Observer* getObserver(EntityId id) const = 0;
  virtual const Occurrence* getOccurrence(EntityId id) const = 0;
};

class SimulationState : public SimulationStateBase {
public:
  bool empty() const override {
    return _observers.empty() && _occurrences.empty() && _connections.empty();
  }

  const Observer* getObserver(EntityId id) const override {
    return _observers.findEntityById(id);
  }

  const Occurrence* getOccurrence(EntityId id) const override {
    return _occurrences.findEntityById(id);
  }

  void resetEntities() {
    _observers.reset();
    _occurrences.reset();
    _connections.reset();
  }

protected:
  EntityBuffer<Observer> _observers;
  EntityBuffer<Occurrence> _occurrences;
  ConnectionManager _connections;
};

