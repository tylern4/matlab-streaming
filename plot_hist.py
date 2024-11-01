from pathlib import Path
import matplotlib.pyplot as plt
import numpy as np
from scipy.stats import norm

test = Path().cwd() / "results.csv"

text = test.read_text().split("\n")[:-1]

numbers = np.array([int(t.split(",")[-1]) for t in text]) * 1E-6
n, bins, _ = plt.hist(numbers, bins=100, range=[0, np.max(numbers)])


mu, std = norm.fit(numbers)
x = np.linspace(np.min(numbers), np.max(numbers), 100)
p = norm.pdf(x, mu, std) * np.max(n)
plt.plot(x, p, 'k', linewidth=2, label='Fitted Gaussian')

# plt.yscale("log")
plt.xlabel("Milliseconds")
plt.show()
