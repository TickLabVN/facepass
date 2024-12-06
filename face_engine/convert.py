import torch
from ultralytics import YOLO

model = YOLO("yolov8n-face.pt").to(torch.device("cuda"))
model.export(format="onnx", imgsz=(640, 640), opset=12)