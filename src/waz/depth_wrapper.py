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

sys.path.insert(1,'/home/jason/models/Depth-Anything-V2')

from depth_anything_v2.dpt import DepthAnythingV2

class DepthParams:

    def __init__(self, path: str):

        self.device_ = str()
        self.encoder_ = str()
        self.model_path_ = str()

        self.model_configs_ = {
            'vits': {'encoder': 'vits', 'features': 64, 'out_channels': [48, 96, 192, 384]},
            'vitb': {'encoder': 'vitb', 'features': 128, 'out_channels': [96, 192, 384, 768]},
            'vitl': {'encoder': 'vitl', 'features': 256, 'out_channels': [256, 512, 1024, 1024]},
            'vitg': {'encoder': 'vitg', 'features': 384, 'out_channels': [1536, 1536, 1536, 1536]}
        }

    def load(self, path: str):

        f = open(path)
        params = json.load(f)

        self.device_ = params["depth"]["device"]
        self.encoder_ = params["depth"]["encoder"]
        self.model_path_ = params["depth"]["path"]

        f.close()

    @property
    def device(self):
        return self.device_

    @property
    def encoder(self):
        return self.encoder_

    @property 
    def model_path(self):
        return self.model_path_

    @property
    def model_config(self):
        return self.model_configs_[self.encoder_]


class DepthWrapper:

    def __init__(path: str, mode_id: str):
        
        self.params_ = DepthParams(path)
        self.model_ = None
        self.device_ = 'cuda' if torch.cuda.is_available() else 'cpu'

    def load(self):
        self.model_ = DepthAnythingV2(**self.params_.model_config)
        self.model.load_state_dict(torch.load(self.params_.model_path, map_location='cpu'))
        self.model_.to(self.device_).eval()

    def inference(self, x: np.ndarray) -> np.ndarray:
        return self.model_.infer_image(x)


        
