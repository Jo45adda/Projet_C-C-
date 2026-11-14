import math
import time
import numpy as np


def cholesky_tridiag(d, s):
    n = len(d)
    c = [0.0] * n
    l = [0.0] * (n - 1)
    if d[0] <= 0:
        raise ValueError("Pas definie positive")
    c[0] = math.sqrt(d[0])
    for i in range(1, n):
        l[i - 1] = s[i - 1] / c[i - 1]
        t = d[i] - l[i - 1] ** 2
        if t <= 0:
            raise ValueError("Pas definie positive")
        c[i] = math.sqrt(t)
    return c, l


def descente(c, l, b):
    n = len(c)
    y = [0.0] * n
    y[0] = b[0] / c[0]
    for i in range(1, n):
        y[i] = (b[i] - l[i - 1] * y[i - 1]) / c[i]
    return y


def remontee(c, l, y):
    n = len(c)
    x = [0.0] * n
    x[n - 1] = y[n - 1] / c[n - 1]
    for i in range(n - 2, -1, -1):
        x[i] = (y[i] - l[i] * x[i + 1]) / c[i]
    return x


def solveur_chol_tridiag(A, b):
    n = len(b)
    d = [A[i, i] for i in range(n)]
    s = [A[i + 1, i] for i in range(n - 1)]
    c, l = cholesky_tridiag(d, s)
    y = descente(c, l, b)
    return remontee(c, l, y)


# Test
for n in [5, 10, 100]:
    A = 2 * np.eye(n) - np.eye(n, k=1) - np.eye(n, k=-1)
    b = np.zeros(n)
    b[0] = 1
    b[-1] = 1
    x = solveur_chol_tridiag(A, b)
    print(f"n={n}, erreur={np.max(np.abs(np.array(x) - 1)):.2e}")

# Temps
for n in [100000, 200000, 400000]:
    d = [2.0] * n
    s = [-1.0] * (n - 1)
    b = [0.0] * n
    b[0] = 1.0
    b[-1] = 1.0

    t0 = time.time()
    c, l = cholesky_tridiag(d, s)
    y = descente(c, l, b)
    x = remontee(c, l, y)
    print(f"n={n} : {time.time() - t0:.4f}s")