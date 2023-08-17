#pragma once

enum class ObjectType {
  OBSERVER,
  OCCURRENCE,
  CONNECTION,

  INVALID = -1
};

class SimObject {
public:
  virtual ~SimObject() = default;

  ObjectType objectType;

  virtual void reset() {}
};

