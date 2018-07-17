import math

import cv2
import numpy as np 
import matplotlib.pyplot as plt

def homo_filter(image: np.ndarray):
    image = image.astype(np.float32)
    M, N = image.shape
    rL = 0.5
    rH = 4.7
    c = 2
    d0 = 10
    image1 = np.log(image + 1)
    f_image = np.fft.fft2(image1)
    n1 = math.floor(M/2)
    n2 = math.floor(N/2)
    D = np.array([[(i-n1)**2 + (j-n2)**2 for j in range(N)] for i in range(M)])
    H = (rH - rL) * np.exp(-c*D/d0**2) + rL
    image2 = np.fft.ifft2(H * f_image)
    image3 = np.real(np.exp(image2) - 1)
    image4 = (image3 - np.min(image3)) / (np.max(image3 - np.min(image3))) * 255
    homo_image = image4.astype(np.uint8)
    return homo_image


if __name__ == "__main__":
    image = cv2.imread("8.bmp", cv2.IMREAD_GRAYSCALE)
    homo_image = homo_filter(image)
    hist_image = cv2.equalizeHist(image)
  
    plt.figure()
    plt.imshow(hist_image, cmap="gray")
    plt.show()