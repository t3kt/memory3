#pragma once

#include <functional>
#include <vector>
#include "Entity.h"

class Connection : public SimObject {
public:
  Connection() {
    objectType = ObjectType::CONNECTION;
  }

  EntityId observerId = invalidId;
  EntityId occurrenceId = invalidId;

  bool operator==(const Connection& other) const {
    return other.observerId == observerId && other.occurrenceId == occurrenceId;
  }

  virtual void reset() override {
    SimObject::reset();
    observerId = invalidId;
    occurrenceId = invalidId;
  }
};

using ConnectionList = std::vector<Connection>;


struct ConnectionHash {
  std::size_t operator()(Connection const& pair) const noexcept {
    return std::hash<EntityId>{}(pair.observerId) ^ (std::hash<EntityId>{}(pair.occurrenceId) << 1);
  }
};
