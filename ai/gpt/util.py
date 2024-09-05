import time
import torch
from typing import Literal

class Timer:
    def __init__(self):
        self.start = time.time()

    def reset(self):
        self.start = time.time()

    def print(self, name: str):
        print(f"\n{name} took {time.time() - self.start:.2f} seconds\n")

    def lap(self, name: str) -> None:
        self.print(name)
        self.reset()

def list_available_devices():
    devices = []
    
    # Check for CPU availability
    devices.append("CPU")

    # Check for GPU/CUDA availability
    if torch.cuda.is_available():
        num_gpus = torch.cuda.device_count()
        for i in range(num_gpus):
            device_name = torch.cuda.get_device_name(i)
            devices.append(f"CUDA:{i} - {device_name}")

    if torch.backends.mps.is_available():
        devices.append("mps")
        
    return devices

def __pytorch_device(type: Literal['cpu', 'gpu']):
    if type == "cpu":
        return torch.device("cpu")
    if torch.cuda.is_available():
        return torch.device("cuda")
    elif torch.backends.mps.is_available():
        return torch.device("mps")
    else:
        return torch.device("cpu")

def pytorch_device(type: Literal['cpu', 'gpu']):
    device = __pytorch_device(type)
    print(f"Using Device: {device}")
    return device


