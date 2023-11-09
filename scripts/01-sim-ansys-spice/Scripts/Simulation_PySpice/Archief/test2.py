import PySpice.Logging.Logging as Logging
logger = Logging.setup_logging()


from PySpice.Spice.Netlist import Circuit, SubCircuit, SubCircuitFactory
from PySpice.Unit import *


class ParallelResistor2(SubCircuit):
    __nodes__ = ('n1', 'n2')
    def __init__(self, name, R1=1@u_Ω, R2=2@u_Ω):
        SubCircuit.__init__(self, name, *self.__nodes__)
        self.R(1, 'n1', 'n2', R1)
        self.R(2, 'n1', 'n2', R2)

circuit = Circuit('Test')
circuit.subcircuit(ParallelResistor2('pr1', R2=2@u_Ω))
circuit.X('1', 'pr1', 1, circuit.gnd)
circuit.subcircuit(ParallelResistor2('pr2', R2=3@u_Ω))
circuit.X('2', 'pr2', 1, circuit.gnd)

print(circuit)