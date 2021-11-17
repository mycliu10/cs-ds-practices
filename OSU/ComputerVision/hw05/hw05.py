import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
import numpy as np
import scipy.ndimage
import scipy.stats
import skimage.morphology
import skimage.measure



# ----------------------------------------------- #
# Part 1 #
tau_list = [2., 5., 10.]
I_list = [plt.imread( "".join(("aerobic-",n.__str__().zfill(3),".bmp"))).astype(float) for n in range(1,23)]

def cleanDiff(I0,I1, tau):
    D = np.abs( I1-I0 ) > tau
    D = scipy.ndimage.median_filter(D, size=3)
    D = skimage.morphology.remove_small_objects(D, min_size=8)
    return D


for tau in tau_list:
    for n in range(1,len(I_list)):
        I1 = I_list[n]
        I0 = I_list[n-1]
        D = cleanDiff(I0,I1, tau)

        plt.imsave("".join( ("aerobic-",(n+1).__str__().zfill(3),"_tau", tau.__str__(),".bmp") ), D, cmap="gray")
 


# Part 2 #
tau = 5.
D = []
for n in range(1,len(I_list)):
   I1 = I_list[n]
   I0 = I_list[n-1]
   D.append( cleanDiff(I0,I1,tau)  )


def MEI(D):
    E = D[0]
    for d in D[1:]:
        E = np.logical_or(E, d)

    return E 

def MHI(D):
    M = np.zeros_like(D[-1].astype(float))
    def F(m,n, tau):
        if tau==0:
            return 0

        if D[int(tau-1)][m,n].astype(float) == 1.:
            return tau
        else:
            return np.maximum( 0., F(m,n,tau-1)-1)


    for m in range(M.shape[0]):
        for n in range(M.shape[1]):
            M[m,n] = F(m,n, len(D))
     
    return M
    
    

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


E = MEI(D) 
plt.imsave( "MEI.bmp", E, cmap="gray" )
M = MHI(D) 
plt.imsave( "MHI.bmp", M, cmap="gray" )

print( similitudeMoments(E) )
print( similitudeMoments(M) )



# Part 3 #
I0 = np.zeros((101,101))
I0[40:61, 6:27] = 255.
I1 = np.zeros_like(I0)
I1[41:62, 7:28] = 255.

Gx = np.zeros((3,3))
Gx[0,0] = -1.
Gx[1,0] = -2.
Gx[2,0] = -1.
Gx[0,2] = 1.
Gx[1,2] = 2.
Gx[2,2] = 1.
Gx /= 8.
Gy = np.zeros((3,3))
Gy[0,0] = -1.
Gy[0,1] = -2.
Gy[0,2] = -1.
Gy[2,0] = 1.
Gy[2,1] = 2.
Gy[2,2] = 1.
Gy /= 8.


fx = scipy.ndimage.convolve(I1, -Gx, mode="nearest")
fy = scipy.ndimage.convolve(I1, -Gy, mode="nearest")
ft = I1 - I0

mag = np.sqrt( fx**2+fy**2 )
w1 =  fx / mag
w2 =  fy / mag
w3 = -ft / mag

plt.figure()
plt.imshow(I1, cmap="gray")
plt.quiver(w1*w3/abs(w3), -w2*w3/abs(w3), units="xy", color="r")
plt.show()



