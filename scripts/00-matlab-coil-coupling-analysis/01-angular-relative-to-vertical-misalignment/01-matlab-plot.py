import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import pandas as pd

df = pd.read_csv(r'k_RX2_ifv_angle.csv')

# fig = plt.figure(figsize=(9.5, 8))
# ax = plt.axes(projection='3d')

num_columns = df.shape[1]
num_rows = df.shape[0]

print(num_columns, num_rows)

# print(np.asarray(df['y']))

X = np.repeat(np.linspace(0, num_columns-2, num_columns-1)*5, num_rows).reshape((num_columns-1, -1))

Y = np.transpose(np.repeat(np.asarray(df['y']), num_columns-1).reshape((num_rows, -1)))

Z = np.asarray(df[f'{0}'])

for i in range(1, num_columns-1):
    new_row = np.asarray(df[f'{i * 5}'])
    Z = np.concatenate((Z, new_row), axis=0)

Z = Z.reshape(num_columns-1, num_rows)

print(X.shape)
print(Y.shape)
print(Z.shape)

num_columns = df.shape[1]



zmove = 200

# print(df[df['zmove'] == zmove]['xmove'])

# Create a grid of values
# X = np.asarray(df[df['zmove'] == zmove]['xmove']).reshape(5, 5) #np.linspace(-5, 5, 100)
# Y = np.asarray(df[df['zmove'] == zmove]['ymove']).reshape(5, 5) #np.linspace(-5, 5, 100)
# X, Y = np.meshgrid(x, y)

# Define a function to plot (in this case, a simple quadratic function)
# Z = np.asarray(df[df['zmove'] == zmove]['k']).reshape(5, 5)

print(X, Y, Z)

# Create a 3D plot
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Plot the surface
m = ax.plot_surface(X, Y, Z, cmap='viridis')

# Add labels
ax.set_xlabel('Angular misalignment [°]')
ax.set_ylabel('Vertical distance [mm]')
ax.set_zlabel('Coupling factor (k) [-]')

# Set step values for the X and Y axis labels
# ax.set_xticks(np.arange(-100, 101, 50))  # Specify the tick locations with a step of 50
# ax.set_yticks(np.arange(-100, 101, 50))  # Specify the tick locations with a step of 50

# Set the title for the plot
# ax.set_title(f'Vertical misaligment of {zmove} mm')

cb = plt.colorbar(m, ax=ax, shrink=0.7)
cb.set_label('Coupling factor (k) [-]')

ax.view_init(20, 40)

plt.tight_layout()

# Show the plot
# plt.show()

fig.savefig(f'angular-relative-to-vertical-misalignment.pdf', bbox_inches='tight', transparent=True)
