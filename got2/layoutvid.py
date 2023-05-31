import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np
import os

file_path = "layout/"
files = [str(r)+".txt" for r in range(100)]

# Calculate global min and max for x and y across all files
global_x_min = np.inf
global_x_max = -np.inf
global_y_min = np.inf
global_y_max = -np.inf

for file in files:
    data = np.loadtxt(file_path + file)
    x_min, x_max = min(data[:, 0]), max(data[:, 0])
    y_min, y_max = min(data[:, 1]), max(data[:, 1])
    if x_min < global_x_min:
      global_x_min = x_min
    if x_max > global_x_max:
      global_x_max = x_max
    if y_min < global_y_min:
      global_y_min = y_min
    if y_max > global_y_max:
      global_y_max = y_max

fig, ax = plt.subplots()
scatter = ax.scatter([], [])

# set global axis limits
ax.set_xlim(global_x_min - 1, global_x_max + 1)
ax.set_ylim(global_y_min - 1, global_y_max + 1)

def init():
    scatter.set_offsets(np.empty((0, 2)))  # Set an empty 2D array.
    return scatter,

def update(num):
    data = np.loadtxt(file_path + files[num])
    scatter.set_offsets(data)


    # add labels to each point
    for i in range(data.shape[0]):
      ax.text(data[i, 0], data[i, 1], str(i), color="red", fontsize=8)
    return scatter,ax

ani = FuncAnimation(fig, update, frames=len(files), init_func=init, blit=True, repeat=True)

plt.show()