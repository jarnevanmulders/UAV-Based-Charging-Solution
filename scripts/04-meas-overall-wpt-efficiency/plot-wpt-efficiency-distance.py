import pandas as pd
import matplotlib.pyplot as plt

data_set = pd.read_csv('wpt-data-distance-sweep.csv')

# for i in range(4):
    # data = data_set[0+i*5:14+i*14]
data = data_set

efficiency = data['vout']*data['iout']/(data['vin']*data['iin'])*100

fig, ax1 = plt.subplots()
ax1.plot(data['distance'], efficiency, label=f"P_out = {round(data['vout'][0]*data['iout'][0]*100)/100} W", color='#1f77b4')
ax1.set_xlabel('Vertical coil distance [mm]')
ax1.scatter(data['distance'], efficiency, color='#1f77b4')
ax1.set_ylabel('Efficiency [%]', color='#1f77b4')
ax1.tick_params(axis='y', labelcolor='#1f77b4')
ax1.grid()

# plt.plot(data['distance'], efficiency, label=f"P_out = {round(data['vout'][0]*data['iout'][0]*100)/100} W")
# plt.scatter(data['distance'], efficiency)

ax2 = ax1.twinx()
ax2.plot(data['distance'], data['vin'], label=f"P_out = {round(data['vout'][0]*data['iout'][0]*100)/100} W", color='#ff7f0e')
ax2.scatter(data['distance'], data['vin'], color='#ff7f0e')
ax2.set_ylabel('Inverter input [V]', color='#ff7f0e')
ax2.tick_params(axis='y', labelcolor='#ff7f0e')
ax2.grid()

ax1_yticks = ax1.get_yticks()
ax2.set_yticks(ax1_yticks/2)

ax1.set_ylim(10, 40)
ax2.set_ylim(5, 20)

# Disable Y-grid lines for the secondary Y-axis
ax2.grid(False)


# plt.show()


import tikzplotlib

def tikzplotlib_fix_ncols(obj):
    """
    workaround for matplotlib 3.6 renamed legend's _ncol to _ncols, which breaks tikzplotlib
    """
    if hasattr(obj, "_ncols"):
        obj._ncol = obj._ncols
    for child in obj.get_children():
        tikzplotlib_fix_ncols(child)

# tikzplotlib_fix_ncols(plt.gca().legend_)

tikzplotlib.save("efficiency-wpt.tex")