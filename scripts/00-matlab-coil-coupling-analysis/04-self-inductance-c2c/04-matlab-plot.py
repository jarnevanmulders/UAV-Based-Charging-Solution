import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import curve_fit


def fit_function(x, a, b, c, d, e):
    return a * x ** 4 + b * x ** 3 + c * x ** 2 + d * x + e


c_data = np.asarray([[0.147390714, 0.075932284, 0.027916606, 0.011778231, 0.005789029],
                     [0.245296167, 0.110924996, 0.04028655, 0.017253615, 0.008581866],
                     [0.396725941, 0.142531452, 0.051778514, 0.0226555, 0.0114453],
                     [0.775946757, 0.165203676, 0.061508011, 0.027688345, 0.014255924]])

l_rx_data = np.asarray([[1.8628, 1.8932, 1.9027, 1.9039, 1.9041],
                        [2.7314, 2.8705, 2.9016, 2.9054, 2.9061],
                        [3.3518, 3.897, 3.9672, 3.9758, 3.9773],
                        [2.0308, 4.9643, 5.0843, 5.0997, 5.1026]])

l_tx_data = np.asarray([[1.5527, 1.578, 1.5859, 1.5869, 1.5871],
                        [1.4917, 1.5676, 1.5846, 1.5867, 1.587],
                        [1.3374, 1.5549, 1.5829, 1.5863, 1.587],
                        [0.6315, 1.5438, 1.5812, 1.5859, 1.5868]])

name = ["Receive coil \SI{750}{\milli\meter}", "Receive coil \SI{100}{\milli\meter}",
        "Receive coil \SI{1250}{\milli\meter}", "Receive coil \SI{1500}{\milli\meter}"]

distance = np.asarray([1, 50, 100, 150, 200])

# styles = ['solid', 'dotted', (0, (5, 5)), (0, (3, 1, 1, 1))]
styles = ['solid', 'dotted', 'dashed', ':']


fig, ax1 = plt.subplots()

for i in range(4):
    x_smooth = np.linspace(distance.min(), distance.max(), 10)
    params, covariance = curve_fit(fit_function, distance, c_data[i])
    y_smooth = fit_function(x_smooth, *params)
    ax1.plot(x_smooth, y_smooth, label=name[i], color='#1f77b4')#, linestyle=styles[i])
    # plt.scatter(x_smooth, y_smooth)
    # plt.plot(distance, data[i], label=name[i])
    ax1.scatter(distance, c_data[i], color='#1f77b4')

# ax1.legend()

ax1.tick_params(axis='y', labelcolor='#1f77b4')
ax1.set_ylabel('Coupling factor (k) [-]')
ax1.grid()

ax2 = ax1.twinx()

for i in range(4):
    ax2.plot(distance, l_rx_data[i], color='#ff7f0e')#, linestyle=styles[i])
    # ax2.plot(distance, l_tx_data[i], color='#2ca02c', linestyle=styles[i])
    ax2.scatter(distance, l_rx_data[i], color='#ff7f0e')

ax2.tick_params(axis='y', labelcolor='#ff7f0e')
ax2.set_ylabel('Selfinductance [\micro\henri]')
ax2.grid()

plt.xlabel('Vertical distance [mm]')
# plt.ylabel('Coupling factor (k) [-]')
# ax2.legend(loc='upper center', bbox_to_anchor=(0.5, -0.15), ncol=2)

# plt.grid()

ax1_yticks = ax1.get_yticks()
ax2.set_yticks(ax1_yticks*6)

ax1.set_ylim(0, 1)
ax2.set_ylim(0, 6)


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

tikzplotlib.save("c-l-different-receive-coils.tex")
