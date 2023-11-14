import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import curve_fit


def fit_function(x, a, b, c, d, e):
    return a * x**4 + b * x**3 + c * x**2 + d * x + e

data = np.asarray([[0.147390714, 0.075932284, 0.027916606, 0.011778231, 0.005789029],
                   [0.245296167, 0.110924996, 0.04028655, 0.017253615, 0.008581866],
                   [0.396725941, 0.142531452, 0.051778514, 0.0226555, 0.0114453],
                   [0.775946757, 0.165203676, 0.061508011, 0.027688345, 0.014255924]])

name = ["Receive coil \SI{750}{\milli\meter}", "Receive coil \SI{100}{\milli\meter}",
        "Receive coil \SI{1250}{\milli\meter}", "Receive coil \SI{1500}{\milli\meter}"]

distance = np.asarray([1, 50, 100, 150, 200])

for i in range(4):
    x_smooth = np.linspace(distance.min(), distance.max(), 10)
    params, covariance = curve_fit(fit_function, distance, data[i])
    y_smooth = fit_function(x_smooth, *params)
    plt.plot(x_smooth, y_smooth, label=name[i])
    # plt.scatter(x_smooth, y_smooth)
    # plt.plot(distance, data[i], label=name[i])
    plt.scatter(distance, data[i])

plt.xlabel('Vertical distance [mm]')
plt.ylabel('Coupling factor (k) [-]')
plt.legend()
plt.grid()

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


tikzplotlib_fix_ncols(plt.gca().legend_)

tikzplotlib.save("different-receive-coils.tex")
