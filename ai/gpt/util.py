import time


class Timer:
    def __init__(self):
        self.start = time.time()

    def reset(self):
        self.start = time.time()

    def print(self, name: str):
        print(f"{name} took {time.time() - self.start:.2f} seconds")

    def lap(self, name: str) -> None:
        self.print(name)
        self.reset()
