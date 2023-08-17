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

  TD::Position position;
  TD::Vector velocity;
  TD::Vector rotation;
  TD::Vector spin;

  virtual void reset() override {
    SimObject::reset();
    id = invalidId;
    alive = false;
    position = TD::Position();
    velocity = TD::Vector();
    rotation = TD::Vector();
    spin = TD::Vector();
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

  float age = 0.0f;
  float lifespan = 0.0f;

  virtual void reset() override {
    Entity::reset();
    age = 0.0f;
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
