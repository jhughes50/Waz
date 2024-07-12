"""
    Author:  Jason Hughes
    Date:    July 2024
    About:   Convert the pytorch 
    Package: Waz
"""
import sys
import torch
import numpy as np
import matplotlib.pyplot as plt
import cv2 
import torch.onnx
from torchvision.transforms import transforms

sys.path.insert(1,'/home/jason/models/Depth-Anything-V2')

from transformers import AutoImageProcessor, AutoModelForDepthEstimation
from transformers import SegformerImageProcessor, SegformerForSemanticSegmentation
transform = transforms.Compose([transforms.ToTensor()])

def test(model_id):
    trace_img = cv2.imread("test-img-1.png")

    if model_id == "depth":
        print("Validating Depth-Anything-V2 model...\n")
        image_processor = AutoImageProcessor.from_pretrained("depth-anything/Depth-Anything-V2-Base-hf")
        inputs = image_processor(images=trace_img, return_tensors="pt")

        print("Input Shape: ", inputs['pixel_values'].shape)
        f_name = "/home/jason/models/depth-anything-v2-base-traced.pt"
        jit_model = torch.jit.load(f_name)
        jit_model.eval()
        
        output = jit_model(inputs["pixel_values"])
        tensor = output[0].detach().squeeze(0)
        np_array = tensor.numpy()

        plt.imshow(np_array)
        plt.savefig("jit_model_depth.png")

        hf_model = AutoModelForDepthEstimation.from_pretrained("depth-anything/Depth-Anything-V2-Base-hf", torchscript=True)    
        hf_model.eval()
        
        output = hf_model(inputs["pixel_values"])
        tensor = output[0].detach().squeeze(0)
        np_array = tensor.numpy()

        plt.imshow(np_array)
        plt.savefig("hf_model_depth.png")

        print("See saved images, done.")


if __name__ == "__main__":

    test("depth")
