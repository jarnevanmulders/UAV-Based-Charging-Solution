import pandas as pd
import matplotlib.pyplot as plt

data_set = pd.read_csv('wpt-data.csv')

# for i in range(4):
    # data = data_set[0+i*5:14+i*14]
data = data_set

efficiency = data['vout']*data['iout']/(data['vin']*data['iin'])*100
plt.plot(data['vout']*data['iout'], efficiency)
# , label=f"{data['iout'][0+i*14]} A ({round(data['iout'][0+i*14]*data['vout'][1+i*14]*100)/100} W)")
plt.scatter( data['vout']*data['iout'], efficiency)

# plt.title('Line Plot')
plt.xlabel('Input voltage sweep [V]')
plt.ylabel('Efficiency [%]')
plt.legend()
plt.grid()

plt.show()

import tikzplotlib

def tikzplotlib_fix_ncols(obj):
    """
    workaround for matplotlib 3.6 renamed legend's _ncol to _ncols, which breaks tikzplotlib
    """
    if hasattr(obj, "_ncols"):
        obj._ncol = obj._ncols
    for child in obj.get_children():
        tikzplotlib_fix_ncols(child)

tikzplotlib_fix_ncols(plt.gca().legend_)

# tikzplotlib.save("efficiency-pru-charger.tex")