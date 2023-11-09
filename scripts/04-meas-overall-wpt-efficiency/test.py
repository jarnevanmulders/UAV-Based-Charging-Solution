import matplotlib.pyplot as plt
import numpy as np

# Create some sample data
x = np.linspace(0, 10, 100)
y1 = np.sin(x)
y2 = np.cos(x)

# Create the primary Y-axis on the left
fig, ax1 = plt.subplots()
ax1.plot(x, y1, color='blue')
ax1.set_xlabel('X-axis')
ax1.set_ylabel('Y1-axis', color='blue')
ax1.tick_params(axis='y', labelcolor='blue')

# Create a secondary Y-axis on the right
ax2 = ax1.twinx()
ax2.plot(x, y2, color='red')
ax2.set_ylabel('Y2-axis', color='red')
ax2.tick_params(axis='y', labelcolor='red')

plt.title('Dual Y-Axis Plot')
plt.show()