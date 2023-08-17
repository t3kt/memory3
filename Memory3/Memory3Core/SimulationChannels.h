#pragma once

#include "../TektTDCommon/TDChannels.h"
#include "Connection.h"
#include "Entity.h"

using tekt::ChannelMap;
using tekt::OutputChannelBase;
using tekt::OutputChannel;
using tekt::FloatOutChannel;
using tekt::IntOutChannel;
using tekt::BoolOutChannel;
using tekt::VectorOutChannel;
using tekt::InputChannelBase;
using tekt::InputChannel;
using tekt::FloatInChannel;
using tekt::IntInChannel;
using tekt::BoolInChannel;
using tekt::VectorInChannel;


class SimulationOutputChannels {
public:
  static const ChannelMap& getOutputChannelMap();

  SimulationOutputChannels(TD::CHOP_Output* outputs, const ChannelMap& channelMap);

  void appendObserver(const Observer& observer);
  void appendOccurrence(const Occurrence& occurrence);
  void appendDefault(ObjectType type = ObjectType::INVALID);
  void appendConnection(const Connection& conn);
private:
  void appendEntity(const Entity& entity);
  void writeDefaults(ObjectType type);

  void forEachChannel(const std::function<void(OutputChannelBase&)>& action);

  std::int32_t _i = 0;

  IntOutChannel _id;
  OutputChannel<ObjectType> _type;
  FloatOutChannel _age;
  FloatOutChannel _life;
  BoolOutChannel _alive;
  VectorOutChannel _t;
  VectorOutChannel _r;
  VectorOutChannel _s;
  VectorOutChannel _vel;
  VectorOutChannel _spin;
  VectorOutChannel _force;
  IntOutChannel _conns;
  IntOutChannel _obsId;
  IntOutChannel _occId;
  VectorOutChannel _t2;
  FloatOutChannel _life2;
};

class SimulationInputChannels {
public:
  explicit SimulationInputChannels(const TD::OP_CHOPInput* inputs);
  ObjectType getObjectType(std::int32_t i) const;
  bool readEntity(std::int32_t i, Entity* entity) const;
  bool readConnection(std::int32_t i, Connection* connection) const;
  std::int32_t numSamples() const { return _numSamples; }
private:
  void forEachChannel(const std::function<void(InputChannelBase&)>& action);

  ChannelMap _channelMap;
  const std::int32_t _numSamples;

  IntInChannel _id;
  InputChannel<ObjectType> _type;
  FloatInChannel _age;
  FloatInChannel _life;
  BoolInChannel _alive;
  VectorInChannel _t;
  VectorInChannel _r;
  VectorInChannel _s;
  VectorInChannel _vel;
  VectorInChannel _spin;
  VectorInChannel _force;
  IntInChannel _conns;
  IntInChannel _obsId;
  IntInChannel _occId;
  VectorInChannel _t2;
  FloatInChannel _life2;
};

