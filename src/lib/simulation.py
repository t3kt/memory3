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

@dataclass
class ParticleObject(SimObject):
	position: 'tdu.Position' = field(default_factory=_zeroPosition)
	velocity: 'tdu.Vector' = field(default_factory=_zeroVector)
	rotation: 'tdu.Vector' = field(default_factory=_zeroVector)
	spin: 'tdu.Vector' = field(default_factory=_zeroVector)

	def distanceFrom(self, pos: 'tdu.Position'):
		return (self.position - pos).length()

@dataclass
class Observer(ParticleObject):
	age: float = 0
	lifespan: float = 0
	connections: List['Connection'] = field(default_factory=list)

@dataclass
class Occurrence(ParticleObject):
	radius: float = 0
	connections: List['Connection'] = field(default_factory=list)

@dataclass
class Connection(SimObject):
	observerId: ObjectId = InvalidId
	occurrenceId: ObjectId = InvalidId

@dataclass
class SimulationContent:
	observers: List[Observer] = field(default_factory=list)
	occurrences: List[Occurrence] = field(default_factory=list)
	connections: List[Connection] = field(default_factory=list)

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
