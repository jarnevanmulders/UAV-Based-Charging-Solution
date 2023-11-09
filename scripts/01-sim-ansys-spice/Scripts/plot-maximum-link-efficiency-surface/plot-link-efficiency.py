from typing import List

import numpy as np
import matplotlib.pyplot as plt

import tikzplotlib

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import pandas as pd

df = pd.read_csv(r'../pyspice-link-efficiency-simulation/output-efficiencies.csv')

print(df['calc_eff_max'])

fig = plt.figure()#figsize=(9.5, 8))

ax = plt.axes(projection='3d')
# ax.set_xlabel('Lateral alignment x [mm]')
# ax.set_ylabel('Lateral alignment y [mm]')
# ax.set_zlabel('Coil distance z [mm]')

# m = ax.scatter(df['xmove'], df['ymove'], df['zmove'], c=df['sim_eff_max'] * 100, cmap='RdYlGn', s=100, marker='o',
#                depthshade=False, vmin=0, vmax=100)

zmove = 200

# Create a grid of values
X = np.asarray(df[df['zmove'] == zmove]['xmove']).reshape(5, 5)
Y = np.asarray(df[df['zmove'] == zmove]['ymove']).reshape(5, 5)

# Define a function to plot (in this case, a simple quadratic function)
Z = np.asarray(df[df['zmove'] == zmove]['calc_eff_max']*100).reshape(5, 5)

# Create a 3D plot
# fig = plt.figure()
# ax = fig.add_subplot(111, projection='3d')

# Plot the surface
m = ax.plot_surface(X, Y, Z, cmap='RdYlGn')

# Add labels
ax.set_xlabel('Lateral alignment x [mm]')
ax.set_ylabel('Lateral alignment y [mm]')
ax.set_zlabel('Max. link efficiency [%]')

# Set step values for the X and Y axis labels
ax.set_xticks(np.arange(-100, 101, 50))  # Specify the tick locations with a step of 50
ax.set_yticks(np.arange(-100, 101, 50))  # Specify the tick locations with a step of 50

# Set the title for the plot
# ax.set_title(f'Vertical misalignment of {zmove} mm')

cb = plt.colorbar(m, ax=ax, shrink=0.8)
cb.set_label('Max. link efficiency [%]')

ax.view_init(20, 40)

plt.tight_layout()

# fig.show()

fig.savefig(f'fem-link-efficiency-zmove-{zmove}.pdf', bbox_inches='tight', transparent=True)

# tikzplotlib.save("couplingfactor.tex")

# print('ok')
#
# for i in range(len(df['sim_eff_max'])):
#     if float(df['sim_eff_max'][i]) > 0.15:
#         print(str(i) + ' -- eta = ' + str(df['sim_eff_max'][i]) + ' -- x: ' + str(df['xmove'][i]) + ' | y: ' + str(df['ymove'][i]) + ' | z: ' + str(df['zmove'][i]))
