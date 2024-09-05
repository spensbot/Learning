# Makemore Notes

- Use mini-batch gradient descent to increase training speed and discourage getting stuck in local minima
- Shrink initial weights so the initial model has a loss closer to a model that guesses at random. Get into the meat of the training sooner. No longer need to waste initial training pruning wack weights
- Try leaky relu to prevent "brain-damaged" neurons with saturated activations that produce no gradient
- implement batch norm
-
