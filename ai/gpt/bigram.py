import torch
import torch.nn as nn
import torch.nn.functional as F
import data
import util
import matplotlib.pyplot as plt
import models

BATCH_SIZE = 128


n_chars = len(data.chars2)
bigram = models.Embedding(n_chars, n_chars)
encoded = data.load(0.8)[0]
loss_fn = nn.CrossEntropyLoss()
optimizer = torch.optim.Adam(bigram.parameters(), lr=0.01)

losses = []


def train(model, encoded, batch_count: int) -> None:
    for batch in range(batch_count):
        iX = torch.randint(0, len(encoded) - 1, (BATCH_SIZE,))
        iY = iX + 1
        X = encoded[iX]
        Y = encoded[iY]

        # forward pass
        optimizer.zero_grad()
        logits = model(X)
        # print(logits.shape)
        loss = loss_fn(logits, Y)
        losses.append(loss.item())

        # backward pass
        loss.backward()
        optimizer.step()

        # print(f"Batch {batch} Loss: {loss.item()}")


def generate(model, len: int) -> str:
    output = " "

    with torch.no_grad():
        for i in range(len):
            last_char = output[-1]
            encoded = torch.tensor(data.encode(last_char)).squeeze()
            logits = model(encoded).squeeze()
            # print(logits)
            probs = torch.softmax(logits, dim=0)
            # print(probs)
            next_char = int(torch.multinomial(probs, 1).item())
            output += data.decode([next_char])

        return output


print(generate(bigram, 1000))

timer = util.Timer()
train(bigram, encoded, 1000)
timer.lap("Training")

print(generate(bigram, 1000))

plt.plot(losses)
plt.show()
