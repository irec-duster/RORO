#!/usr/bin/env python3

import numpy as np
from quaternion import Quaternion
from math import sin, cos, asin, acos


class Attitude:
    def __init__(self, mode='constant'):
        self.rot_quat = Quaternion([1,0,0,0])
        self.mode = mode

    def integration_step(self, omega, delta_t):
        d_theta = np.linalg.norm(omega) * delta_t
        omega_normal = np.array(omega) / np.linalg.norm(omega)
        if self.mode == 'euler':
            v = d_theta / 2 * omega_normal
            delta_rot = Quaternion([0] + list(v))
            self.rot_quat = (self.rot_quat + self.rot_quat * delta_rot).normalized()
        elif self.mode == 'constant':
            v = sin(d_theta/2) * omega_normal
            delta_rot = Quaternion([cos(d_theta/2)] + list(v))
            self.rot_quat = (self.rot_quat * delta_rot).normalized()
        elif self.mode == 'RK4':
            pass
        else:
            raise Exception("unknown integration mode")

    def get_quaternion(self):
        return self.rot_quat.q

    def get_rotation_vector_angle(self):
        angle = acos(self.rot_quat.q[0]) * 2
        v = np.array(self.rot_quat.q[1:])
        v = v / np.linalg.norm(v)
        return (v, angle)

    def local_to_global(self, vector):
        vector = q * Quaternion() # todo
        return vector

