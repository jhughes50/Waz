"""
    Author:  Jason Hughes
    Date:    July 2024
    About:   Convert the pytorch 
    Package: Waz
"""
import sys
import torch
import cv2 
from torchvision.transforms import transforms

sys.path.insert(1,'/home/jason/models/Depth-Anything-V2')

from depth_anything_v2.dpt import DepthAnythingV2

transform = transforms.Compose([transforms.ToTensor()])

def convert(model_id):

    model_configs = {
        'vits': {'encoder': 'vits', 'features': 64, 'out_channels': [48, 96, 192, 384]},
        'vitb': {'encoder': 'vitb', 'features': 128, 'out_channels': [96, 192, 384, 768]},
        'vitl': {'encoder': 'vitl', 'features': 256, 'out_channels': [256, 512, 1024, 1024]},
        'vitg': {'encoder': 'vitg', 'features': 384, 'out_channels': [1536, 1536, 1536, 1536]}      }
    print("loading model...")
    if model_id == "depth":
        encoder = 'vitb'
        print("converting depth model")
        model = DepthAnythingV2(**model_configs[encoder])
        model.load_state_dict(torch.load("depth_anything_v2_vitb.pth"))
        model.eval()
        
        trace_img = cv2.imread("test-img.png")
        trace_img = cv2.cvtColor(trace_img, cv2.COLOR_BGR2RGB)
        trace_img = trace_img[:714,:1274,:]
        trace_tensor = transform(trace_img)
        trace_tensor = torch.unsqueeze(trace_tensor,0)

        traced_model = torch.jit.trace(model, trace_tensor)

        torch.jit.save(traced_model, "depth_anything_v2_vitb_traced.pt")
        print("Saved traced depth model")
        
        script_model = torch.jit.script(model)
        torch.jit.save(script_mode, "depth_anything_v2_vitb_scripted.pt")

    elif model_id == "segment":
        print("converting segformer model")

if __name__ == "__main__":
    convert("depth")
