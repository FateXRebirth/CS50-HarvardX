import crypt
import sys
import cs50

# function to test all password
def tryPassword(ciphertext, salt):
    # dictionary for all possible letters
    dictionary = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
    # Compare
    plaintext = ""
    # save each word
    password = ""
    # _ test case only one letter 
    for letter in dictionary:
        password = letter
        plaintext = crypt.crypt(password, salt)
        if plaintext == ciphertext:
            return password
    # __ test case two letters
    password = ""
    plaintext = ""
    for letter1 in dictionary:
        password += letter1
        for letter2 in dictionary:
            password += letter2
            plaintext = crypt.crypt(password, salt)
            if plaintext == ciphertext:
                return password
            password = password[0:1]
        password = password[:0]
    # ___ test case three letters
    password = ""
    plaintext = ""
    for letter1 in dictionary:
        password += letter1
        for letter2 in dictionary:
            password += letter2
            for letter3 in dictionary:
                password += letter3
                plaintext = crypt.crypt(password, salt)
                if plaintext == ciphertext:
                    return password
                password = password[0:2]
            password = password[0:1]
        password = password[:0]
    # ____ test case four letters
    password = ""
    plaintext = ""
    for letter1 in dictionary:
        password += letter1
        for letter2 in dictionary:
            password += letter2
            for letter3 in dictionary:
                password += letter3
                for letter4 in dictionary:
                    password += letter4
                    plaintext = crypt.crypt(password, salt)
                    if plaintext == ciphertext:
                        return password
                    password = password[0:3]
                password = password[0:2]
            password = password[0:1]
        password = password[:0]
    
    # no match password
    return ""

def main():
    # ensure proper usage
    if len(sys.argv) != 2:
        sys.exit("Usage: ./crack hash")
    # get ciphertext
    ciphertext = sys.argv[1]
    # get salt
    salt = ciphertext[:2]
    # test - > print("ciphertext-> {} salt-> {}".format(ciphertext,salt))
    plaintext = tryPassword(ciphertext, salt)
    print(plaintext)
    return

if __name__ == '__main__':
    main()