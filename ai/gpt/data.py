import os
import torch


def read_gutenberg_books(dir_path: str) -> str:
    start_marker = "*** START OF THE PROJECT GUTENBERG EBOOK"
    end_marker = "*** END OF THE PROJECT GUTENBERG EBOOK"
    book_texts: str = ""

    i = 0

    for filename in os.listdir(dir_path):
        if filename.endswith(".txt"):
            i += 1
            if i > 1:
                break
            with open(os.path.join(dir_path, filename), "r", encoding="utf-8") as file:
                content = file.read()
                start_idx = content.find(start_marker)
                end_idx = content.rfind(end_marker)
                if start_idx != -1 and end_idx != -1:
                    start_idx = (
                        content.find("\n", start_idx) + 1
                    )  # Move to the end of the line containing the start marker
                    book_text = content[start_idx:end_idx].strip()
                    book_texts += book_text

    return book_texts


def read_shakespeare(dir_path: str) -> str:
    with open(
        os.path.join(dir_path, "t8.shakespeare.txt"), "r", encoding="utf-8"
    ) as file:
        return file.read()


# books = "".join(read_gutenberg_books("./gutenberg"))
# chars = ".".join(sorted(set("".join(books))))
chars2 = """ \n.!?"',-:;()[]*&%$#@abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"""
itoc = {i: c for i, c in enumerate(chars2)}
ctoi = {c: i for i, c in enumerate(chars2)}


def encode(text: str) -> torch.Tensor:
    list = [ctoi[c] for c in text if c in ctoi]
    return torch.tensor(list)


def decode(encoded_text: list[int]) -> str:
    return "".join(itoc[i] for i in encoded_text)


def load(split: float) -> tuple[torch.Tensor, torch.Tensor]:
    # all_text: str = read_shakespeare("./shakespeare")
    all_text: str = read_gutenberg_books("./gutenberg")
    n = len(all_text)
    split_idx = int(n * split)
    train_text = all_text[:split_idx]
    test_text = all_text[split_idx:]
    return (encode(train_text), encode(test_text))
