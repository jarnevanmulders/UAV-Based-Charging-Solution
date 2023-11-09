from typing import List

import numpy as np
import matplotlib.pyplot as plt

import tikzplotlib

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import pandas as pd

df = pd.read_csv(r'../../CouplingCoeff-XYZmove.csv')

fig = plt.figure(figsize=(9.5, 8))
ax = plt.axes(projection='3d')
ax.set_xlabel('Lateral alignment x [mm]')
ax.set_ylabel('Lateral alignment y [mm]')
ax.set_zlabel('Coil distance z [mm]')

m = ax.scatter(df['xmove'], df['ymove'], df['zmove'], c=df['k'], cmap='RdYlGn', s=100, marker='o',
               depthshade=False, vmin=0, vmax=0.1)

cb = plt.colorbar(m, ax=ax, shrink=0.7)
cb.set_label('Coupling factor [-]')

ax.view_init(7, 52)

fig.show()

plt.tight_layout()

# fig.savefig('couplingfactor.svg', bbox_inches='tight', transparent=True)

# tikzplotlib.save("couplingfactor.tex")

for i in range(len(df['k'])):
    print(f"x: {df['xmove'][i]}, y: {df['ymove'][i]}, z: {df['zmove'][i]}, k = {round(df['k'][i],3)}")