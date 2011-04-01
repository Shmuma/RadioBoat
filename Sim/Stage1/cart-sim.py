#!/usr/bin/env python

import math

l = 0.3                           # distance between back wheel and mass center in meters
alpha = 10.0                      # angle of wheel
vel = 0.1                         # velocity in m/s
delta_t = 0.1                     # time interval in seconds
max_t = 5                         # time to simulate

# consts
sin_a = math.sin (math.radians (alpha))
cos_a = math.cos (math.radians (alpha))

# state
A = (0.0, 0.0)                    # back wheel initial position
O = (0.0, l)                      # mass center initial position
phi = 0.0                         # initial angle in degrees

t = 0

A_x = A_y = []
O_x = A_y = []
Phi = []


while t < max_t:
    t += delta_t
    phi = math.degrees (math.asin (t * vel * sin_a * sin_a))
    A = (t * vel * math.sin (math.radians (alpha + phi)), 
         t * vel * math.cos (math.radians (alpha + phi)))
    O = (t * vel * cos_a * math.sin (math.radians (phi)),
         t * vel * cos_a * math.cos (math.radians (phi)))

    print "%.2f: phi=%.2f, A=(%.4f, %.4f), O=(%.4f, %.4f)" %  (t, phi, A[0], A[1], O[0], O[1])
    Phi.append (phi)

print
print "Phi = " + str (Phi)
