import matplotlib.pyplot as plt
import numpy as np
import scipy.linalg




# Part 1 #
img_raw = plt.imread("target.jpg")
img_t = plt.imread("target.jpg").astype(float)
nr = img_t.shape[0]
nc = img_t.shape[1]

nr_w = 70
nc_w = 24
x = np.zeros((nr_w,nc_w))
y = np.zeros((nr_w,nc_w))
for i in range(nr_w):
    y[i,:] = i
for j in range(nc_w):
    x[:,j] = j

nr_d = nr - nr_w + 1
nc_d = nc - nc_w + 1

distance = np.zeros((nr_d,nc_d))

modelCovMatrix = np.array([ [47.917, 0., -146.636, -141.572, -123.269],
                            [0., 408.250, 68.487, 69.828, 53.479],
                            [-146.636, 68.487, 2654.285, 2621.672, 2440.381],
                            [-141.572, 69.828, 2621.672, 2597.818, 2435.368],
                            [-123.269, 53.479, 2440.381, 2435.368, 2404.923] ])
for i in range(nr_d):
    for j in range(nc_d):
        img = np.zeros( (nr_w,nc_w,5) )
        img[:,:,0] = x
        img[:,:,1] = y
        img[:,:,2:5] = img_t[i:i+nr_w, j:j+nc_w, :]
        img = img.reshape( (nr_w*nc_w, 5) )
        C = np.cov( img.transpose(), bias=True )
        [w,v] = scipy.linalg.eigh(modelCovMatrix, b=C)
        distance[i,j] = np.sqrt( np.sum( np.log(w+1.e-13)**2 ) )

idx_min = np.unravel_index( np.argmin(distance), distance.shape)
print(idx_min)
plt.imsave("best_match.bmp", img_raw[idx_min[0]:idx_min[0]+nr_w,idx_min[1]:idx_min[1]+nc_w,:])
plt.imsave("distance.bmp", distance, cmap="gray")




# Part 2 #
def circularNeighbors(img, x, y, radius):
    nr, nc = img.shape[:2]
    xi = np.zeros((nr,nc))
    yi = np.zeros((nr,nc))
    for i in range(nr):
        yi[i,:] = i
    for j in range(nc):
        xi[:,j] = j
    r = np.sqrt( (xi-x)**2 + (yi-y)**2 )
    num_points = int(np.sum( r < radius ))

    X = np.zeros( (num_points,5) )
    count = 0
    for j in range(nr):
        for i in range(nc):
            if r[j,i] < radius:
                X[count,0] = i
                X[count,1] = j
                X[count,2:5] = img[j,i,:]
                count += 1
    return X



# Part 3 #
def colorHistogram(X, bins, x, y, h):
    hist = np.zeros( (bins,bins,bins) ) 
    b = 256. / bins
    
    r = ( (X[:,0]-x)**2 + (X[:,1]-y)**2 ) / h**2

    for n in range(X.shape[0]):
        i = int(X[n,2] // b)
        j = int(X[n,3] // b)
        k = int(X[n,4] // b)
        if r[n] < 1.:
            hist[i,j,k] += 1. - r[n]

    hist /= np.sum(hist)
    return hist




# Part 4 #
def meanshiftWeights(X, q_model, p_test, bins):
    w = np.zeros( X.shape[0] )
    b = 256. / bins
    
    for n in range(X.shape[0]):
        i = int(X[n,2] // b)
        j = int(X[n,3] // b)
        k = int(X[n,4] // b)
        if p_test[i,j,k] > 0.:
            w[n] += q_model[i,j,k] / p_test[i,j,k];
        
    return w


# Part 5 #
def update(X, w, x,y, h):
    r = ( (X[:,0]-x)**2 + (X[:,1]-y)**2 ) / h**2
    g = r < 1.
    x1 = np.sum( w*g*X[:,0] ) / np.sum( w*g )
    y1 = np.sum( w*g*X[:,1] ) / np.sum( w*g )

    return x1, y1


img1 = plt.imread("img1.jpg").astype(float)
img2 = plt.imread("img2.jpg").astype(float)
nr,nc = img1.shape[0:2]

h = 25.
bins = 16

x0,y0 = 150., 175.
X = circularNeighbors(img1, x0,y0, h)
q_model = colorHistogram(X, bins, x0,y0, h)

x,y = x0,y0
for count in range(25):
    X = circularNeighbors(img2, x,y, h)
    p_test = colorHistogram(X, bins, x,y, h)
    w = meanshiftWeights(X, q_model, p_test, bins)
    x,y = update(X, w, x,y, h)
    print(x,y)

plt.imsave("model.bmp", plt.imread("img1.jpg")[int(y0-h):int(y0+h+1),int(x0-h):int(x0+h+1)])
plt.imsave("match.bmp", plt.imread("img2.jpg")[int(y-h):int(y+h+1)  ,int(x-h):int(x+h+1)  ])
