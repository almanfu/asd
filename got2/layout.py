import matplotlib.pyplot as plt

def read_points(filename):
    points = []
    with open(filename, 'r') as file:
        for line in file:
            points.append([float(i) for i in line.split()])
    return points

def plot_points(points):
    for i, point in enumerate(points, start=1):
        plt.scatter(*point)
        plt.text(point[0], point[1], str(i), color="red", fontsize=12)
    plt.show()

points = read_points('info.txt')
plot_points(points)