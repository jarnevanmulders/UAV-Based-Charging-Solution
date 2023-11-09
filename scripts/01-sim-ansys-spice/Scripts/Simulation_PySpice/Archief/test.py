import matplotlib.pyplot as plt

from PySpiceTest.Transformer import Transformer

import math

import PySpice.Logging.Logging as Logging

logger = Logging.setup_logging()

from PySpice.Spice.Netlist import Circuit
from PySpice.Unit import *


def rms(buffer):
    return math.sqrt(sum(buffer ** 2) / len(buffer))


FREQ = 6.78e6

L1 = 2.131e-6
L2 = 1.155e-6
R1 = 0.24617
R2 = 0.28147

RLOAD = 3.17 @ u_Ω
RS = 1e-3 @ u_Ω

k = 0.044284

C1 = ((2 * math.pi * FREQ) ** 2 * L1) ** (-1)
C2 = ((2 * math.pi * FREQ) ** 2 * L2) ** (-1)

print("------------------------------------------------")
print("Capacitor 1: " + str(round(C1 * 1e12, 1)) + " pF")
print("Capacitor 2: " + str(round(C2 * 1e12, 1)) + " pF")
print("------------------------------------------------")

circuit = Circuit('Wireless Power Transfer Simulator')

circuit.subcircuit(Transformer('transfo', primary_inductance=L1, primary_resistance=R1,
                               secondary_inductance=L2, secondary_resistance=R2,
                               coupling=k))

source = circuit.SinusoidalVoltageSource('in', 1, circuit.gnd, amplitude=10, frequency=FREQ)
circuit.R('S', 1, 2, RS)
circuit.C(1, 2, 3, C1)
circuit.X('1', 'transfo', 3, circuit.gnd, 4, circuit.gnd)

circuit.C(2, 4, 5, C2)
circuit.R('load', 5, circuit.gnd, RLOAD)

print(circuit)

print("ok")

figure, ax = plt.subplots(figsize=(20, 10))

simulator = circuit.simulator(temperature=25, nominal_temperature=25)
analysis = simulator.transient(step_time=source.period / 200, end_time=102 @ u_us, start_time=100 @ u_us)

I1 = (analysis['2'] - analysis['1']) / RS
V1 = analysis['2']
I2 = (analysis['5']) / RLOAD
V2 = analysis['5']
print("Source: V1: " + str(rms(V1)) + " I1: " + str(rms(I1)))
print("Load: V2: " + str(rms(V2)) + " I2: " + str(rms(I2)))
EFF = rms(V2) * rms(I2) / (rms(V1) * rms(I1))

print(EFF)

# Calculate the optimal load
QT = 2 * math.pi * FREQ * L1 / R1
Q2 = 2 * math.pi * FREQ * L2 / R2
RL_OPT = math.sqrt(R2 ** 2 * (1 + k ** 2 * QT * Q2))
print(RL_OPT)

# Calculate the link efficiency
QR = 2 * math.pi * FREQ * L2 / (R2 + RL_OPT)
COMB = k**2*QT*QR
EFF_OPT = RL_OPT/(RL_OPT+R2)*COMB/(1+COMB)
print(EFF_OPT)

ax.set_title('a')
ax.set_xlabel('Time [s]')
ax.set_ylabel('Voltage [V]')
ax.grid()
ax.plot(analysis['1'])
ax.plot(analysis['5'])
ax.plot(I1)
ax.plot(I2)
ax.legend(('input', 'output'), loc=(.05, .1))

plt.tight_layout()
# plt.show()
