from pathlib import Path
import matplotlib.pyplot as plt

test = Path().cwd() / "test.out"

text = test.read_text().split("\n")[:-1]

numbers = [int(t.split("=")[-1]) for t in text]
plt.hist(numbers, bins=200)
plt.show()
