import numpy as np
import cv2
import matplotlib.pyplot as plt
import skimage.feature
import skimage.color
import skimage.segmentation 
import skimage.morphology
import os
from numba import jit

def main():
    # read patterns (templates)
    eye_patterns = [ preprocess( "".join( ("pattern/eye", n.__str__(), ".jpg") ) ) for n in range(2) ]
    eye_grays = [e[0] for e in eye_patterns]
    eye_edges = [e[1] for e in eye_patterns]

    # image
    for filename in ["".join( ("./data/mask",n.__str__(),".jpg") ) for n in range(27)]:
        processData(filename, eye_edges)

    return 


def preprocess(filename):
    # filename preprocess #
    prefix = "test_" + os.path.split(filename)[-1]

    # read #
    image = cv2.imread(filename)

    # color space #
    image_gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    # output gray #
    plt.figure()
    plt.imshow(image_gray, cmap="gray")
    plt.savefig("".join( (prefix, "_gray.jpg")))
    plt.close()


    # Canny edge #
    edges = genericCanny(image_gray)
    # edge close #
    kernel = np.ones((7,7), np.uint8)
    edges = skimage.morphology.binary_closing(edges, selem=kernel)
    # output edge #
    plt.figure()
    plt.imshow(edges, cmap="gray")
    plt.savefig("".join( (prefix, "_canny.jpg")))
    plt.close()

    return image_gray, edges



def processData(filename, eye_patterns):
    # filename preprocess #
    prefix = "test_" + os.path.split(filename)[-1]

    # read #
    image = cv2.imread(filename)


    # color space #
    image_gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    image_hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)

    image_gray, _ = resizeImage(image_gray, 530)
    image_hsv , _ = resizeImage(image_hsv,  530)
    plt.figure()
    plt.imshow(image_gray, cmap="gray")
    plt.savefig("".join( (prefix, "_resized.jpg")))
    plt.close()


    # HSV thresholding #
    lower = np.array([ 0,  71,   0], dtype=np.uint8)
    upper = np.array([25, 173, 255], dtype=np.uint8)
    image_hsv = cv2.inRange(image_hsv, lower, upper)
    # HSV close and open #
    kernel = np.ones((3,3), np.uint8)
    image_hsv = cv2.morphologyEx(image_hsv, cv2.MORPH_CLOSE, kernel)
    image_hsv = cv2.morphologyEx(image_hsv, cv2.MORPH_OPEN, kernel)
    # output HSV #
    plt.figure()
    plt.imshow(image_hsv, cmap="gray")
    plt.savefig("".join( (prefix, "_hsv.jpg")))
    plt.close()

    # Canny edge #
    edges = genericCanny(image_gray)
    # edge close #
    kernel = np.ones((7,7), np.uint8)
    edges = skimage.morphology.binary_closing(edges, selem=kernel)
    # output edge #
    plt.figure()
    plt.imshow(edges, cmap="gray")
    plt.savefig("".join( (prefix, "_canny.jpg")))
    plt.close()

    # face extract #
    nst0, nen0, nst1, nen1 = cropImage(edges)
    face_edges = edges[nst0:nen0,nst1:nen1]
    face_gray = image_gray[nst0:nen0,nst1:nen1]
    # output face edge #
    plt.figure()
    plt.imshow(face_edges, cmap="gray")
    plt.savefig("".join( (prefix, "_face.jpg")))
    plt.close()

    # template matching #
    tm_eye_result = multiTemplateMatch(eye_patterns, face_edges)
    plt.figure()
    plt.imshow(face_edges, cmap="gray")
    plt.plot(tm_eye_result[2], tm_eye_result[1], "ro", markersize=12)
    plt.savefig("".join( (prefix, "_eye.jpg") ))
    plt.close()


    # examine mask location #
    distance = 100.
    count = 0
    j = nst1 + tm_eye_result[2]
    for i in range(nst0+tm_eye_result[1]+tm_eye_result[4], image_hsv.shape[0]):
        if image_hsv[i,j]==0:
            count += 1
            if count==10:
                distance = (i - (nst0+tm_eye_result[1]) - 10) / tm_eye_result[4]
                break
        else:
            count = 0
    
    result = distance < 1.1
    print(filename, ": mask-eye distance = ", distance, " of eye widths.", "Safty: ", result)
    plt.figure()
    plt.imshow(image_hsv, cmap="gray")
    plt.plot(j, nst0+tm_eye_result[1], "ro", markersize=12)
    plt.plot(j, i-10, "yo", markersize=12)
    plt.savefig(prefix + "_mask.jpg")
    plt.close()



    # output result #
    plt.figure()
    plt.imshow(image_gray, cmap="gray")
    plt.plot(j, nst0+tm_eye_result[1], "ro", markersize=12)
    plt.plot(j, i-10, "yo", markersize=12)
    plt.savefig(prefix)
    plt.close()






def cropImage(image):
    i0 = np.mean(image, axis=1)
    i1 = np.mean(image, axis=0)
    def crop1D(image):
        imax = np.amax(image)
        for n in range(image.shape[0]):
            if image[n] > imax/3.:
                nst = n
                break
        for n in range(image.shape[0]):
            if image[image.shape[0]-1-n] > imax/3.:
                nen = image.shape[0]-1-n
                break
        return nst, nen

    nst0, nen0 = crop1D(i0)
    nst1, nen1 = crop1D(i1)

    return nst0, nen0, nst1, nen1


def resizeImage(image, w):
    if w==image.shape[1]:
        return image, 1.

    h = int( image.shape[0]/image.shape[1] * w )
    ratio = image.shape[1] / w

    if image.ndim==2:
        image1 = np.zeros( (h,image.shape[1]) )
        xp = np.linspace(0., 1., image.shape[0])
        x  = np.linspace(0., 1., h)
        for c in range(image1.shape[1]):
            image1[:,c] = np.interp( x, xp, image[:,c] )
        image2 = np.zeros( (h,w) )
        xp = np.linspace(0., 1., image1.shape[1])
        x  = np.linspace(0., 1., w)
        for r in range(h):
            image2[r,:] = np.interp( x, xp, image1[r,:] )
    else:
        image1 = np.zeros( (h,image.shape[1],3) )
        xp = np.linspace(0., 1., image.shape[0])
        x  = np.linspace(0., 1., h)
        for n in range(3):
            for c in range(image1.shape[1]):
                image1[:,c,n] = np.interp( x, xp, image[:,c,n] )
        image2 = np.zeros( (h,w,3) )
        xp = np.linspace(0., 1., image.shape[1])
        x  = np.linspace(0., 1., w)
        for n in range(3):
            for r in range(h):
                image2[r,:,n] = np.interp( x, xp, image1[r,:,n] )
 
    image_sized = image2 

    return image_sized, ratio


def mirroImage(image):
    image_mirrored = image[:,::-1,:]
    return image_mirrored


def multiTemplateMatch(tlist, image, debug=False):
    found = None
    image_sized, ratio = resizeImage(image, 500)
    for scale in np.linspace(0.1, 2.0, 100):
        w = int( scale*image_sized.shape[1] )
        resized_image, _ = resizeImage(image_sized, w)

        count = 0
        for t in tlist:
            r = templateMatch(t, resized_image)

            if debug and r is not None:
                plt.figure()
                plt.imshow(r[3], cmap="gray")
                plt.savefig( "".join( ("test_s", scale.__str__(), "_t", count.__str__(), ".jpg") ) )
                plt.close()

            if r is not None and (found is None or r[0] > found[0]):
                found = (r[0], int( (r[1]+t.shape[0]/2)/scale * ratio ), int( (r[2]+t.shape[1]/2)/scale * ratio ), count, int(t.shape[1]/scale * ratio) )

            count += 1

    return found


def templateMatch(t, image):
    found = None
    w = image.shape[1]
    h = image.shape[0]

    if w < t.shape[1] or h < t.shape[0]:
        return found

    r = genericMatchTemplate(image, t)

    idxmax = np.argmax( r )
    idxmax = np.unravel_index(idxmax, r.shape)
    maxloc = idxmax
    maxval = r[idxmax]

    found = (maxval, maxloc[0], maxloc[1], r)

    return found


def genericMatchTemplate(image, t):
    nr_t, nc_t = t.shape[:2]
    nr_t, nc_t = t.shape[:2]
    nr_s, nc_s = image.shape[:2]
    scores = np.zeros( (nr_s-nr_t+1, nc_s-nc_t+1) )

    t_raveled = t.ravel() / 255.
    tmean = np.mean(t_raveled)
    tstd = np.std( t_raveled, ddof=1 )
    
    for r in range(scores.shape[0]):
        for c in range(scores.shape[1]):
            i_raveled = image[r:r+nr_t,c:c+nc_t].ravel() / 255.
            imean = np.mean(i_raveled)
            istd = np.std( i_raveled, ddof=1 )
            if istd==0.:
                scores[r,c] = 0.
            else:
                scores[r,c] = np.sum( (i_raveled-imean)*(t_raveled-tmean) ) / istd / tstd / (i_raveled.shape[0]-1)

    return scores
    






def gaussianKernel(sigma):
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



def convolveKernel(image, kernel):
    nk = (kernel.shape[0] - 1) // 2
    nr, nc = image.shape

    image_work = np.zeros_like(kernel)
    result = np.zeros_like(image)

    for r in range(nr):
        for c in range(nc):
            for i in range(2*nk+1):
                for j in range(2*nk+1):
                    iwork = r + i - nk 
                    if iwork < 0:
                        iwork = 0
                    if iwork >= nr:
                        iwork = nr - 1
                    jwork = c + j - nk 
                    if jwork < 0:
                        jwork = 0
                    if jwork >= nc:
                        jwork = nc - 1

                    image_work[i,j] = image[iwork,jwork]
            result[r,c] = np.sum( image_work*kernel )

    return result


def sobelFilter(image):
    sx = np.array([[-1, 0, 1], [-2, 0, 2], [-1,  0,  1]])
    sy = np.array([[ 1, 2, 1], [ 0, 0, 0], [-1, -2, -1]])
    sx = sx / 8.
    sy = sy / 8.

    Ix = convolveKernel(image, sx)
    Iy = convolveKernel(image, sy)

    G = np.sqrt( Ix*Ix + Iy*Iy )
    G = G / np.max(G) * 255.
    theta = np.arctan2(Iy, Ix)

    return G, theta


def NMS(image, theta):
    nr,nc = image.shape
    result = np.zeros_like(image)
    angle = theta / np.pi * 180.
    angle[angle < 0] += 180.

    for r in range(1, nr-1):
        for c in range(1, nc-1):
            a = angle[r,c]
            if (0. <= a and a < 22.5) or (157.5 <= a and a <= 180.):
                q = image[r,c+1]
                p = image[r,c-1]
            elif 22.5 <= a and a < 67.5:
                q = image[r+1,c-1]
                p = image[r-1,c+1]
            elif 67.5 <= a and a < 112.5:
                q = image[r+1,c]
                p = image[r-1,c]
            elif 112.5 <= a and a < 157.5:
                q = image[r-1,c-1]
                p = image[r+1,c+1]

            if (image[r,c] >= q) and (image[r,c] >= p):
                result[r,c] = image[r,c]
    return result


def threshold(image, low, high):
    result = np.zeros_like(image)
    
    idx_high = np.where( image >= high )
    idx_mid  = np.where( np.logical_and(low <= image, image < high) )

    result[idx_high] = 255.
    result[idx_mid] = 25.

    return result

   
def hysteresis(image):
    nr,nc = image.shape
    for r in range(1, nr-1):
        for c in range(1, nc-1):
            if image[r,c]==25.:
                if np.sum( image[r-1:r+2,c-1:c+2]==255. ) >= 1.:
                    image[r,c] = 255.
                else:
                    image[r,c] = 0.

    return image 


def genericCanny(image, low=0.2, high=0.4, sigma=1.):
    low  =  low * np.max(image)
    high = high * np.max(image)

    g = gaussianKernel(sigma)
    G = convolveKernel(image, g)

    G, theta = sobelFilter(G)


    G = NMS(G, theta)

    G = threshold(G, low, high)

    G = hysteresis(G)

    return G.astype(np.uint8)


if __name__=="__main__":
    main()
