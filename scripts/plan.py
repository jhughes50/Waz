"""
    Author: Jason Hughes
    Date:   June 2024
    About:  A brief hardcoded path planner using semantics 
            and relative depth.
"""
import PIL
import numpy as np
from typing import List, Tuple, Dict

from bfs import BinaryBreadthFirstSearch
from semantics import SemanticMappings
from cost_map import SemanticCostMap

class ManetPlanner:

    def __init__(self, h, w):
        """
            Pass the hieght and width of the image,
            we'll use this to inform where we are in the image
        """
        self.height_ = h
        self.width_  = w
        self.start_  = (w//2, h)

        self.destination_ = None

    @destination.setter
    def destination(self, dest: tuple):
        self.destination_ = dest

    def cancel_condition(self, d):
        if d[0] < 0 or d[0] > self.width_:
            return False
        elif d[1] > self.height_:
            return False
        else:
            return True

    def semantic_planner(self, img: PIL.Image, seg: PIL.Image):
        cost_map = SemanticCostMap(np.array(seg))
        start = Pair(self.start_[0], self.start_[1])
        end = Pair(self.destination_[0], self.destination_[1])
        

    def depth_planner(self, img: PIL.Image, dep: PIL.Image):
        pass

    def semantic_depth_planner(self, img: PIL.Image, seg: PIL.Image, dep: PIL.Image):
        pass
