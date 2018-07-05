# -*- coding: utf-8 -*-
"""
Created on Thu Jun 21 10:30:03 2018

@author: Administrator
"""
#%%
import numpy as np
import cv2
from matplotlib import pyplot as plt
#%%
index = 34
folderOne = "TrainingImages/"
folderTwo = "TargetImages/"
sourceImage = cv2.imread(folderOne+str(index)+".bmp", cv2.IMREAD_GRAYSCALE)
targetImage = cv2.imread(folderTwo+str(index)+".jpg", cv2.IMREAD_GRAYSCALE)
plt.hist(sourceImage.flatten(), bins=256, range=(0, 256))
plt.show()
#%%
roiImage = sourceImage[targetImage==255]
plt.hist(roiImage.flatten(), bins=256, range=(0, 256))
plt.show()
#%%
histSourceImage = cv2.equalizeHist(sourceImage)
plt.hist(histSourceImage.flatten(), bins=256, range=(0, 256))
plt.show()
#%%
imageThresh = 255 - cv2.compare(sourceImage, 55, cv2.CMP_GE)
imageBlend = 255 - cv2.add(imageThresh, targetImage)
plt.imshow(imageBlend, cmap="gray")
plt.show()
cv2.imwrite(str(index)+".tif", imageBlend)
#%%