#pragma once

#include <functional>
#include "../TektTDCommon/CHOP_ApiWrapper.h"
#include "SimObject.h"

using EntityId = int;
const EntityId invalidId = -1;

class Entity : public SimObject {
public:
  EntityId id = invalidId;
  bool alive = false;
  float lifeAmount = 0.0f;
  float age = 0.0f;

  TD::Vector position;
  TD::Vector velocity;
  TD::Vector rotation;
  TD::Vector spin;
  TD::Vector scale;

  int connectionCount;

  virtual void reset() override {
    SimObject::reset();
    id = invalidId;
    alive = false;
    lifeAmount = 0.0f;
    age = 0.0f;
    position = TD::Vector();
    velocity = TD::Vector();
    rotation = TD::Vector();
    spin = TD::Vector();
    scale = TD::Vector();
    connectionCount = 0;
  }
};

using EntityAction = std::function<void(Entity&)>;
using EntityConstAction = std::function<void(const Entity&)>;
using EntityTest = std::function<bool(Entity&)>;
using EntityConstTest = std::function<bool(const Entity&)>;
using EntityPairAction = std::function<void(Entity&, Entity&)>;
using EntityPairConstAction = std::function<void(const Entity&, const Entity&)>;

class Observer : public Entity {
public:
  Observer() {
    objectType = ObjectType::OBSERVER;
  }

  float lifespan = 0.0f;

  virtual void reset() override {
    Entity::reset();
    lifespan = 0.0f;
  }
};

class Occurrence : public Entity {
public:
  Occurrence() {
    objectType = ObjectType::OCCURRENCE;
  }

  float radius = 0.0f;

  virtual void reset() override {
    Entity::reset();
    radius = 0.0f;
  }
};

class EntityRef {
public:
  ObjectType type = ObjectType::INVALID;
  EntityId id = invalidId;
  void assign(const Entity* entity);
  bool isInvalid() const { return type == ObjectType::INVALID || id == invalidId; }
};
