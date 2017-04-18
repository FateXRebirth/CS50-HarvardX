import cs50
import sys

# ensure proper usage
    if len(sys.argv) != 2:
        sys.exit("Usage: ./caesar k")
# ensure proper usage
if int(sys.argv[1]) < 0:
    print("k must be a non-negative integer.")
    sys.exit(1)

k = int(sys.argv[1])
# variable of position 
index = 0
# variable for calculating
temp = 0
# new string
ans = ""

print("plaintext:  ", end = "")
s = cs50.get_string()
for i in s:
    # if it is letter
    if str.isalpha(i):
        # if it is uppercase
        if str.isupper(i):
            index = ord(i) - 65
            temp = ( index + k ) % 26
            ans += chr(temp + 65)
        # if it is lowercase
        else:
            index = ord(i)- 97
            temp = ( index + k ) % 26
            ans += chr(temp + 97)
    else:
        ans += i
print("ciphertext: {}".format(ans))
