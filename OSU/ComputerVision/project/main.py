import numpy as np
import cv2
import matplotlib.pyplot as plt
import skimage.feature
import skimage.color
import skimage.segmentation 
import os

def main():
    # read patterns (templates)
    eye_patterns = [ preprocess( "".join( ("pattern/eye", n.__str__(), ".jpg") ) ) for n in range(2) ]
    eye_grays = [e[0] for e in eye_patterns]
    eye_edges = [e[1] for e in eye_patterns]

#    segs = skimage.segmentation.slic(image, n_segments=10, compactness=0.1, start_label=1)
#    plt.imshow(skimage.color.label2rgb(segs, image))
#    plt.show()


    # image
    for filename in ["".join( ("./data/mask",n.__str__(),".jpg") ) for n in range(27)]:
        processData(filename, eye_edges)

    return 


#    edges = skimage.feature.canny(image_gray, sigma=sigma)
#    plt.imshow(edges, cmap="gray")
#    plt.show()

#    sift = cv2.SIFT_create()
#    kp, des = sift.detectAndCompute(edges, None)
#
#
#    eye_sift = [sift.detectAndCompute(img, None) for img in eye_edges]
#
#    i = cv2.drawKeypoints(image_gray, kp, image)
##    plt.imshow(i)
##    plt.show()
#
#    bf = cv2.BFMatcher(cv2.NORM_L1, crossCheck=True)
#    eye_matches = bf.match(des, eye_sift[0][1])
#    eye_matches = sorted(eye_matches, key = lambda x:x.distance)
#
#    img = cv2.drawMatches(edges, kp, eye_edges[0], eye_sift[0][0], eye_matches[:10], None, flags=cv2.DrawMatchesFlags_NOT_DRAW_SINGLE_POINTS)

#    plt.imshow(img)
#    plt.show()

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
    edges = cv2.Canny(image_gray, 100., 200., apertureSize=3)
    # edge close #
    kernel = np.ones((7,7), np.uint8)
    edges = cv2.morphologyEx(edges, cv2.MORPH_CLOSE, kernel)
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
    image, _ = resizeImage(image, 530)


    # color space #
    image_gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    image_hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)

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
    edges = cv2.Canny(image_gray, 100., 200., apertureSize=3)
    # edge close #
    kernel = np.ones((7,7), np.uint8)
    edges = cv2.morphologyEx(edges, cv2.MORPH_CLOSE, kernel)
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
    plt.plot(tm_eye_result[2], tm_eye_result[1], "ro")
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
    plt.plot(j, nst0+tm_eye_result[1], "ro")
    plt.plot(j, i-10, "yo")
    plt.savefig(prefix + "_mask.jpg")
    plt.close()



    # output result #
    plt.figure()
    plt.imshow(image_gray, cmap="gray")
    plt.plot(j, nst0+tm_eye_result[1], "ro")
    plt.plot(j, i-10, "yo")
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
    dims = (w, h)
    image_sized = cv2.resize(image, dims, interpolation = cv2.INTER_AREA)

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

    r = cv2.matchTemplate(image, t, cv2.TM_CCORR_NORMED)
#    r = cv2.matchTemplate(i, resized_t, cv2.TM_SQDIFF_NORMED)
#    r = cv2.matchTemplate(i, resized_t, cv2.TM_CCOEFF_NORMED)
    minval, maxval, minloc, maxloc = cv2.minMaxLoc(r)

    found = (maxval, maxloc[1], maxloc[0], r)

    return found























if __name__=="__main__":
#    image = cv2.imread("pattern/eye0.jpg")
#    image = mirroImage(image)
#    image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
#    plt.imsave("pattern/eye1.jpg", image)
    main()
