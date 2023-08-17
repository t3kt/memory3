#include "SimulationChannels.h"

static const std::initializer_list<std::string> simulationChannelNames{
  "body_id",
  "type",
  "age",
  "life",
  "active",
  "tx", "ty", "tz",
  "rx", "ry", "rz",
  "sx", "sy", "sz",
  "vel_tx", "vel_ty", "vel_tz",
  "vel_rx", "vel_ry", "vel_rz",
  "forcex", "forcey", "forcez",
  "conns",
  "obsid",
  "occid",
  "tx2", "ty2", "tz2",
  "life2",
};

static const ChannelMap outputChannelMap(simulationChannelNames);

const ChannelMap& SimulationOutputChannels::getOutputChannelMap() {
  return outputChannelMap;
}

SimulationOutputChannels::SimulationOutputChannels(TD::CHOP_Output* outputs, const ChannelMap& channelMap)
  : _id({ "body_id" }, -1)
  , _type({ "type" }, ObjectType::INVALID)
  , _age({ "age" }, -1)
  , _life({ "life" }, -1)
  , _alive({ "active" }, false)
  , _t({ "tx", "ty", "tz" }, TD::Vector(0, 0, 0))
  , _r({ "rx", "ry", "rz" }, TD::Vector(0, 0, 0))
  , _s({ "sx", "sy", "sz" }, TD::Vector(0, 0, 0))
  , _vel({ "vel_tx", "vel_ty", "vel_tz" }, TD::Vector(0, 0, 0))
  , _spin({ "vel_rx", "vel_ry", "vel_rz" }, TD::Vector(0, 0, 0))
  , _force({ "forcex", "forcey", "forcez" }, TD::Vector(0, 0, 0))
  , _conns({ "conns" }, 0)
  , _obsId({ "obsid" }, -1)
  , _occId({ "occid" }, -1)
  , _t2({ "tx2", "ty2", "tz2" }, TD::Vector(0, 0, 0))
  , _life2({ "life2" }, -1) {
  forEachChannel([&](auto& channel) {
    channel.attachOutput(outputs, channelMap);
    });
}

SimulationInputChannels::SimulationInputChannels(const TD::OP_CHOPInput* inputs)
  : _id({ "body_id" }, -1)
  , _type({ "type" }, ObjectType::INVALID)
  , _age({ "age" }, -1)
  , _life({ "life" }, -1)
  , _alive({ "active" }, false)
  , _t({ "tx", "ty", "tz" }, TD::Vector(0, 0, 0))
  , _r({ "rx", "ry", "rz" }, TD::Vector(0, 0, 0))
  , _s({ "sx", "sy", "sz" }, TD::Vector(0, 0, 0))
  , _vel({ "vel_tx", "vel_ty", "vel_tz" }, TD::Vector(0, 0, 0))
  , _spin({ "vel_rx", "vel_ry", "vel_rz" }, TD::Vector(0, 0, 0))
  , _force({ "forcex", "forcey", "forcez" }, TD::Vector(0, 0, 0))
  , _conns({ "conns" }, 0)
  , _obsId({ "obsid" }, -1)
  , _occId({ "occid" }, -1)
  , _t2({ "tx2", "ty2", "tz2" }, TD::Vector(0, 0, 0))
  , _life2({ "life2" }, -1)
  , _numSamples(inputs->numSamples) {
  _channelMap.addFromInput(inputs);
  _channelMap.addAll(simulationChannelNames);
  forEachChannel([&](auto& channel) {
    channel.attachInput(inputs, _channelMap);
    });
}

void SimulationOutputChannels::forEachChannel(const std::function<void(OutputChannelBase&)>& action) {
  action(_id);
  action(_type);
  action(_age);
  action(_life);
  action(_alive);
  action(_t);
  action(_r);
  action(_s);
  action(_vel);
  action(_spin);
  action(_force);
  action(_conns);
  action(_obsId);
  action(_occId);
  action(_t2);
  action(_life2);
}

void SimulationInputChannels::forEachChannel(const std::function<void(InputChannelBase&)>& action) {
  action(_id);
  action(_type);
  action(_age);
  action(_life);
  action(_alive);
  action(_t);
  action(_r);
  action(_s);
  action(_vel);
  action(_spin);
  action(_force);
  action(_conns);
  action(_obsId);
  action(_occId);
  action(_t2);
  action(_life2);
}

void SimulationOutputChannels::writeDefaults(ObjectType type) {
  forEachChannel([&](auto& chan) {
    chan.outputDefault(_i);
    });
  _type.output(_i, type);
}

void SimulationOutputChannels::appendObserver(const Observer& observer) {
  appendEntity(observer);
}

void SimulationOutputChannels::appendOccurrence(const Occurrence& occurrence) {
  appendEntity(occurrence);
}

void SimulationOutputChannels::appendEntity(const Entity& entity) {
  writeDefaults(entity.objectType);
  _id.output(_i, entity.id);
  _alive.output(_i, entity.alive);
  _life.output(_i, entity.lifeAmount);
  _age.output(_i, entity.age);
  _t.output(_i, entity.position);
  _r.output(_i, entity.rotation);
  _s.output(_i, entity.scale);
  _vel.output(_i, entity.velocity);
  //_force.output(_i, entity.linearForce);
  _spin.output(_i, entity.spin);
  _conns.output(_i, entity.connectionCount);
  _i++;
}

//void SimulationOutputChannels::appendConnection(const Entity& observer, const Entity& occurrence) {
//  writeDefaults(ObjectType::CONNECTION);
//  _life.output(_i, observer.lifeAmount);
//  _alive.output(_i, observer.alive);
//  _t.output(_i, observer.position);
//  _obsId.output(_i, observer.id);
//  _occId.output(_i, occurrence.id);
//  _t2.output(_i, occurrence.position);
//  _life2.output(_i, occurrence.lifeAmount);
//  _i++;
//}

void SimulationOutputChannels::appendConnection(const Connection& conn) {
  writeDefaults(ObjectType::CONNECTION);
  _obsId.output(_i, conn.observerId);
  _occId.output(_i, conn.occurrenceId);
}

void SimulationOutputChannels::appendDefault(ObjectType type) {
  writeDefaults(type);
  _i++;
}

ObjectType SimulationInputChannels::getObjectType(std::int32_t i) const {
  return _type.input(i);
}

bool SimulationInputChannels::readEntity(std::int32_t i, Entity* entity) const {
  auto id = _id.input(i);
  auto alive = _alive.input(i);
  if (!alive || id < 0) {
    return false;
  }
  entity->id = id;
  entity->alive = alive;
  entity->lifeAmount = _life.input(i);
  entity->position = _t.input(i);
  entity->rotation = _r.input(i);
  entity->scale = _s.input(i);
  entity->velocity = _vel.input(i);
  entity->spin = _spin.input(i);
  // no force
  //entity->resetForces();
  entity->connectionCount = 0;
  return true;
}

bool SimulationInputChannels::readConnection(std::int32_t i, Connection* connection) const {
  auto obsId = _obsId.input(i);
  auto occId = _occId.input(i);
  if (obsId < 0 || occId < 0) {
    return false;
  }
  connection->observerId = obsId;
  connection->occurrenceId = occId;
  return true;
}
