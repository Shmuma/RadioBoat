#!/usr/bin/env python

import math

l = 0.3                           # distance between back wheel and mass center in meters
alpha = 20.0                      # angle of wheel
vel = 0.1                         # velocity in m/s
delta_t = 0.1                     # time interval in seconds
max_t = 60                        # time to simulate

# consts
sin_a = math.sin (math.radians (alpha))
cos_a = math.cos (math.radians (alpha))

# state
A = (0.0, 0.0)                    # back wheel initial position
O = (0.0, l)                      # mass center initial position
phi = 0.0                         # initial angle in degrees

t = 0

A_data = []
O_data = []
Phi = []


def dump_pts (name, pts):
    f = open (name, 'w')
    for ox,oy in pts:
        f.write ("%f %f\n" % (ox, oy))
    f.close ()
    

while t < max_t:
    t += delta_t
    A = (O[0] + l * math.sin (math.radians (-phi)),
         O[1] - l * math.cos (math.radians (-phi)))
    O = (O[0] + delta_t * vel * cos_a * math.sin (math.radians (phi)),
         O[1] + delta_t * vel * cos_a * math.cos (math.radians (phi)))
    phi += -math.degrees (delta_t * vel * sin_a / l)
    print "%.2f: phi=%.2f, A=(%.4f, %.4f), O=(%.4f, %.4f)" %  (t, phi, A[0], A[1], O[0], O[1])
    O_data.append (O)
    A_data.append (A)
    Phi.append ((t, phi))

dump_pts ("O.dat", O_data)
dump_pts ("A.dat", A_data)
dump_pts ("Phi.dat", Phi)
