from typing import List

import numpy as np
import matplotlib.pyplot as plt

# import tikzplotlib

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import pandas as pd


def hex_to_RGB(hex):
    ''' "#FFFFFF" -> [255,255,255] '''
    # Pass 16 to the integer function for change of base
    return [int(hex[i:i + 2], 16) for i in range(1, 6, 2)]


def RGB_to_hex(RGB):
    ''' [255,255,255] -> "#FFFFFF" '''
    # Components need to be integers for hex to make sense
    RGB = [int(x) for x in RGB]
    return "#" + "".join(["0{0:x}".format(v) if v < 16 else
                          "{0:x}".format(v) for v in RGB])


def color_dict(gradient):
    """ Takes in a list of RGB sub-lists and returns dictionary of
    colors in RGB and hex form for use in a graphing function
    defined later on """
    return {"hex": [RGB_to_hex(RGB) for RGB in gradient],
            "r": [RGB[0] for RGB in gradient],
            "g": [RGB[1] for RGB in gradient],
            "b": [RGB[2] for RGB in gradient]}


def linear_gradient(start_hex, finish_hex="#FFFFFF", n=10):
    """ Returns a gradient list of (n) colors between
    two hex colors. start_hex and finish_hex
    should be the full six-digit color string,
    including the number sign ("#FFFFFF") """
    # Starting and ending colors in RGB form
    s = hex_to_RGB(start_hex)
    f = hex_to_RGB(finish_hex)
    # Initialize a list of the output colors with the starting color
    RGB_list = [s]
    # Calculate a color at each evenly spaced value of t from 1 to n
    for t in range(1, n):
        # Interpolate RGB vector for color at the current value of t
        curr_vector = [
            int(s[j] + (float(t) / (n - 1)) * (f[j] - s[j]))
            for j in range(3)
        ]
        # Add it to our list of output colors
        RGB_list.append(curr_vector)

    return color_dict(RGB_list), RGB_list


df = pd.read_csv(r'../../CouplingCoeff-XYZmove.csv')

color_list, RGB_list = linear_gradient("#ff2689", "#09EA69", len(df['k']))

RGB_list[:] = [[item / 255 for item in subl] for subl in RGB_list]

print(df)

new_df = df.sort_values(by='k')

print(RGB_list)

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# m = ax.scatter(new_df['xmove'], new_df['ymove'], new_df['zmove'], c=RGB_list, marker='^', depthshade=True)

m = ax.scatter(new_df['xmove'], new_df['ymove'], new_df['zmove'], c=new_df['k'], cmap='RdBu', marker='o', depthshade=False)

cb = plt.colorbar(m)
cb.set_label('Coupling factor [-]')

fig.show()

