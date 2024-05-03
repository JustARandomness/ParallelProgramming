import numpy as np
import matplotlib.pyplot as plt

fig, ax = plt.subplots()

# Открываем файл с данными
with open('points.txt', 'r') as file:
    # Читаем строки файла и разделяем их на пары значений
    data = [tuple(map(float, line.strip().split(','))) for line in file]

center_of_mass = (-2.468750, 2.083333)
center_of_masses = [(-11.833333,13.916667), (-13.000000,22.250000), (-5.000000,-10.916667), (14.666667,1.500000), (-12.000000,-2.500000), (14.833333,1.166667), (-5.166667,6.166667), (-2.250000,-14.916667)]

for elem in data:
    ax.plot(elem[0], elem[1], color='blue', marker='.')

ax.plot(center_of_mass[0], center_of_mass[1], color='red', marker='.')
print(list(center_of_masses[0]))
for i in range(len(center_of_masses)):
    new_list = list(center_of_masses[i])
    ax.scatter(new_list[0], new_list[1], color='green')
plt.show()
