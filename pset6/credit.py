import cs50

# get coefficient of each bit
def getCoefficient(n):
    x = pow(10, n)
    return x
    
def main():
    print("Number: ", end = "")
    n = cs50.get_float()
    
    # variable for calculating
    sum1 = 0
    sum2 = 0
    sum3 = 0
    temp1 = 0 
    temp2 = 0
    temp3 = 0
    first = 0
    second = 0
    
    for i in range(0, 16, 1):
        # sort even bit out
        if i % 2 == 0:
            temp1 = (n // getCoefficient(i)) % getCoefficient(1)
            second = temp1
            sum2 = sum2 + temp1
        # sort odd bit out
        else:
            temp2 = (n // getCoefficient(i)) % getCoefficient(1)
            first = temp2
            temp2 = temp2 * 2
            if temp2 >= 10:
                temp3 = temp2 % 10
                temp2 = temp2 // 10
                temp2 = temp2 + temp3
            sum3 = sum3 + temp2
    
    sum1 = sum2 + sum3
    
    # determine type of each card
    if sum1 % 10 != 0:
        print("INVALID")
    else:
        if first == 5:
            print("MASTERCARD")
        elif first == 4:
            print("VISA")
        elif second == 3 and first == 0:
            print("AMEX")
        else:
            print("VISA")
    return


if __name__ == '__main__':
    main()