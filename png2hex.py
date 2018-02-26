from __future__ import print_function
from PIL import Image
import sys
from os.path import basename

img = Image.open(sys.argv[1])
pixels = img.load()

def hx(val):
	return format(val, 'x')

name = basename(img.filename)[:-4]

print("texture_t texture_" + name + " = {")
print("    " + str(img.width) + ", " + str(img.height) + ",")
print("    {")

for y in range(img.height):
	print("        ", end="");
	for x in range(img.width):
		pixel = pixels[x, y]
		print("0x" + hx(pixel[3]) + hx(pixel[0]) + hx(pixel[1]) + hx(pixel[2]) + ("" if (x == (img.width - 1) and y == (img.height - 1)) else ", "), end="")
	print("")
print("    }")
print("};")
