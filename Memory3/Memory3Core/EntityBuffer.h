#pragma once

#include <functional>
#include <queue>
#include <unordered_map>
#include <vector>
#include "Entity.h"

/// Manages a range of potentially available indices in a fixed size list.
/// It starts out with all indices "free" (available for usage).
/// When an available index is requested, it checks if there are any available
/// and returns the least recently used currently free index if any and marks
/// it as used.
/// When an index is released, it is marked as being available and put onto
/// the end of the queue of available indices.
/// 
/// The goal is to have the longest possible time between an entity dying and
/// its slot being reused for a new entity, in order to allow for things like
/// fading out slowly instead of immediately disappearing.
class IndexAllocator {
public:
  using IndexQueue = std::queue<std::size_t>;

  void reset(std::size_t newSize);
  void release(std::size_t index);
  std::size_t request();

  std::size_t totalSize() const { return _size; }
  std::size_t freeCount() const { return _free.size(); }
  std::size_t usedCount() const { return totalSize() - freeCount(); }
  bool full() const { return _free.empty(); }
  bool empty() const { return freeCount() == totalSize(); }
private:
  std::size_t _size = 0;
  IndexQueue _free;
};


template <typename T>
class EntityBuffer {
public:
  using List = std::vector<T>;
  using IdIndexMap = std::unordered_map<EntityId, std::size_t>;
  using Test = std::function<bool(T&)>;
  using Action = std::function<void(T&)>;
  using ConstAction = std::function<void(const T&)>;
  using PairAction = std::function<void(T&, T&)>;

  void initializeIfResized(std::size_t newSize) {
    if (newSize == size()) return;
    _entities.clear();
    for (std::size_t i = 0; i < newSize; ++i) {
      _entities.emplace_back(T());
    }
    _idToIndex.clear();
    _nextId = 1;
    _indexAllocator.reset(newSize);
  }

  std::size_t size() const { return _entities.size(); }
  std::size_t aliveCount() const { return _indexAllocator.usedCount(); }
  bool full() const { return _indexAllocator.full(); }
  bool empty() const { return _indexAllocator.empty(); }

  void reset() {
    _idToIndex.clear();
    _entities.assign(_entities.size(), T());
    _indexAllocator.reset(_entities.size());
  }

  T* findEntityById(EntityId id) {
    auto index = indexOfId(id);
    return entityAtIndex(index);
  }
  const T* findEntityById(EntityId id) const {
    auto index = indexOfId(id);
    return entityAtIndex(index);
  }
  void forEachEntity(const Action& action) {
    for (auto& entity : _entities) {
      action(entity);
    }
  }
  void forEachEntity(const ConstAction& action) const {
    for (auto& entity : _entities) {
      action(entity);
    }
  }
  T* tryInitNewEntity(const Test& initAndTest) {
    if (full()) {
      return nullptr;
    }
    auto index = _indexAllocator.request();
    if (index < 0) {
      return nullptr;
    }
    auto& entity = _entities[index];
    entity.reset();
    entity.id = _nextId;
    if (!initAndTest(entity)) {
      _indexAllocator.release(index);
      return nullptr;
    }
    if (entity.id >= _nextId) {
      _nextId = entity.id + 1;
    }
    else {
      ++_nextId;
    }
    _idToIndex[entity.id] = index;
    return &entity;
  }
  bool killEntityById(EntityId id) {
    auto index = indexOfId(id);
    auto entity = entityAtIndex(index);
    if (entity == nullptr || !entity->alive) {
      return false;
    }
    _idToIndex.erase(id);
    _indexAllocator.release(index);
    entity->kill();
    return true;
  }

  void forEachLiveEntity(const Action& action) {
    for (auto& entity : _entities) {
      if (entity.alive) {
        action(entity);
      }
    }
  }
  void forEachLiveEntityPair(const PairAction& action) {
    for (std::size_t i = 0; i < size(); ++i) {
      auto& entity1 = _entities[i];
      for (std::size_t j = i + 1; j < size(); ++j) {
        auto& entity2 = _entities[j];
        action(entity1, entity2);
      }
    }
  }
protected:
  std::size_t indexOfId(EntityId id) const {
    auto indexIter = _idToIndex.find(id);
    if (indexIter == _idToIndex.end()) {
      return -1;
    }
    if (indexIter->second < 0 || indexIter->second >= size()) {
      return -1;
    }
    return indexIter->second;
  }
  T* entityAtIndex(std::size_t index) {
    if (index < 0 || index >= size()) {
      return nullptr;
    }
    return &_entities.at(index);
  }
  const T* entityAtIndex(std::size_t index) const {
    if (index < 0 || index >= size()) {
      return nullptr;
    }
    return &_entities.at(index);
  }

  List _entities;
  IdIndexMap _idToIndex;
  IndexAllocator _indexAllocator;
  EntityId _nextId = 1;
};

