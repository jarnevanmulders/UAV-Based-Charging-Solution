from PySpice.Spice.Netlist import Circuit, SubCircuit, SubCircuitFactory
from PySpice.Unit import *


# Simplified model
# Extended model: https://pyspice.fabrice-salvaire.fr/releases/latest/examples/transformer/Transformer.html

class Transformer(SubCircuit):
    __nodes__ = ('input_plus', 'input_minus',
                 'output_plus', 'output_minus')

    def __init__(self,
                 name,
                 primary_inductance=1@u_H,
                 primary_resistance=1@u_Ω,
                 secondary_inductance=1@u_H,
                 secondary_resistance=1@u_Ω,
                 coupling=.999,
                 ):

        SubCircuit.__init__(self, name, *self.__nodes__)

        # Primary
        primary_inductor = self.L('primary', 'input_plus', 2, primary_inductance)
        self.R('primary', 2, 'output_minus', primary_resistance)

        # Secondary
        secondary_inductor = self.L('secondary', 'output_plus', 4, secondary_inductance)
        self.R('secondary', 4, 'output_minus', secondary_resistance)

        # Coupling
        self.CoupledInductor('coupling', primary_inductor.name, secondary_inductor.name, coupling)
