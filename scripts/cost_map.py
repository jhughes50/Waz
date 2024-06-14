"""
    Author: Jason Hughes
    Date:   June 2024
    About:  Cost map based on semantics
"""
import numpy as np
from scipy import stats
from semantics import SemanticMappings

class SemanticCostMap:

    def __init__(self, img, results, div = 4):
        h, w, c = sem.shape
        self.height_ = h // div
        self.width_  = w // div
        self.div_    = div
        
        self.semantic_gt_ = SemanticMappings(results)

        self.binary_map_ = self.initialize_binary_map(img)
        self.index_map_ = self.initialize_index_map()

    @property
    def binary_map(self):
        return self.binary_map_

    @property
    def index_map(self):
        return self.index_map_

    def initialize_binary_map(self, img: np.ndarray) -> Tuple:
        temp_map = list()
        for i in range(self.height_):
            temp_map_j = list()
            for j in range(self.width_):
                subsec = self.img[i:(i+1)*self.div_, j:(j+1)*self.div_]
                mode   = stats.mode(subsec)
                if mode == self.semantic_gt_["road"]:
                    temp_map_j.append(1)
                else:
                    temp_map_j.append(0)

            temp_map.append(temp_map_j)

        return tuple(temp_map)

    def initialize_index_map(self) -> np.ndarray:
        return np.arange(self.height_ * self.width_).reshape((self.height_, self.width_))

    def report(self):
        print("[COST-MAP] height: %s, width %s" %(self.height_, self.width_))
