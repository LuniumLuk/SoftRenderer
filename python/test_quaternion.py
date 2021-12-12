from pyquaternion import Quaternion

q = Quaternion(1, 2, 3, 4)
for i in range(100):
    q = q * Quaternion(2, 1, 1, 1)
    q = q.normalised
    print(q)