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

from transformers import SegformerImageProcessor, SegformerForSemanticSegmentation
transform = transforms.Compose([transforms.ToTensor()])

def test(model_id):
    trace_img = cv2.imread("test-img-1.png")

    if model_id == "depth":
        print("Validating SegFormer model...\n")
        image_processor = SegformerImageProcessor.from_pretrained("nvidia/segformer-b5-finetuned-cityscapes-1024-1024")
        inputs = image_processor(images=trace_img, return_tensors="pt")
       
        inp = inputs["pixel_values"]
        inp = inp.squeeze().permute(1,2,0)
        np_array = inp.numpy()
        
        if np_array.dtype == np.float32 or np_array.dtype == np.float64:
            np_array = (np_array * 255).clip(0, 255).astype(np.uint8)
    
        # Convert RGB to BGR if it's a color image
        if np_array.shape[-1] == 3:
            np_array = cv2.cvtColor(np_array, cv2.COLOR_RGB2BGR)
        cv2.imshow("img", np_array)
        cv2.waitKey(0)
        cv2.destroyAllWindows()
        #plt.imshow(inp)
        #plt.savefig("test.png")

        print("Input Shape: ", inputs['pixel_values'].shape)
        f_name = "/home/jason/models/segformer-b5-cityscapes-1024-traced.pt"
        jit_model = torch.jit.load(f_name)
        jit_model.eval()
        
        output = jit_model(inputs["pixel_values"])
        #tensor = output[0].detach().squeeze(0)
        #np_array = tensor.numpy()

        #plt.imshow(np_array)
        #plt.savefig("jit_model_depth.png")

        hf_model = SegformerForSemanticSegmentation.from_pretrained("nvidia/segformer-b5-finetuned-cityscapes-1024-1024", torchscript=True)    
        hf_model.eval()
        
        #output = hf_model(inputs["pixel_values"])
        #tensor = output[0].detach().squeeze(0)
        #np_array = tensor.numpy()

        #plt.imshow(np_array)
        #plt.savefig("hf_model_depth.png")

        print("See saved images, done.")


if __name__ == "__main__":

    test("depth")
