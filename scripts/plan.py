"""
    Author: Jason Hughes
    Data: June 2024
    About: A brief hardcoded paht planner using semantics 
    and relative depth.
"""

class ManetPlanner:

    def __init__(self, h, w):
        """
            Pass the hieght and width of the image,
            we'll use this to inform where we are in the image
        """
        self.height_ = h
        self.width_ = w
        self.start_ = (w//2, 0)

    def cancel_condition(self, d):
        if d[0] < 0 or d[0] > self.width_:
            return False
        elif d[1] > self.height_:
            return False
        else:
            return True

    def semantic_planner(self, img, seg):
        dot = self.start_

        while cancel_condition(dot):
            


    def depth_planner(self, img, dep):
        pass

    def semantic_depth_planner(self, img, seg, dep):
        pass


