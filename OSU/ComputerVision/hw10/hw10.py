import numpy as np
import matplotlib.pyplot as plt
import scipy.signal
from sklearn.neighbors import KNeighborsClassifier

# Part 1 #
def NCC(a, b):
    fa = a - np.mean(a)
    fb = b - np.mean(b)
    f = np.correlate(fa, fb)
    f /= f.shape[0]
    
    sa = np.std(fa)
    sb = np.std(fb)
    if sa==0. or sb==0.:
        f = 0.
    else:
        f /= sa*sb

    return f


def BasicStereo(il, ir, window_size=11):
    D = np.zeros_like(il)
    halfws = (window_size-1) // 2
    for c in range(halfws, il.shape[1]-halfws):
        for r in range(halfws, il.shape[0]-halfws):
            p = il[r-halfws:r+halfws+1,c-halfws:c+halfws+1]
            count = 0
            cmax = -1
            ccmax = -1.
            for n in range(c-1, halfws-1, -1):
                s = ir[r-halfws:r+halfws+1,n-halfws:n+halfws+1]
                f = NCC(p.ravel(), s.ravel())
                if f > ccmax:
                    cmax = n
                    ccmax = f
                count += 1
                if count==50:
                    break
            D[r,c] = c - cmax
    
    return D




il = plt.imread("left.png").astype(float) * 255.
ir = plt.imread("right.png").astype(float) * 255.

D = BasicStereo(il, ir)
plt.imsave("disparity.bmp", D, cmap="gray")



# Part 2 #
train_data = np.loadtxt("train.txt")
test_data = np.loadtxt("test.txt")
for K in [1, 5, 11, 15]:
    knnc = KNeighborsClassifier(n_neighbors=K)
    knnc.fit(train_data[:,:-1], train_data[:,-1])
    y = knnc.predict(test_data[:,:-1])
    accuracy = knnc.score(test_data[:,:-1], test_data[:,-1])
    print("Accuracy = ", accuracy)
    plt.figure()
    idx = y==1.
    plt.plot(test_data[idx, 0], test_data[idx,1], "r.")
    idx = y==2.
    plt.plot(test_data[idx, 0], test_data[idx,1], "b.")
    idx = y!=test_data[:,-1]
    plt.plot(test_data[idx, 0], test_data[idx,1], "ko")
    plt.savefig("KNN_K"+K.__str__()+".png")





