#pragma once

#include <functional>
#include <unordered_set>
#include "Connection.h"

using ConnectionSet = std::unordered_set<Connection, ConnectionHash>;

class ConnectionManager
{
public:
  void addConnection(EntityId obsId, EntityId occId);

  void removeConnection(EntityId obsId, EntityId occId);

  void removeAllFromObserver(EntityId obsId);

  void removeAllToOccurrence(EntityId occId);

  void removeWhere(const std::function<bool(const Connection&)>& predicate);

  void forEachConnectionFromObserver(EntityId obsId, const std::function<void(const EntityId&)>& action);

  void forEachConnectionToOccurrence(EntityId occId, const std::function<void(const EntityId&)>& action);

  //EntityIdList getObserversConnectedToOccurrence(EntityId occId) const;
  //EntityIdList getOccurrencesConnectedToObserver(EntityId obsId) const;

  ConnectionSet::iterator begin() { return _connections.begin(); }
  ConnectionSet::const_iterator begin() const { return _connections.begin(); }
  ConnectionSet::iterator end() { return _connections.end(); }
  ConnectionSet::const_iterator end() const { return _connections.end(); }

  std::size_t size() const { return _connections.size(); }
  bool empty() const { return _connections.empty(); }

  void reset() {
    _connections.clear();
  }
private:
  ConnectionSet _connections;
};

