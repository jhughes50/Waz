"""
    Author:  Jason Hughes
    Date:    July 2024
    About:   A wrapper around the models 
    Package: Waz
"""
import sys
import cv2
import torch
import json
import numpy as np

from transformers import AutoImageProcessor, SegformerModel

class SegmentorParams:

    def __init__(self, path: str):

        self.device_ = str()
        self.config_ = str()
        self.model_path_ = str()


    def load(self, path: str):

        f = open(path)
        params = json.load(f)

        self.device_ = params["segformer"]["device"]
        self.config_ = params["segformer"]["config"]
        self.model_path_ = params["segformer"]["path"]

        f.close()
    
    @property
    def device(self):
        return self.device_

    @property
    def config(self):
        return self.config_

    @property 
    def model_path(self):
        return self.model_path_


class SegmentorWrapper:

    def __init__(path: str, mode_id: str):
        
        self.params_ = SegmentorParams(path)
        self.model_ = None
        self.device_ = 'cuda' if torch.cuda.is_available() else 'cpu'

    def load(self):
        self.model_ = init_segmentor(self.params_.config, self.params_.path, self.device_)

    def inference(self, x: np.ndarray) -> np.ndarray:
        return inference_segmentor(self.model_, x)


        
