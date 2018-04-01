from __future__ import print_function
import sys
import random
from os.path import basename

count = int(sys.argv[1])
size = float(sys.argv[2])
range = float(sys.argv[3])
name = "triangles_" + sys.argv[4]

def coord(seed):
	return "{0:.4f}".format(random.uniform(seed - size, seed + size))

def color():
    return "{0:.4f}".format(random.uniform(0.0, 1.0))

def vertexPos(seedX, seedY, seedZ):
    return str(coord(seedX)) + ", " + str(coord(seedY)) + ", " + str(coord(seedZ)) + ", 0.0000"

def vertexCol():
    return str(color()) + ", " + str(color()) + ", " + str(color()) + ", " + str(color())

print("#ifdef LOAD_MODELS")
print("#define TRIANGLES_" + sys.argv[4] + "_COUNT " + sys.argv[1])
print("float " + name + "_vertices[] = {")

for i in xrange(count):
    x = random.uniform(-range, range)
    y = random.uniform(-range, range)
    z = random.uniform(-range, range)
    print("    " + vertexPos(x, y, z) + ", " + vertexPos(x, y, z) + ", " + vertexPos(x, y, z), end="")
    if (i + 1) < count:
        print(",")
    else:
        print("")

print("};\n")
print("float " + name + "_colors[] = {")

for i in xrange(count):
    print("    " + vertexCol() + ", " + vertexCol() + ", " + vertexCol(), end="")
    if (i + 1) < count:
        print(",")
    else:
        print("")

print("};")
print("#endif")
