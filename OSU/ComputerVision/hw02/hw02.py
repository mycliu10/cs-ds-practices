import matplotlib.pyplot as plt
import numpy as np
import scipy.ndimage
import skimage.color
import skimage.feature

# ----------------------------------------------- #
filename_grey = "affleck_gray.png"


# Part 1 #
img_grey = plt.imread(filename_grey)

#plt.imshow(img_grey, cmap="gray")
#plt.show()

sigma_list = [20., 10., 5., 2., 1., 0.5]
for count, sigma in enumerate(sigma_list):
    print("sigma = ", sigma)
    N = int(2 * np.ceil(3*sigma) + 1)
    G = np.zeros((N,N))
    G[(N-1)//2,(N-1)//2] = 1.
    G = scipy.ndimage.gaussian_filter(G, sigma)
    img = scipy.ndimage.convolve(img_grey, G, mode="nearest")
    plt.figure()
    plt.imshow(img, cmap="gray")
    filename = "".join( ("affleck_gray_", count.__str__(), ".jpg") )
    plt.imsave(filename, img, cmap="gray")


# Part 2 #
def gauss(sigma):
    N = int(2 * np.ceil(3*sigma) + 1)
    x = np.zeros((N,N))
    y = np.zeros((N,N))
    for i in range(N):
        for j in range(N):
            x[i,j] = i - (N-1)/2
            y[i,j] = j - (N-1)/2 
    g = 1. / 2.*np.pi / sigma**2 * np.exp(-(x**2+y**2)/2./sigma**2 )

    g /= np.sum(g)

    return g

def gaussDeriv2D(sigma):
    N = int(2 * np.ceil(3*sigma) + 1)
    x = np.zeros((N,N))
    y = np.zeros((N,N))
    for i in range(N):
        for j in range(N):
            x[j,i] = i - (N-1)/2
            y[j,i] = j - (N-1)/2 
    gx = x / 2.*np.pi / sigma**4 * np.exp(-(x**2+y**2)/2./sigma**2 )
    gy = y / 2.*np.pi / sigma**4 * np.exp(-(x**2+y**2)/2./sigma**2 )

    return gx, gy


sigma = 5.0
Gx, Gy = gaussDeriv2D(sigma)
N = Gx.shape[0]
plt.figure()
plt.plot(Gx[(N-1)//2,:])
plt.savefig("Gx_1D.png")

plt.figure()
plt.contour(Gx)
plt.savefig("Gx_2D.png")

plt.figure()
plt.plot(Gy[:,(N-1)//2])
plt.savefig("Gy_1D.png")

plt.figure()
plt.contour(Gy)
plt.savefig("Gy_2D.png")

# Part 3 #
img = plt.imread("doggy.jpg")
img = skimage.color.rgb2gray(img)
gxIm = scipy.ndimage.convolve(img, Gx, mode="nearest")
gyIm = scipy.ndimage.convolve(img, Gy, mode="nearest")
magIm = np.sqrt( gxIm**2 + gyIm**2 )
plt.figure()
plt.imsave("doggy_x.jpg", gxIm, cmap="gray")
plt.figure()
plt.imsave("doggy_y.jpg", gyIm, cmap="gray")
plt.figure()
plt.imsave("doggy_mag.jpg", magIm, cmap="gray")


# Part 4 #
T_max = np.max(magIm)
T_list = [T_max/2., T_max/4., T_max/8.]
for count, T in enumerate(T_list):
    print("T = ", T)
    tIm = magIm > T
    plt.figure()
    filename = "".join( ("T_", count.__str__(), ".jpg") )
    plt.imsave(filename, tIm, cmap="gray")


# Part 5 #
N = int(2 * np.ceil(3*sigma) + 1)
S = np.zeros((N,N))
S[(N-1)//2,(N-1)//2] = 1.
Sx = -scipy.ndimage.sobel(S, axis=1)
Sy = -scipy.ndimage.sobel(S, axis=0)
 
sxIm = scipy.ndimage.convolve(img, Sx, mode="nearest")
syIm = scipy.ndimage.convolve(img, Sy, mode="nearest")
magIm = np.sqrt( sxIm**2 + syIm**2 )
T = T_max / 4.
tIm = magIm > T
plt.figure()
plt.imsave("doggy_sobel.jpg", tIm, cmap="gray")


# Part 6 #
edges = skimage.feature.canny(img, sigma=sigma)
plt.imsave("doggy_canny.jpg", edges, cmap="gray")

