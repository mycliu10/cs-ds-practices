import matplotlib.pyplot as plt
import numpy as np
from skimage import color

# ----------------------------------------------- #
filename_grey = "buckeyes_gray.bmp"
filename_rgb  = "buckeyes_rgb.bmp"


# Part 1 #
img_grey = plt.imread(filename_grey)
plt.imshow(img_grey, cmap="gray", aspect="equal")
plt.show()
plt.imsave("buckeyes_gray.jpg", img_grey, cmap="gray")

img_rgb = plt.imread(filename_rgb)
plt.imshow(img_rgb, aspect="equal")
plt.show()
plt.imsave("buckeyes_rgb.jpg", img_rgb)


# Part 2 #
img_grey_converted = color.rgb2gray(img_rgb)
plt.imshow(img_grey_converted, cmap="gray", aspect="equal")
plt.show()
plt.imsave("buckeyes_gray_convertded.bmp", img_grey_converted, cmap="gray")


# Part 3 #
zblock = np.zeros([10,10])
oblock = np.ones([10,10]) * 255
r1 = np.concatenate( [zblock,oblock], axis=1 )
r2 = np.concatenate( [oblock,zblock], axis=1 )
pattern = np.concatenate( [r1,r2], axis=0 )
img_checker = np.tile(pattern, [5,5])
plt.imshow(img_checker, cmap="gray")
plt.show()
plt.imsave("checkerIm.bmp", img_checker, cmap="gray")


