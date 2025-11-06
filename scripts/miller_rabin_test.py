from primes import PRIMES
import random


def is_prime(w) -> bool:
    for i in PRIMES:
        if w % i == 0:
            return False

    w1 = w - 1
    m = w1
    a = 0
    while m % 2 == 0:
        m >>= 1
        a += 1

    iter = 128
    while iter != 0:
        iter -= 1

        b = random.randint(2, w1)

        z = pow(b, m, w)
        if z == 1 or z == w1:
            continue

        pprime = False
        for _ in range(1, a):
            z = (z * z) % w
            if z == w1:
                pprime = True
                break
            if z == 1:
                return False

        if not pprime:
            return False

    return True


l = 2**511
r = 2**512


x = random.randint(l + 1, r - 1) | 3

for i in range(10000000000):
    if is_prime(x) and is_prime(x // 2):
        print(i, x)
        break
    x += 4

print("End")
