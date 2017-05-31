import math

class Quaternion:
    def __init__(self, q=1.0):
        if hasattr(q, '__len__'):
            if len(q) == 4:
                self.q = tuple(q)
            else:
                raise TypeError("4 elements expected")
        else:
            self.q = (float(q), 0, 0, 0)

    def __str__(self):
        return "( {} + {} i + {} j + {} k )".format(*self.q)

    def __repr__(self):
        return self.__str__()

    def __add__(self, other):
        if isinstance(other, Quaternion):
            return Quaternion([a + b for a,b in zip(self.q, other.q)])
        else:
            raise TypeError("addition is only supported between quaternions")

    def __mul__(self, other):
        if isinstance(other, Quaternion):
            q1 = self.q
            q2 = other.q
            q = [None] * 4
            q[0] = q1[0]*q2[0] - q1[1]*q2[1] - q1[2]*q2[2] - q1[3]*q2[3];
            q[1] = q1[0]*q2[1] + q1[1]*q2[0] + q1[2]*q2[3] - q1[3]*q2[2];
            q[2] = q1[0]*q2[2] - q1[1]*q2[3] + q1[2]*q2[0] + q1[3]*q2[1];
            q[3] = q1[0]*q2[3] + q1[1]*q2[2] - q1[2]*q2[1] + q1[3]*q2[0];
            return Quaternion(q)
        else:
            return Quaternion([i * other for i in self.q])

    def __truediv__(self, other):
        if isinstance(other, Quaternion):
            self * other.inverse()
        else:
            return Quaternion([i / other for i in self.q])

    def inverse(self):
        q = self.conjugated()
        normsq = sum([i**2 for i in q])
        return q / normsq

    def conjugated(self):
        return Quaternion((self.q[0], -self.q[1], -self.q[2], -self.q[3]))

    def norm(self):
        return math.sqrt(sum([i**2 for i in self.q]))

    def normalized(self):
        norm = math.sqrt(sum([i**2 for i in self.q]))
        return Quaternion(self.q) / norm
