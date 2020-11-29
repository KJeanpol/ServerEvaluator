from PIL import Image
import numpy as np
from numpy import *
import matplotlib.pyplot as plt
import os
import math
import sys

def sobel2D(img):
    """
    Function convolves an image with 2-D version of Sobel kernel. Pixels at borders set to black

    Input: 2-D array of an image with 1 channel
    Output 2-D convolved array
    """
    # Sobel kernel for horizontal gradient
    sobel2Dx = np.array([
        [1, 0, -1],
        [2, 0, -2],
        [1, 0, -1]
    ], dtype=np.int32)

    # Sobel kernel for vertical gradient
    sobel2Dy = sobel2Dx.T

    # Flip sobel kernel along x and y axes for convolution
    sobel2Dx = np.flip(sobel2Dx, axis=[0, 1])

    # Flip sobel kernel along x and y axes for convolution
    sobel2Dy = np.flip(sobel2Dy, axis=[0, 1])

    # Arrays to store convolved image
    nx = np.ndarray(shape=img.shape, dtype=np.int32)
    ny = np.ndarray(shape=img.shape, dtype=np.int32)

    for r in range(img.shape[0]):
        ith = r - 1
        for c in range(img.shape[1]):
            jth = c - 1
            # If kernel exceeds image size, make pixel 0 at that location
            if ith < 0 or jth < 0:
                nx[r, c] = 0
                ny[r, c] = 0
            elif ith + 3 > img.shape[0] or jth + 3 > img.shape[1]:
                nx[r, c] = 0
                ny[r, c] = 0
            else:
                # Convolution operation
                nx[r, c] = round((img[ith:ith + 3, jth:jth + 3] * sobel2Dx).sum(), 0)
                ny[r, c] = round((img[ith:ith + 3, jth:jth + 3] * sobel2Dy).sum(), 0)

    return (nx, ny)


def main(argv):
    # Image path + name
    pth1 = argv[0]
    path, dirs, files = next(os.walk("Images"+str(argv[1])))
    file_count = len(files)

    # import image
    inputImg = inputImg1 = Image.open(pth1)
    img = np.array(inputImg1.convert("L"), dtype=np.uint8) #Convert image to grayscale and numpy array

    # Convolve image with 2D kernel
    gx2D, gy2D = sobel2D(img)

    #Calculate gradient magnitudes
    gx2Dmag = np.sqrt(gx2D * gx2D)

    #Visualize results
    if (file_count<100):
        plt.imsave("Images"+str(argv[1])+"/"+str(file_count+1)+".png", gx2Dmag, cmap="gray")



if __name__ == "__main__":
   main(sys.argv[1:])