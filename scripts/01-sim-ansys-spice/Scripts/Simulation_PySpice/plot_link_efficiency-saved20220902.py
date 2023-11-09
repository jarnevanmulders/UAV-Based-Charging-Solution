from typing import List

import numpy as np
import matplotlib.pyplot as plt

import tikzplotlib

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import pandas as pd

df = pd.read_csv(r'output_efficiencies.csv')

print(df['sim_eff_max'])

fig = plt.figure(figsize=(10, 8))

ax = plt.axes(projection='3d')
ax.set_xlabel('Lateral alignment x [mm]')
ax.set_ylabel('Lateral alignment y [mm]')
ax.set_zlabel('Coil distance z [mm]')

m = ax.scatter(df['xmove'], df['ymove'], df['zmove'], c=df['sim_eff_max'] * 100, cmap='RdYlGn', s=100, marker='o',
               depthshade=False, vmin=0, vmax=100)

cb = plt.colorbar(m, ax=ax, shrink=0.7)
cb.set_label('Link efficiency [%]')

ax.view_init(7, 52)

# fig.show()

plt.tight_layout()

# fig.savefig('link_efficiency.svg', bbox_inches='tight', transparent=True)

# tikzplotlib.save("couplingfactor.tex")

print('ok')

for i in range(len(df['sim_eff_max'])):
    if float(df['sim_eff_max'][i]) > 0.15:
        print(str(i) + ' -- eta = ' + str(df['sim_eff_max'][i]) + ' -- x: ' + str(df['xmove'][i]) + ' | y: ' + str(df['ymove'][i]) + ' | z: ' + str(df['zmove'][i]))
