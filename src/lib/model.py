from dataclasses import dataclass, field
from typing import Iterable, List, Optional

# noinspection PyUnreachableCode
if False:
	# noinspection PyUnresolvedReferences
	from _stubs import *

def _zeroPosition(): return tdu.Position(0, 0, 0)
def _zeroVector(): return tdu.Vector(0, 0, 0)

ObjectId = int
InvalidId = -1

@dataclass
class SimObject:
	objectId: ObjectId = InvalidId
	alive: bool = True

	@classmethod
	def getChopChannelNames(cls):
		return ['id', 'alive']

	@classmethod
	def prepareChopChannels(cls, chop: 'scriptCHOP'):
		chop.clear()
		for name in cls.getChopChannelNames():
			chop.appendChan(name)

	def writeToChop(self, chop: 'scriptCHOP', i: int):
		chop['id'][i] = self.objectId
		chop['alive'][i] = int(self.alive)

def _vectorChanNames(name): return [name + '_x', name + '_y', name + '_z']

@dataclass
class ParticleObject(SimObject):
	position: 'tdu.Position' = field(default_factory=_zeroPosition)
	velocity: 'tdu.Vector' = field(default_factory=_zeroVector)
	rotation: 'tdu.Vector' = field(default_factory=_zeroVector)
	spin: 'tdu.Vector' = field(default_factory=_zeroVector)

	@classmethod
	def getChopChannelNames(cls):
		names = SimObject.getChopChannelNames()
		names += _vectorChanNames('position') + _vectorChanNames('velocity')
		names += _vectorChanNames('rotation') + _vectorChanNames('spin')
		return names

	def writeToChop(self, chop: 'scriptCHOP', i: int):
		super().writeToChop(chop, i)
		chop['position_x'][i], chop['position_y'][i], chop['position_z'][i] = self.position
		chop['velocity_x'][i], chop['velocity_y'][i], chop['velocity_z'][i] = self.velocity
		chop['rotation_x'][i], chop['rotation_y'][i], chop['rotation_z'][i] = self.rotation
		chop['spin_x'][i], chop['spin_y'][i], chop['spin_z'][i] = self.spin

	def distanceFrom(self, pos: 'tdu.Position'):
		return (self.position - pos).length()

@dataclass
class Observer(ParticleObject):
	age: float = 0
	lifespan: float = 0
	connections: List['Connection'] = field(default_factory=list)

	@classmethod
	def getChopChannelNames(cls):
		names = ParticleObject.getChopChannelNames()
		names += ['age', 'lifespan', 'connection_count']
		return names

	def writeToChop(self, chop: 'scriptCHOP', i: int):
		super().writeToChop(chop, i)
		chop['age'][i] = self.age
		chop['lifespan'][i] = self.lifespan
		chop['connection_count'][i] = len(self.connections)

@dataclass
class Occurrence(ParticleObject):
	radius: float = 0
	connections: List['Connection'] = field(default_factory=list)

	@classmethod
	def getChopChannelNames(cls):
		names = ParticleObject.getChopChannelNames()
		names += ['radius', 'connection_count']
		return names

	def writeToChop(self, chop: 'scriptCHOP', i: int):
		super().writeToChop(chop, i)
		chop['radius'][i] = self.radius
		chop['connection_count'][i] = len(self.connections)

@dataclass
class Connection(SimObject):
	observerId: ObjectId = InvalidId
	occurrenceId: ObjectId = InvalidId

	@classmethod
	def getChopChannelNames(cls):
		return ['obsId', 'occId']

	@classmethod
	def prepareChopChannels(cls, chop: 'scriptCHOP'):
		chop.clear()
		for name in cls.getChopChannelNames():
			chop.appendChan(name)

	def writeToChop(self, chop: 'scriptCHOP', i: int):
		chop['obsId'][i] = self.observerId
		chop['occId'][i] = self.occurrenceId

@dataclass
class SimulationContent:
	observers: List[Observer] = field(default_factory=list)
	occurrences: List[Occurrence] = field(default_factory=list)
	connections: List[Connection] = field(default_factory=list)
	lastId: ObjectId = InvalidId

	def getNextId(self):
		self.lastId += 1
		return self.lastId

	def getObserverById(self, obsId: ObjectId) -> Optional[Observer]:
		for obs in self.observers:
			if obs.objectId == obsId:
				return obs

	def getOccurrenceById(self, occId: ObjectId) -> Optional[Occurrence]:
		for occ in self.occurrences:
			if occ.objectId == occId:
				return occ

	def getConnectionsFromObserver(self, obsId: ObjectId) -> Iterable[Connection]:
		for conn in self.connections:
			if conn.observerId == obsId:
				yield conn

	def getConnectionsFromOccurrence(self, occId: ObjectId) -> Iterable[Connection]:
		for conn in self.connections:
			if conn.occurrenceId == occId:
				yield conn

	def getObserversInRange(self, pos: 'tdu.Position', radius: float) -> Iterable[Observer]:
		for obs in self.observers:
			if obs.distanceFrom(pos) <= radius:
				yield obs
