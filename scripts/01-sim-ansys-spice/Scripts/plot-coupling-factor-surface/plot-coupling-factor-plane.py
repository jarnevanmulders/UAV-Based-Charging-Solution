import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import pandas as pd

df = pd.read_csv(r'../../CouplingCoeff-XYZmove-missing-values-added.csv')

# fig = plt.figure(figsize=(9.5, 8))
# ax = plt.axes(projection='3d')

zmove = 200

# print(df[df['zmove'] == zmove]['xmove'])

# Create a grid of values
X = np.asarray(df[df['zmove'] == zmove]['xmove']).reshape(5, 5) #np.linspace(-5, 5, 100)
Y = np.asarray(df[df['zmove'] == zmove]['ymove']).reshape(5, 5) #np.linspace(-5, 5, 100)
# X, Y = np.meshgrid(x, y)

# Define a function to plot (in this case, a simple quadratic function)
Z = np.asarray(df[df['zmove'] == zmove]['k']).reshape(5, 5)

# print(X, Y, Z)

# Create a 3D plot
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Plot the surface
m = ax.plot_surface(X, Y, Z, cmap='viridis')

# Add labels
ax.set_xlabel('Lateral alignment x [mm]')
ax.set_ylabel('Lateral alignment y [mm]')
ax.set_zlabel('Coupling factor (k) [-]')

# Set step values for the X and Y axis labels
ax.set_xticks(np.arange(-100, 101, 50))  # Specify the tick locations with a step of 50
ax.set_yticks(np.arange(-100, 101, 50))  # Specify the tick locations with a step of 50

# Set the title for the plot
# ax.set_title(f'Vertical misaligment of {zmove} mm')

cb = plt.colorbar(m, ax=ax, shrink=0.7)
cb.set_label('Coupling factor (k) [-]')

ax.view_init(20, 40)

plt.tight_layout()

# Show the plot
# plt.show()

fig.savefig(f'fem-coupling-zmove-{zmove}.pdf', bbox_inches='tight', transparent=True)
