#!/usr/bin/env pyhton3


import math

m = float(input("mass [g]: "))
cm = float(input("cm [mm]: "))
Il = float(input("I_L [gmm2]: "))

# I = sum ( m * r^2 ) = m/2 * dx^2 + m/2 *dx^2 = m*dx^2
dx = math.sqrt(Il/m)
m1x = cm - dx
m2x = cm + dx

print('mass 1: {} g, {} mm'.format(m/2, m1x))
print('mass 2: {} g, {} mm'.format(m/2, m2x))

