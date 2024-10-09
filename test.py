import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import matplotlib.lines as mlines
import matplotlib.path as mpath

plt.xlim(0, 1)
plt.ylim(0, 1)
plt.grid(True)

ax = plt.gca()
ax.set_aspect("equal")

r = 0.03
circle1 = mpatches.Circle((r, 1 - r), radius=r, fill='True', color='g')
ax.add_patch(circle1)
circle2 = mpatches.Circle((r, r), radius=r, fill=True, color='r')
ax.add_patch(circle2)
plt.savefig("Circle05.png")
for i in range(1, 75):
    circle2 = mpatches.Circle(((i - 1) * 0.0125 + r, (i - 1)*0.0125 + r), radius=r + 0.1 * r, fill=True, color='w')
    ax.add_patch(circle2)
    circle2 = mpatches.Circle((i * 0.0125 + r, i * 0.0125 + r), radius=r + 0.1 * r, fill=True, color='r')
    ax.add_patch(circle2)
    file = './CircleAnimationTest/Circle' + str(i) + '.png'