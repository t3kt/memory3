from model import *

# noinspection PyUnreachableCode
if False:
	# noinspection PyUnresolvedReferences
	from _stubs import *

class Simulation:
	def __init__(self, ownerComp: 'COMP'):
		self.ownerComp = ownerComp
		self.content = SimulationContent()

	def writeObserversChop(self, chop: 'scriptCHOP'):
		chop.clear()
		Observer.prepareChopChannels(chop)
		n = len(self.content.observers)
		if n == 0:
			chop.numSamples = 1
			chop['id'][0] = InvalidId
			return
		chop.numSamples = n
		for i in range(n):
			self.content.observers[i].writeToChop(chop, i)

	def writeOccurrencesChop(self, chop: 'scriptCHOP'):
		chop.clear()
		Occurrence.prepareChopChannels(chop)
		n = len(self.content.occurrences)
		if n == 0:
			chop.numSamples = 1
			chop['id'][0] = InvalidId
			return
		for i in range(n):
			self.content.occurrences[i].writeToChop(chop, i)

	def writeConnectionsChop(self, chop: 'scriptCHOP'):
		chop.clear()
		Connection.prepareChopChannels(chop)
		n = len(self.content.connections)
		if n == 0:
			chop.numSamples = 1
			chop['obsId'][0] = InvalidId
			chop['occId'][0] = InvalidId
			return
		chop.numSamples = n
		for i in range(n):
			self.content.connections[i].writeToChop(chop, i)
