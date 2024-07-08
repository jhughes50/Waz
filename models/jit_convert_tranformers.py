"""
    Author:  Jason Hughes
    Date:    July 2024
    About:   Convert the pytorch 
    Package: Waz
"""
import sys
import torch
import cv2 
import torch.onnx
from torchvision.transforms import transforms

sys.path.insert(1,'/home/jason/models/Depth-Anything-V2')

#from depth_anything_v2.dpt import DepthAnythingV2
import transformers
from transformers import AutoImageProcessor, AutoModelForDepthEstimation
from transformers import SegformerImageProcessor, SegformerForSemanticSegmentation
transform = transforms.Compose([transforms.ToTensor()])

def convert(model_id):

    model_configs = {
        'vits': {'encoder': 'vits', 'features': 64, 'out_channels': [48, 96, 192, 384]},
        'vitb': {'encoder': 'vitb', 'features': 128, 'out_channels': [96, 192, 384, 768]},
        'vitl': {'encoder': 'vitl', 'features': 256, 'out_channels': [256, 512, 1024, 1024]},
        'vitg': {'encoder': 'vitg', 'features': 384, 'out_channels': [1536, 1536, 1536, 1536]}      }
    print("[JIT-CONVERT] Loading model from HuggingFace...")

    
    trace_img = cv2.imread("test-img.png")
    trace_img = cv2.cvtColor(trace_img, cv2.COLOR_BGR2RGB)
    trace_img = trace_img[:714,:1274,:]
    trace_tensor = transform(trace_img)
    trace_tensor = torch.unsqueeze(trace_tensor,0)

    if model_id == "depth":
        print("[JIT-CONVERT] Converting Depth-Anything-V2 model...\n")
        image_processor = AutoImageProcessor.from_pretrained("depth-anything/Depth-Anything-V2-Base-hf")
        model = AutoModelForDepthEstimation.from_pretrained("depth-anything/Depth-Anything-V2-Base-hf", torchscript=True)    
        model.eval()

        inputs = image_processor(images=trace_img, return_tensors="pt")
        traced_model = torch.jit.trace(model, trace_tensor)
        f_name = "depth-anything-v2-base-traced.pt"
        torch.jit.save(traced_model, f_name)

        print("[JIT-CONVERT] Converted and saved traced file: %s" %f_name)
        
    elif model_id == "segment":
        print("[JIT-CONVERT] Converting segformer model...")

        feature_extractor = SegformerImageProcessor.from_pretrained("nvidia/segformer-b5-finetuned-cityscapes-1024-1024")
        model = SegformerForSemanticSegmentation.from_pretrained("nvidia/segformer-b5-finetuned-cityscapes-1024-1024", torchscript=True)
        model.eval()

        inputs = feature_extractor(images=trace_img, return_tensors="pt")
        #print(inputs)
        traced_model = torch.jit.trace(model, inputs['pixel_values'])
        f_name = "segformer-b5-cityscapes-1024-traced.pt"
        torch.jit.save(traced_model, f_name)

        print("[JIT-CONVERT] Converted and saved traced file: %s" %f_name)

if __name__ == "__main__":

    if sys.argv[1] == "depth":
        convert("depth")
    elif sys.argv[1] == "segment":
        convert("segment")
    else:
        raise Exception("[JIT-CONVERT] Please provide a valid model name: depth or segment")
