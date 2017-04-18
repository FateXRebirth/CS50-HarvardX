import cs50

n = 0

# ensure proper usage
while True:
    print("Height: ", end = "")
    n = cs50.get_int()
    if n >= 0 and n < 24:
        break

x = n - 1
y = 1
z = 2

# print pyramid
for i in range(0, n):
    for j in range(x, 0, -1):
        print(" ", end = "")
    for k in range(0, y, 1):
        print("#", end = "")
    for w in range(0, z, 1):
        print(" ", end = "")
    for v in range(0, y, 1):
        print("#", end = "")
    x = x - 1
    y = y + 1
    print()
    