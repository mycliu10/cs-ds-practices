import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
import numpy as np
import scipy.ndimage
import scipy.stats


# ----------------------------------------------- #


# Part 1 #
def similitudeMoments(img):
    img = img.astype(float)
    img /= np.max(img)

    mag = np.sum(img)

    x = np.zeros_like(img)
    y = np.zeros_like(img)

    for m in range(x.shape[0]):
        y[m,:] = float(m)
    for n in range(x.shape[1]):
        x[:,n] = float(n)

    xbar =  np.sum(x*img) / mag
    ybar =  np.sum(y*img) / mag

    def eta(img,i,j):
        e = np.sum( (x-xbar)**float(i) * (y-ybar)**float(j) * img ) / mag**(float(i+j)/2.+1.)
        return e


    return [eta(img,0,2), eta(img,0,3), eta(img,1,1), eta(img,1,2), eta(img,2,0), eta(img,2,1), eta(img,3,0)]


for n in range(1,5):
    filename = "".join( ("boxIm",n.__str__()) )
    I = plt.imread(filename+".bmp")
    print( similitudeMoments(I) )




# Part 2 #
X = np.loadtxt("eigdata.txt")
plt.figure()
plt.plot(X[:,0], X[:,1], "b.")
plt.gca().set_aspect('equal')
plt.savefig("eigdata.png")

m = np.mean(X, axis=0)
Y = X.copy()
Y[:,0] -= m[0]
Y[:,1] -= m[1]
plt.figure()
plt.plot(Y[:,0], Y[:,1], "r.")
plt.gca().set_aspect('equal')
plt.savefig("eigdata_meansub.png")




# Part 3 #
K = np.cov(Y.transpose())
[w,v] = np.linalg.eig(K)

t0 = np.arctan2(v[1,0], v[0,0])

def plotEllipse(w,t0,C, f):
    a = np.sqrt(w[0] * C)
    b = np.sqrt(w[1] * C)

    env = []
    for t in np.linspace( 0., 2.*np.pi, 128 ):
        x1 = a * np.cos(t)
        y1 = b * np.sin(t)
        x = x1 * np.cos(t0) - y1 * np.sin(t0)
        y = x1 * np.sin(t0) + y1 * np.cos(t0)
        env.append( [x,y] )

    env = np.array(env)
    f.plot( env[:,0], env[:,1], "k" )


plotEllipse(w, t0, 3.**2, plt)
plt.savefig("eigdata_ellipse.png")




# Part 4 #
R = Y.copy()
for i in range(R.shape[0]):
    x1 = R[i,0]
    y1 = R[i,1]
    x = x1 * np.cos(t0) - y1 * np.sin(t0)
    y = x1 * np.sin(t0) + y1 * np.cos(t0)
    R[i,0] = x
    R[i,1] = y
 
plt.figure()
plt.plot(R[:,0], Y[:,1], ".r")
plt.gca().set_aspect('equal')
plt.savefig("eigdata_rotated.png")





# Part 5 #
(mu,sigma) = scipy.stats.norm.fit(R[:,1])
plt.figure()
n,bins,patches = plt.hist(R[:,1], density=True)
x = np.linspace( bins[0], bins[-1], 128 )
g = scipy.stats.norm(mu, sigma).pdf(x)
plt.plot(x, g, "-k")

plt.savefig("eigdata_hist.png")

   
