# -*- coding: utf-8 -*-
"""
Created on Tue Jun 26 06:03:51 2018

@author: Administrator
"""

import numpy as np

def loadSimpData():
    datMat = np.matrix([[1.0, 2.1],
                        [2.0, 1.1],
                        [1.0, 1.0],
                        [2.0, 1.0]])
    classLabel = [1.0, 1.0, -1.0, -1.0, 1.0]
    return datMat, classLabel