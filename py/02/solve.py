def accept(l):
    dl = lambda l: (l2 - l1 for (l1, l2) in zip(l[1:], l))
    return all(i <= 3 and i >= 1 for i in dl(l)) or all(i >= -3 and i <= -1 for i in dl(l))
with open("../../data/02.txt", "r") as f:
    rs = [[int(tok) for tok in l.split()] for l in f]
print("part1: %d" % sum(1 for r in rs if accept(r)))
print("part2: %d" % sum(1 for r in rs if any(accept(r[:i] + r[i+1:]) for i in range(len(r) + 1))))
