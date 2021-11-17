import numpy as np
import matplotlib.pyplot as plt
import scipy.ndimage



# Part 1 #
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

    gx /= np.sum(np.abs(gx))
    gy /= np.sum(np.abs(gy))

    return gx, gy



def Harris(I, sigma1=1., sigmaD=0.7, alpha=0.05):
    g = gauss(sigma1)
    gx, gy = gaussDeriv2D(sigmaD)
    Ix = scipy.ndimage.convolve(I, gx, mode="nearest")
    Iy = scipy.ndimage.convolve(I, gy, mode="nearest")
    
    Ixx = scipy.ndimage.convolve(Ix*Ix, g, mode="nearest")
    Iyy = scipy.ndimage.convolve(Iy*Iy, g, mode="nearest")
    Ixy = scipy.ndimage.convolve(Ix*Iy, g, mode="nearest")
    
    det = Ixx*Iyy - Ixy*Ixy
    trace = Ixx + Iyy
    
    R = det - alpha * trace*trace

    return R


I = plt.imread("checker.png").astype(float) * 255.
R = Harris(I)

print(R[16:24,16:24])
R[R<1.e6] = 0.
R /= np.max(R)

plt.figure()
plt.imshow(R, cmap="gray")
plt.imsave("HarrisR.bmp", R, cmap="gray")

# NMS
X = []
Y = []
R_NMS = R.copy()
for j in range(1, R.shape[0]-1):
    for i in range(1, R.shape[1]-1):
        lm = R[j,i] > R[j-1:j+2,i-1:i+2]
        if np.sum(lm) == 8:
            R_NMS[j,i] = R[j,i]
            X.append(i)
            Y.append(j)
plt.figure()
plt.imshow(I, cmap="gray")
plt.plot(X, Y, "r+", markersize=8)
plt.savefig("NMSR.png")
 



 


# Part 2

def countPixel(b):
    N = b.shape[0]
    indices_0 = []
    for i in range(N):
        if b[i]==0.:
            indices_0.append(i)
    if len(indices_0)==0:
        if np.unique(b).shape[0]==1:
            return N
        else:
            lmax = 0
            l = 1
            for n in range(1, N):
                if b[n]==b[n-1]:
                    l += 1
                else:
                    if l > lmax:
                        lmax = l
                    l = 1
            return lmax

    c = np.roll(b, -indices_0[0])
    lmax = 0
    for i,ist in enumerate(indices_0):
        i0 = ist - indices_0[0] + 1
        if i < len(indices_0)-1:
            i1 = indices_0[i+1]
        else:
            i1 = N
        l = 1
        for n in range(i0 + 1, i1):
            if c[n]==c[n-1]:
                l += 1
            else:
                if l > lmax:
                    lmax = l
                l = 1
    return lmax







def FAST(I, T=10, nst=9):
    indices = [(0,3), (1,3), (2,2), (3,2), 
               (3,0), (3,-1), (2,-2), (1,-3),
               (0,-3), (-1,-3), (-2,-2), (-3,-1),
               (-3,0), (-3,1), (-2,2), (-1,3)]

    result = np.zeros_like(I)
    X = []
    Y = []

    b = np.zeros((16))
    c = np.zeros((16))
    for j in range(3,I.shape[0]-3):
        for i in range(3,I.shape[1]-3):
            for count in range(16):
                b[count] = I[j+indices[count][1], i+indices[count][0]]
            c[:] = 0.
            c[b < I[j,i] - T] = 1.
            c[b > I[j,i] + T] = 2.
            n = countPixel(c)
            if n >= nst:
                result[j,i] = 1.
                X.append(i)
                Y.append(j)

    return X, Y


I = plt.imread("tower.png").astype(float) * 255.
for T in (10, 20, 30, 50):
    X, Y = FAST(I, T=T)
    plt.figure()
    plt.imshow(I, cmap="gray")
    plt.plot(X, Y, "r.")
    filename = "FAST_T" + T.__str__() + ".png"
    plt.savefig(filename)
 



    
