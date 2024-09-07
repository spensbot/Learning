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
LOSS_FN = nn.CrossEntropyLoss()
DEVICE = util.pytorch_device("gpu")


def getModel(config: models.TransformerConfig):
    n_chars = len(data.chars2)

    transformer = models.Transformer(config)
    transformer.to(DEVICE)
    return transformer


def getBatch(dataSet, attention_config):
    n_l = attention_config.n_l
    batch_indexes = torch.randint(0, len(dataSet) - n_l, (BATCH_SIZE,))
    X = torch.stack([dataSet[i : i + n_l] for i in batch_indexes])
    Y = torch.stack([dataSet[i + 1 : i + n_l + 1] for i in batch_indexes])
    return X.to(DEVICE), Y.to(DEVICE)


def train(model, trainData, testData, batch_count: int) -> list[float]:
    losses = []
    optimizer = torch.optim.Adam(model.parameters(), lr=0.001)
    eval(model, testData)
    for batch in range(batch_count):
        X, Y = getBatch(trainData, model.attention_config)

        # forward pass
        optimizer.zero_grad()
        logits = model(X)  # (BATCH_SIZE, n_l, n_chars)
        B, T, C = logits.shape
        logits = logits.view(B * T, C)
        targets = Y.view(B * T)
        loss = LOSS_FN(logits, targets)
        losses.append(loss.item())

        # backward pass
        loss.backward()
        optimizer.step()

        if batch % 100 == 0:
            print(f"Batch {batch} Loss: {loss.item()}")
            eval(model, testData)
    return losses


def eval(model, data) -> None:
    with torch.no_grad():
        X, Y = getBatch(data, model.attention_config)

        logits = model(X)  # (BATCH_SIZE, n_l, n_chars)
        B, T, C = logits.shape
        logits = logits.view(B * T, C)
        targets = Y.view(B * T)
        loss = LOSS_FN(logits, targets)

        print(f"Test Loss: {loss.item()}")


def generate(model, seed: str, count: int) -> str:
    n_l = model.attention_config.n_l
    n_pad = max(0, n_l - len(seed))
    print(f"n_pad: {n_pad}")
    tokens = data.encode(" " * n_pad + "\n" + seed).unsqueeze(0).to(DEVICE)  # (B, L)
    # tokens = torch.zeros((1, n_l), dtype=torch.long).to(DEVICE)  # (B, L)

    with torch.no_grad():
        for i in range(count):
            context = tokens[:, -n_l:]  # (1, n_l)
            logits = model(context)  # (1, n_l, n_chars)
            prediction_logits = logits[:, -1, :]  # (1, n_chars)
            probs = torch.softmax(prediction_logits, dim=-1)
            next_token = torch.multinomial(probs, 1)  # (1, 1)
            tokens = torch.cat([tokens, next_token], dim=1)  # (1, L)

        return data.decode(tokens[0].tolist())[n_pad:]


if __name__ == "__main__":
    model = getModel(
        models.TransformerConfig(
            n_tokens=len(data.chars2),
            n_layers=2,
            attention=models.MultiHeadAttentionConfig(n_heads=8, n_qkv=64, masked=True),
            layer_norm=True,
            dropout=0.1,
            residual=True,
        )
    )

    print("\n\nBEFORE TRAINING\n\n")
    seed = "LEWIS: What is't thou doing, fool? I thought we would feast this day?\nKING GODDARD: "
    print(generate(model, seed, 1000))

    trainSet, testSet = data.load(0.8)
    timer.lap("Loading")
    losses = train(model, trainSet, testSet, 1000)
    timer.lap("Training")

    print("\n\nAFTER TRAINING\n\n")
    print(generate(model, seed, 1000))
    timer.lap("Generate")

    plt.plot(losses)
    plt.show()
