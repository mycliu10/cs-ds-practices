import numpy as np
import matplotlib.pyplot as plt
from skimage.segmentation import slic



# Part 1 #
img = plt.imread("doggy.jpg").astype(float)
numsegs_list = [2, 10 ]
compact_list = [0.1, 1., 10., 100.]

for n in numsegs_list:
    for c in compact_list:
        segments_lic = slic(img, n_segments=n, compactness=c, start_label=1)
        print( "num_segs = ", n, "    compactness = ", c , " num of segs = ", np.max(segments_lic))
        for i in range(1, np.max(segments_lic)+1):
            filename = "".join( ("doggy_n",n.__str__(),"_c",c.__str__(),"_",i.__str__(),".bmp") )
            img_to_save = img.copy()
            img_to_save[segments_lic!=i] = 0.
            plt.imsave( filename,  img_to_save/255.)



# Part 2 #
img_s = plt.imread("search.png").astype(float)
img_t = plt.imread("template.png").astype(float)

nr_t, nc_t = img_t.shape[:2]
nr_s, nc_s = img_s.shape[:2]
nr_t_half = (nr_t-1) // 2
nc_t_half = (nc_t-1) // 2
scores = np.zeros( (nr_s-2*nr_t_half, nc_s-2*nc_t_half) )

ustd3 = np.std(img_t, axis=(0,1), ddof=1)
for c in range(scores.shape[1]):
    for r in range(scores.shape[0]):
        img = img_s[r:r+nr_t,c:c+nc_t,:]
        sig = np.std(img, axis=(0,1), ddof=1)
        for n in range(3):
            P = img[:,:,n]
            T = img_t[:,:,n]
            scores[r,c] += np.sum( (P-np.mean(P))*(T-np.mean(T)) ) / ustd3[n] / sig[n] / (P.size-1)

idx_sorted = np.argsort(scores, axis=None)
scores_sorted = scores.ravel()[idx_sorted]

plt.figure()
plt.plot(np.arange(scores_sorted.size)+1, scores_sorted[::-1])
plt.savefig("scores_vs_k.png")

for k in [1, 2, 5, 10, 100, 500]:
    idx = np.unravel_index(idx_sorted[-k], scores.shape)
    plt.imsave( "".join( (k.__str__(),"th.png") ), img_s[idx[0]:idx[0]+nr_t, idx[1]:idx[1]+nc_t,:] )


