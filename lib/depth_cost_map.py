"""
    Author: Jason Hughes
    Date: June 2024
    About: Depth Cost map object
"""

import PIL
import numpy as np
from PIL import Image

class DepthCostMap:

    def __init__(self, depth_img: PIL.Image, map_size=(12,12)):
        
        self.img_ = np.array(depth_img)
        self.cost_map_ = np.zeros(map_size)
        self.map_size_ = map_size

    def get_padding_dim(self, dim: int, block: int) -> int:
        """
        Caclualte the next common multiple for padding
        """
        return dim % block

    def pad(self, pad_x: int, pad_y: int) -> np.ndarray:
        """
        pad along an axis
        """
        padded_img = np.zeros((self.img_.shape[0]+pad_x, self.img_.shape[1]+pad_y))

        start_x, end_x = pad_x // 2, self.img_.shape[0] - pad_x // 2
        start_y, end_y = pad_y // 2, self.img_.shape[1] - pad_y // 2

        padded_img[start_x:end_x,start_y:end_y] = self.img_.copy()
    
        return padded_img

    def calculate_cost_map(self):
        """
        Slice the image and take the min value in each slice 
        """
        pad_x = self.get_padding_dim( self.img_.shape[0], self.map_size_[0] )
        pad_y = self.get_padding_dim( self.img_.shape[1], self.map_size_[1] )

        padded_img = self.pad(pad_x, pad_y)

        m, n = padded_img.shape
        size_x, size_y = self.map_size_[0], self.map_size_[1]

        return padded_img.reshape(m//size_x, size_x, n//size_y, size_y).min(axis=(1,3))
