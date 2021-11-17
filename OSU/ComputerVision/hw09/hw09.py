import numpy as np
import matplotlib.pyplot as plt



# Part 1 #
p2d = np.loadtxt("2Dpoints.txt")
p3d = np.loadtxt("3Dpoints.txt")

N = p2d.shape[0] 
A = np.zeros( (N*2, 12) )
for i in range(N):
    A[2*i+0, 0:3] = p3d[i,:]
    A[2*i+0, 3] = 1.
    A[2*i+0, 8:11] = -p3d[i,:] * p2d[i,0]
    A[2*i+0, 11] = - p2d[i,0]
    A[2*i+1, 4:7] = p3d[i,:]
    A[2*i+1, 7] = 1.
    A[2*i+1, 8:11] = -p3d[i,:] * p2d[i,1]
    A[2*i+1, 11] = - p2d[i,1]
#    print(A[2*i+0,:])
#    print(A[2*i+1,:])


B = np.matmul(A.transpose(), A)

w,v = np.linalg.eig(B)

idx_min = np.argmin(w)
p = v[:,idx_min]
p /= np.linalg.norm(p)
P = p.reshape((3,4))
print(P)


# Part 2 #
p2d_est = np.zeros( (3,N) )
for i in range(N):
    x3d = np.ones( (4) )
    x3d[:3] = p3d[i,:]
    p2d_est[:,i] = np.matmul(P, x3d)
    p2d_est[0:2,i] /= p2d_est[2,i]
#    print(p2d_est[0:2,i])
#    print(p2d[i,0:2])

error = np.linalg.norm( p2d_est[:2,:].transpose() - p2d[:N,:] )
print(error)




# Part 3 #
def makeNDLT(p):
    x = p[:,0]
    y = p[:,1]
    xmean = np.mean(x)
    ymean = np.mean(y)
    s = np.sqrt(2) * x.shape[0] / np.sum( np.sqrt((x-xmean)**2+(y-ymean)**2) )
    T = np.zeros((3,3))
    T[0,0] =  s
    T[0,2] = -s * xmean
    T[1,1] = s
    T[1,2] = -s * ymean
    T[2,2] = 1.

    P = np.ones( (3,p.shape[0]) )
    P[:2,:] = p.transpose()
    P = np.matmul(T, P)
    return T, P[:2,:].transpose()

points = np.loadtxt("homography.txt", delimiter=",")
image1 = points[:,:2]
image2 = points[:,2:4]
T1, image1_T = makeNDLT(image1)
T2, image2_T = makeNDLT(image2)

N = image1.shape[0] 
A = np.zeros( (N*2, 9) )
for i in range(N):
    A[2*i+0, 0:2] = image1_T[i,:]
    A[2*i+0, 2] = 1.
    A[2*i+0, 6:8] = -image1_T[i,:] * image2_T[i,0]
    A[2*i+0, 8] = - image2_T[i,0]
    A[2*i+1, 3:5] = image1_T[i,:]
    A[2*i+1, 5] = 1.
    A[2*i+1, 6:8] = -image1_T[i,:] * image2_T[i,1]
    A[2*i+1, 8] = - image2_T[i,1]

#    print(A[2*i+0,:])
#    print(A[2*i+1,:])


B = np.matmul(A.transpose(), A)

w,v = np.linalg.eig(B)

idx_min = np.argmin(w)
h = v[:,idx_min]
h /= np.linalg.norm(h)
H = h.reshape((3,3))
H = np.matmul(np.linalg.inv(T2), H)
H = np.matmul(H, T1)
print(H)

image2_est = np.zeros( (3,N) )
for i in range(N):
    x = np.ones( (3) )
    x[:2] = image1[i,:]
    image2_est[:,i] = np.matmul(H, x)
    image2_est[0:2,i] /= image2_est[2,i]
#    print(image2_est[0:2,i])
#    print(image2[i,0:2])

plt.figure()
plt.plot(image2[:,0], image2[:,1], "r.")
plt.plot(image2_est[0,:], image2_est[1,:], "b+")
plt.legend(["real image2", "projected image2"])
plt.savefig("NDLTH.png")
error = np.linalg.norm( image2_est[:2,:].transpose() - image2[:N,:] )
print(error)


