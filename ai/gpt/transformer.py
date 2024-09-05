import util

timer = util.Timer()
import torch
import torch.nn as nn
import torch.nn.functional as F
import data
import matplotlib.pyplot as plt
import models

timer.lap("Imports")

BATCH_SIZE = 64

n_chars = len(data.chars2)
device = util.pytorch_device('gpu')
attention_params = models.MultiHeadAttentionParams(n_heads=4, n_qkv=64, masked=True)
n_l = attention_params.n_l
print(attention_params)

transformer = models.Transformer(
    n_tokens=n_chars, n_layers=2, attention_params=attention_params
)
transformer.to(device)
loss_fn = nn.CrossEntropyLoss()
optimizer = torch.optim.Adam(transformer.parameters(), lr=0.001)

losses = []


def getBatch(data):
    batch_indexes = torch.randint(0, len(data) - n_l, (BATCH_SIZE,))
    X = torch.stack([data[i : i + n_l] for i in batch_indexes])
    Y = torch.stack([data[i + 1 : i + n_l + 1] for i in batch_indexes])
    return X.to(device), Y.to(device)


def train(model, data, batch_count: int) -> None:
    eval(model, testSet)
    for batch in range(batch_count):
        X, Y = getBatch(data)

        # forward pass
        optimizer.zero_grad()
        logits = model(X)  # (BATCH_SIZE, n_l, n_chars)
        B, T, C = logits.shape
        logits = logits.view(B * T, C)
        targets = Y.view(B * T)
        loss = loss_fn(logits, targets)
        losses.append(loss.item())

        # backward pass
        loss.backward()
        optimizer.step()

        if batch % 100 == 0:
            print(f"Batch {batch} Loss: {loss.item()}")
            eval(model, testSet)


def eval(model, data) -> None:
    with torch.no_grad():
        X, Y = getBatch(data)

        logits = model(X)  # (BATCH_SIZE, n_l, n_chars)
        B, T, C = logits.shape
        logits = logits.view(B * T, C)
        targets = Y.view(B * T)
        loss = loss_fn(logits, targets)
        losses.append(loss.item())

        print(f"Test Loss: {loss.item()}")


def generate(model, len: int) -> str:
    tokens = torch.zeros((1, n_l), dtype=torch.long).to(device)  # (B, L)
    print(f"tokens.device: {tokens.device}")

    with torch.no_grad():
        for i in range(len):
            context = tokens[:, -n_l:]  # (1, n_l)
            logits = model(context)  # (1, n_l, n_chars)
            prediction_logits = logits[:, -1, :]  # (1, n_chars)
            probs = torch.softmax(prediction_logits, dim=-1)
            next_token = torch.multinomial(probs, 1)  # (1, 1)
            tokens = torch.cat([tokens, next_token], dim=1)  # (1, L)

        return data.decode(tokens[0].tolist())


print("\n\nBEFORE TRAINING\n\n")
print(generate(transformer, 1000))

trainSet, testSet = data.load(0.8)
timer.lap("Loading")
train(transformer, trainSet, 5000)
timer.lap("Training")

print("\n\nAFTER TRAINING\n\n")
print(generate(transformer, 1000))
timer.lap("Generate")

plt.plot(losses)
plt.show()
