from typing import List
import numpy as np
import matplotlib.pyplot as plt
import tikzplotlib
import math
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import pandas as pd

FREQ = 6.78 * 1e6

df = pd.read_csv(r'../../CouplingCoeff-XYZmove.csv')

def optimal_load_calculation(l1, r1, l2, r2, k):
    qt = 2 * math.pi * FREQ * l1 / r1
    q2 = 2 * math.pi * FREQ * l2 / r2
    return math.sqrt(r2 ** 2 * (1 + k ** 2 * qt * q2))

def efficiency_calculation(l1, r1, l2, r2, load, k):
    qt = 2 * math.pi * FREQ * l1 / r1
    qr = 2 * math.pi * FREQ * l2 / (r2 + load)
    comb = k ** 2 * qt * qr
    return load / (load + r2) * comb / (1 + comb)

def voltage_s(l1, r1, l2, r2, load, k, eff):
    qt = 2 * math.pi * FREQ * l1 / r1
    qr = 2 * math.pi * FREQ * l2 / (r2 + load)
    vs = math.sqrt(r1*(r2+load)*k**2*qt*qr/(load*eff**2)*21)
    return vs


load = 0
eff = 0
vs = 0
index = 0

for i in range(len(df['xmove'])):
    if df['xmove'][i] == 0 and df['ymove'][i] == 0 and df['zmove'][i] == 150:
        print(df['k'][i])
        load = optimal_load_calculation(df['L1'][i] * 1e-6, df['R1'][i] * 1e-3, df['L2'][i] * 1e-6, df['R2'][i] * 1e-3, df['k'][i])
        eff = efficiency_calculation(df['L1'][i] * 1e-6, df['R1'][i] * 1e-3, df['L2'][i] * 1e-6, df['R2'][i] * 1e-3, load, df['k'][i])
        vs = voltage_s(df['L1'][i] * 1e-6, df['R1'][i] * 1e-3, df['L2'][i] * 1e-6, df['R2'][i] * 1e-3, load, df['k'][i], eff)
        index = i
        break

print(load, eff*100, vs)

print(index)

eff = 0.7712712881483997

def calc_m(L1, L2, k):
    m = k * np.sqrt(L1 + L2)
    return m

# a = [df['L1'][index] * 1e-6, df['R1'][index] * 1e-3, df['L2'][index] * 1e-6, df['R2'][index] * 1e-3, df['k'][index]]
a = [calc_m(df['L1'][index], df['L2'][index], df['k'][index]), df['L1'][index], df['R1'][index], df['L2'][index], df['R2'][index], df['k'][index]]
print()
for element in a:
    print(round(element, 3))
print()
print(1/((FREQ*2*math.pi)**2*df['L1'][index] * 1e-6) * 1e12)
print(1/((FREQ*2*math.pi)**2*df['L2'][index] * 1e-6) * 1e12)
