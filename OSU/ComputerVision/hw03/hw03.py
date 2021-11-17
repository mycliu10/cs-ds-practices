import matplotlib.pyplot as plt
import numpy as np
import scipy.ndimage
import skimage.color
import skimage.feature
import skimage.morphology
import skimage.measure

# ----------------------------------------------- #


# Part 1 #
img = plt.imread("doggy.jpg")
img = skimage.color.rgb2gray(img)
img = img[:641,:481]

plt.imsave("doggy_lvl0.png", img, cmap="gray")


def usePyramid(i0, a=0.4):
    nr0,nc0 = i0.shape
    i0_cpy0 = i0.copy()
    i0_cpy1 = i0.copy()

    nr1 = (nr0-1) // 2 + 1
    nc1 = (nc0-1) // 2 + 1

    i1 = np.zeros((nr1,nc1))
    g = np.array( [.25-.5*a, .25, a, .25, .25-.5*a] )
    h = np.zeros_like(g)

    for i in range(nr0):
        for j in range(nc0):
            if i==0:
                h[0] = i0[0,j]
                h[1] = i0[0,j]
                h[2:5] = i0[0:3,j]
            elif i==1:
                h[0] = i0[0,j]
                h[1:5] = i0[0:4,j]
            elif i==nr0-1:
                h[4] = i0[nr0-1,j]
                h[3] = i0[nr0-1,j]
                h[0:3] = i0[nr0-3:nr0,j]
            elif i==nr0-2:
                h[4] = i0[nr0-1,j]
                h[0:4] = i0[nr0-4:nr0,j]
            else:
                h = i0[i-2:i+3,j]

            i0_cpy0[i,j] = np.dot( g, h )
    
    for i in range(nr0):
        for j in range(nc0):
            if j==0:
                h[0] = i0_cpy0[i,0]
                h[1] = i0_cpy0[i,0]
                h[2:5] = i0_cpy0[i,0:3]
            elif j==1:
                h[0] = i0_cpy0[i,0]
                h[1:5] = i0_cpy0[i,0:4]
            elif j==nc0-1:
                h[4] = i0_cpy0[i,nc0-1]
                h[3] = i0_cpy0[i,nc0-1]
                h[0:3] = i0_cpy0[i,nc0-3:nc0]
            elif j==nc0-2:
                h[4] = i0_cpy0[i,nc0-1]
                h[0:4] = i0_cpy0[i,nc0-4:nc0]
            else:
                h = i0_cpy0[i,j-2:j+3]

            i0_cpy1[i,j] = np.dot( g, h )

    i1 = i0_cpy1[::2,::2]

    i0_est = np.zeros_like(i0)
    i0_est[::2,::2] = i1
    for i in range(nr0):
        for j in range(nc0):
            if i%2==0 and j%2==1:
                i0_est[i,j] = (i0_est[i,j-1] + i0_est[i,j+1]) / 2.
    for i in range(nr0):
        for j in range(nc0-1):
            if i%2==1:
                i0_est[i,j] = (i0_est[i-1,j] + i0_est[i+1,j]) / 2.




    i1_lap = i0 - i0_est

    return i1, i1_lap


i1, i1lag = usePyramid(img)
i2, i2lag = usePyramid(i1)
i3, i3lag = usePyramid(i2)

plt.imsave("doggy_lvl1.png", i1, cmap="gray")
plt.imsave("doggy_lvl2.png", i2, cmap="gray")
plt.imsave("doggy_lvl3.png", i3, cmap="gray")
plt.imsave("doggy_lvl1lag.png", i1lag, cmap="gray")
plt.imsave("doggy_lvl2lag.png", i2lag, cmap="gray")
plt.imsave("doggy_lvl3lag.png", i3lag, cmap="gray")
    


# Part 2 #
img_bg = plt.imread("bg000.bmp").astype(float)
img = plt.imread("walk.bmp").astype(float)

for T in [1,2,5,10,20,50,100]:
    B = np.abs( img - img_bg ) > T
    img_obj =  B#img - img*B
    plt.imsave( "".join(("walk_obj_T", T.__str__(),".bmp")), img_obj, cmap="gray")



# Part 3 #
img_list = []
for n in range(30):
    filename = "".join( ("bg", n.__str__().zfill(3), ".bmp") )
    img = plt.imread(filename).astype(float)
    img_list.append( img )

mu = np.mean( img_list, axis=0 )
sigma = np.std( img_list, axis=0 )

img = plt.imread("walk.bmp").astype(float)

for T in [1,2,5,10,20,50,100]:
    sigma[sigma==0.] = 1.
    B = ( img - mu)**2 / sigma**2 > T**2
    img_obj =  B#img - img*B
    plt.imsave( "".join(("walk_obj_2_T", T.__str__(),".bmp")), img_obj, cmap="gray")

    if T==20:
        B_best = B.copy()



# Part 4 # 
B_dilated = skimage.morphology.binary_dilation(B_best)
plt.imsave("walk_obj_dilated.bmp", B_dilated, cmap="gray")



# Part 5 #
B_labelled = skimage.measure.label(B_dilated)
B_labelled = B_labelled == 0
plt.imsave("walk_obj_label.bmp", B_labelled, cmap="gray")

