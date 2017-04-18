import cs50

print("O hai! ", end = "")

x = 0.0

# ensure proper usage
while True:
    print("How much change is owed?")
    x = cs50.get_float()
    if x >= 0.0:
        break
# deal with number convert
n = (int)(x * 100 + 0.5)

# variable for each type of coins
count = 0
quarter = 25
dime = 10
nickel = 5
penny = 1

# count how many coins need
while n >= quarter:
    count += 1
    n -= quarter
while n >= dime:
    count += 1
    n -= dime
while n >= nickel:
    count += 1
    n -= nickel
while n >= penny:
    count += 1
    n -= penny

print(count)