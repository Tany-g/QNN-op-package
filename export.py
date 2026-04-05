import torch
import torch.nn as nn

class MiniModel(nn.Module):
    def forward(self, x):
        y = torch.relu(x)
        z = y + 1.0
        return z

model = MiniModel().eval()
x = torch.tensor([[-1.0, 0.5, 2.0]], dtype=torch.float32)

torch.onnx.export(
    model,
    x,
    "work/mini.onnx",
    input_names=["input"],
    output_names=["output"],
    opset_version=13
)

print("exported: work/mini.onnx")
