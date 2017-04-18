import cs50
import sys

# select key from a string
def selectK(k, index):
    if str.isupper(k[index]):
        return ord(k[index]) - 65
    else:
        return ord(k[index]) - 97

def main():
    # ensure proper usage
    if len(sys.argv) != 2:
        sys.exit("Usage: ./vigenere k")
    # ensure proper usage
    keyword = sys.argv[1]
    for i in keyword:
        if not str.isalpha(i):
            print("k must be letters")
            sys.exit(1)
    
    k_length = len(keyword)
    k = 0
    k_index = 0
    index = 0
    temp = 0
    ans = ""
    
    print("plaintext:  ", end = "")
    s = cs50.get_string()
    for i in s:
        if k_index == k_length:
            k_index = 0
        # check if it's letter
        if str.isalpha(i):
            # if it's uppercase
            if str.isupper(i):
                index = ord(i) - 65
                k = selectK(keyword, k_index)
                temp = ( index + k ) % 26
                ans += chr(temp + 65)
            # if it's lowercase
            else:
                index = ord(i) - 97
                k = selectK(keyword, k_index)
                temp = ( index + k ) % 26
                ans += chr(temp + 97)
            k_index += 1
        else:
            ans += i
    
    print("ciphertext: {}".format(ans))
    return

if __name__ == '__main__':
    main()