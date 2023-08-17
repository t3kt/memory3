#pragma once

#include "../TektTDCommon/CHOP_ApiWrapper.h"

using EntityId = int;

enum class EntityType {
  PERSON,
  EVENT,

  INVALID = -1
};

class EntityBase {
public:
  virtual ~EntityBase() = default;

  EntityType entityType;

  EntityId id = -1;
  bool alive = false;
  TD::Vector position;

  virtual void reset();
};
