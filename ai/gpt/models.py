import torch
import torch.nn as nn
import torch.nn.functional as F


class MultiHeadAttentionConfig:
    def __init__(self, n_heads: int, n_qkv: int, masked=False):
        self.n_heads = n_heads
        self.n_qkv = n_qkv
        self.n_embed = (
            n_heads * n_qkv
        )  # necessary since the output of the heads will be concatenated
        self.n_l = self.n_embed  # necessary for the tril matrix to be square
        self.masked = masked

    def __str__(self):
        return f"MultiHeadAttentionConfig(n_heads={self.n_heads}, n_qkv={self.n_qkv}, n_embed={self.n_embed}, n_l={self.n_l}, masked={self.masked})"


class TransformerConfig:
    def __init__(
        self,
        n_tokens: int,
        n_layers: int,
        attention: MultiHeadAttentionConfig,
        layer_norm: bool = True,
        dropout: float = 0.1,
        residual: bool = True,
    ):
        self.n_tokens = n_tokens
        self.n_layers = n_layers
        self.attention = attention
        self.layer_norm = layer_norm
        self.dropout = dropout
        self.residual = residual

    def __str__(self):
        return f"""
TransformerConfig(n_tokens={self.n_tokens}, n_layers={self.n_layers}, layer_norm={self.layer_norm}, dropout={self.dropout}, residual={self.residual}
{self.attention}
)"""


class Embedding(nn.Module):
    def __init__(self, n_tokens: int, n_embed: int) -> None:
        super().__init__()
        self.table = nn.Parameter(torch.randn([n_tokens, n_embed]) * 0.1)
        print(f"Embedding.table.device: {self.table.device}")

    # In: tensor filled with dict indices from [0, n_dict)
    # Out: tensor with an added end dimention of n_embed
    def forward(self, x):
        return self.table[x]


class AttentionHead(nn.Module):
    def __init__(
        self,
        params: MultiHeadAttentionConfig,
    ) -> None:
        super().__init__()
        self.params = params
        self.q = nn.Linear(params.n_embed, params.n_qkv)
        self.k = nn.Linear(params.n_embed, params.n_qkv)
        self.v = nn.Linear(params.n_embed, params.n_qkv)
        mask = (
            torch.tril(torch.ones(params.n_l, params.n_l)).logical_not()
            if params.masked
            else None
        )
        self.register_buffer("mask", mask)

    # In: tensor (B, L, E): (batch, sequence length, embedding size)
    # Out: tensor (B, L, n_qkv): (batch, sequence length, n_qkv)
    def forward(self, x: torch.Tensor) -> torch.Tensor:
        q = self.q(x)  # (B, L, n_qkv)
        k = self.k(x)
        v = self.v(x)

        logits: torch.Tensor = (
            q @ k.transpose(-2, -1) / self.params.n_qkv**0.5
        )  # (B, L, L)
        if self.mask is not None:
            logits.masked_fill_(self.mask, float("-inf"))
        probs = F.softmax(logits, dim=-1)  # (B, L, L)
        return probs @ v  # (B, L, n_qkv)


class MultiHeadAttention(nn.Module):
    def __init__(self, params: MultiHeadAttentionConfig) -> None:
        super().__init__()
        self.params = params
        self.heads = nn.ModuleList(
            [AttentionHead(params) for _ in range(params.n_heads)]
        )
        self.linear = nn.Linear(params.n_embed, params.n_embed)

    # In: tensor (B, L, E) (batch, sequence length, embedding size)
    # Out: tensor (B, L, E)
    def forward(self, x):
        # TODO: Remove this cat for efficiency? Can you?
        x = torch.cat([head(x) for head in self.heads], dim=-1)
        x = self.linear(x)
        return x


class AttentionLayer(nn.Module):
    def __init__(self, config: TransformerConfig) -> None:
        super().__init__()
        self.config = config
        self.multiHeadAttention = MultiHeadAttention(config.attention)
        n_embed = config.attention.n_embed
        self.linear1 = nn.Linear(n_embed, n_embed * 4)
        self.linear2 = nn.Linear(n_embed * 4, n_embed)

    # In: tensor (B, L, E) (batch, sequence length, embedding size)
    # Out: tensor (B, L, E)
    def forward(self, x):
        x_res = x
        x = self.multiHeadAttention(x)
        x = AddAndNorm(x, x_res, self.config)
        x_res2 = x
        x = self.linear1(x)
        x = F.relu(x)
        x = self.linear2(x)
        return AddAndNorm(x, x_res2, self.config)


class Transformer(nn.Module):
    def __init__(self, config: TransformerConfig):
        super().__init__()
        self.config = config
        self.attention_config = config.attention
        self.token_embed = nn.Embedding(config.n_tokens, config.attention.n_embed)
        self.position_embed = nn.Embedding(
            config.attention.n_l, config.attention.n_embed
        )
        self.attention_layers = nn.ModuleList(
            [AttentionLayer(config) for _ in range(config.n_layers)]
        )
        self.linear = nn.Linear(config.attention.n_embed, config.n_tokens)

    # In: tensor (B, L) of token indices
    # Out: tensor (B, L, T) of token probabilities
    def forward(self, x):
        token_embedding = self.token_embed(x)  # (B, L, E)
        position_embedding = self.position_embed(
            torch.arange(0, self.config.attention.n_embed).to(x.device)
        )  # (L, E)

        # TODO: Try without positional embedding to see how much of an impact it has.
        x = token_embedding + position_embedding  # (B, L, E)

        # TODO: Try Concatenation instead of addition...
        # My hunch is that adding them probably allows the model to learn how best to distribute the available embedding depth.
        # x = torch.cat([token_embedding, position_embedding], dim=2)

        for layer in self.attention_layers:
            x = layer(x)  # (B, L, E)

        x = self.linear(x)  # (B, L, T)

        return x


def AddAndNorm(
    x: torch.Tensor, x_res: torch.Tensor, config: TransformerConfig
) -> torch.Tensor:
    if config.residual:
        x += x_res
    if config.layer_norm:
        x = LayerNorm(x, layerDim=1)
    return x


# TODO: Turn LayerNorm into a Module with learnable beta and gamma params
def LayerNorm(x: torch.Tensor, layerDim: int, eps: float = 1e-5) -> torch.Tensor:
    mean = x.mean(dim=layerDim, keepdim=True)
    var = x.var(dim=layerDim, keepdim=True)
    return (x - mean) / torch.sqrt(var + eps)


# These shouldn't be required until the layer count is increased to a high number. >3ish in my experience
# TODO: Residual connections
# TODO: LayerNorm
# TODO: Dropout
