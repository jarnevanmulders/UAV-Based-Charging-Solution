import matplotlib.pyplot as plt
from Transformer import Transformer
import math
import PySpice.Logging.Logging as Logging

logger = Logging.setup_logging()
from PySpice.Spice.Netlist import Circuit
from PySpice.Unit import *
import sys

import pandas as pd

FREQ = 6.78e6

'''
L1 = 2.131e-6
L2 = 1.155e-6
R1 = 0.24617
R2 = 0.28147
k = 0.044284
'''


def rms(buffer):
    return math.sqrt(sum(buffer ** 2) / len(buffer))


def efficiency(vl1, vr1, vk, vl2, vr2, vrload):
    RS = 1e-3 @ u_Ω

    C1 = ((2 * math.pi * FREQ) ** 2 * vl1) ** (-1)
    C2 = ((2 * math.pi * FREQ) ** 2 * vl2) ** (-1)

    circuit = Circuit('Wireless Power Transfer Simulator')

    circuit.subcircuit(Transformer('transfo', primary_inductance=vl1, primary_resistance=vr1,
                                   secondary_inductance=vl2, secondary_resistance=vr2,
                                   coupling=vk))

    source = circuit.SinusoidalVoltageSource('in', 1, circuit.gnd, amplitude=10, frequency=FREQ)
    circuit.R('S', 1, 2, RS)
    circuit.C(1, 2, 3, C1)
    circuit.X('1', 'transfo', 3, circuit.gnd, 4, circuit.gnd)

    circuit.C(2, 4, 5, C2)
    circuit.R('load', 5, circuit.gnd, vrload)

    simulator = circuit.simulator(temperature=25, nominal_temperature=25)
    analysis = simulator.transient(step_time=source.period / 200, end_time=102 @ u_us, start_time=100 @ u_us)

    I1 = (analysis['2'] - analysis['1']) / RS
    V1 = analysis['2']
    I2 = (analysis['5']) / vrload
    V2 = analysis['5']
    EFF = rms(V2) * rms(I2) / (rms(V1) * rms(I1))

    '''
    print("------------------------------------------------")
    print("Capacitor 1: " + str(round(C1 * 1e12, 1)) + " pF")
    print("Capacitor 2: " + str(round(C2 * 1e12, 1)) + " pF")
    print("------------------------------------------------")

    print(circuit)
    
    print("Source: V1: " + str(rms(V1)) + " I1: " + str(rms(I1)))
    print("Load: V2: " + str(rms(V2)) + " I2: " + str(rms(I2)))
    '''

    return EFF


def efficiency_calculation(l1, r1, l2, r2, load, k):
    qt = 2 * math.pi * FREQ * l1 / r1
    qr = 2 * math.pi * FREQ * l2 / (r2 + load)
    comb = k ** 2 * qt * qr
    return load / (load + r2) * comb / (1 + comb)


def optimal_load_calculation(l1, r1, l2, r2, k):
    qt = 2 * math.pi * FREQ * l1 / r1
    q2 = 2 * math.pi * FREQ * l2 / r2
    return math.sqrt(r2 ** 2 * (1 + k ** 2 * qt * q2))


df = pd.read_csv(r'../../CouplingCoeff-XYZmove.csv')

length = len(df['k'])


sim_eff_max_list = []
sim_optimal_load_list = []
calc_eff_max_list = []
calc_optimal_load_list = []

for j in range(length):

    # SIMULATION - Find optimal load with PySpice (iterative process)
    eff_buf = [0]
    stepsize = 0.1

    # SIMULATION - Calculate efficiency for multiple loads
    sys.stdout.write("[")
    for i in range(1, int(10 / stepsize)):
        eff_buf.append(
            efficiency(df['L1'][j].astype(float) * 1e-6, df['R1'][j].astype(float) * 1e-3, df['k'][j].astype(float),
                       df['L2'][j].astype(float) * 1e-6, df['R2'][j].astype(float) * 1e-3, i * stepsize))
        sys.stdout.write("=")
    print("]")

    # SIMULATION - Search for the optimal load (load which gives highest efficiency)
    sim_eff_max = max(eff_buf)
    sim_optimal_load = eff_buf.index(sim_eff_max) * stepsize

    # SIMULATION - Append to the lists
    sim_eff_max_list.append(sim_eff_max)
    sim_optimal_load_list.append(sim_optimal_load)

    # CALCULATION - Calculation optimal load
    calc_optimal_load = optimal_load_calculation(df['L1'][j].astype(float) * 1e-6,
                                                 df['R1'][j].astype(float) * 1e-3,
                                                 df['L2'][j].astype(float) * 1e-6,
                                                 df['R2'][j].astype(float) * 1e-3,
                                                 df['k'][j].astype(float))
    calc_optimal_load_list.append(calc_optimal_load)

    # CALCULATION - Calculation maximum link efficiency
    calc_eff_max = efficiency_calculation(df['L1'][j].astype(float) * 1e-6,
                                          df['R1'][j].astype(float) * 1e-3,
                                          df['L2'][j].astype(float) * 1e-6,
                                          df['R2'][j].astype(float) * 1e-3,
                                          calc_optimal_load,
                                          df['k'][j].astype(float))

    calc_eff_max_list.append(calc_eff_max)

    print('*** info ***')
    print("X: " + str(df['xmove'][j]) + " Y: " + str(df['ymove'][j]) + " Z: " + str(df['zmove'][j]))
    print('Simulation:')
    print('Optimal load: ' + str(sim_optimal_load) + ' ohm - Efficiency: ' + str(sim_eff_max))
    print('Calculation:')
    print('Optimal load: ' + str(calc_optimal_load) + ' ohm - Efficiency: ' + str(calc_eff_max))


df['sim_eff_max'] = sim_eff_max_list
df['sim_optimal_load'] = sim_optimal_load_list
df['calc_eff_max'] = calc_eff_max_list
df['calc_optimal_load'] = calc_optimal_load_list

# WRITE - To a file
#df.to_csv('output-efficiencies.csv')

