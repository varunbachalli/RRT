import matplotlib.pyplot as plt

def plot_path(path):
    x = []
    y = []
    for point in path:
        x.append(point[0])
        y.append(point[1])
        # plt.plot(point[0], point[1], 'bo')
    plt.plot(x,y)

f = open("../build/output.txt")
lines = f.readlines()
init = lines[0].split(',')
startx = float(init[0])
starty = float(init[1])
endx = float(init[2])
endy = float(init[3])
plt.figure()
plt.plot(startx,starty,"k*")
plt.plot(endx, endy, "r*")

path = []
paths = []
for line in lines[1:]:
    if("new line" in line):
        paths.append(path)
        path = []
        continue
    points = line.split(',')
    path.append([float(points[0]),float(points[1])])
f.close()

print(paths)
for path in paths:
    plot_path(path)

plt.show()