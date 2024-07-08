"""
    Author:  Jason Hughes
    Date:    July 2024
    About:   Test script for wrapper around models
    package: Waz
"""

import cv2
from depth_wrapper import DepthWrapper
from segmentation_wrapper import SegmentorWrapper

if __name__ == "__main__":

    dw = DepthWrapper("/home/jason/config/depth_anything.json")

    img = cv2.imread("/home/jason/test/test-img.png")
    
    output = dw.inference(img)
