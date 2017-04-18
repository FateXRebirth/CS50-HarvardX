import cs50

n = 0

# ensure proper usage
while True:
    print("Height: ", end = "")
    n = cs50.get_int()
    if n >= 0 and n < 24:
        break;
    

x = 2
y = n - 1

# print half-pyramid
for i in range(0, n):
    for j in range(y, 0, -1):
        print(" ", end = "")
    for k in range(0, x, 1):
        print("#", end = "")
    x += 1
    y -= 1
    print()
    