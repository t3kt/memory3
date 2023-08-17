#include "Entity.h"

void EntityRef::assign(const Entity* entity) {
  if (entity == nullptr) {
    type = ObjectType::INVALID;
    id = invalidId;
  }
  else {
    type = entity->objectType;
    id = entity->id;
  }
}
