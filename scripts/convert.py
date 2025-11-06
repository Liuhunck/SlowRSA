while True:
    x = input("->")
    a = [int(t, 16) for t in x.split()]

    res = 0
    for t in reversed(list(a)):
        res = res << 64 | t
    print(res)
