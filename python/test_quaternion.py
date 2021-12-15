from pyquaternion import Quaternion

a = Quaternion(1,2,3,4).normalised
b = Quaternion(2,-2,1,1).normalised

q = Quaternion.slerp(a, b, 0.2)

print(q)
