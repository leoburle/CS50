from cs50 import get_int


def main():
    cardNum = get_int("Number:")
    flag = luhnAlg(cardNum)
    # Check if there is a valid card flag and prints it
    if (flag):
        print(flag)
    else:
        print("INVALID")


# Applies Lunh's Algorithm


def luhnAlg(cardNum):
    cardNum = str(cardNum)

    total = 0

    size = len(cardNum)
    for i in range(size):
        digit = int(cardNum[size - i - 1])
        if (i % 2) == 0:
            total += digit
        else:
            digit = digit * 2
            # Gets the first digit of the result and add to total
            total += digit % 10
            digit = digit / 10
            # Gets the second digit of the result and add to total
            total += int(digit) % 10

    total = str(total)
    if (total[-1] == "0"):
        flag = checkFlag(cardNum)
        if (flag):
            return flag
        else:
            return None
    else:
        return None


def checkFlag(cardNum):
    size = len(cardNum)
    firstDigits = cardNum[0] + cardNum[1]
    if not(size == 15 or size == 16 or size == 13):
        return None
    else:
        # AMEX Credit Card Number formation rule
        if (firstDigits == "34" or firstDigits == "37") and (size == 15):
            return "AMEX"
        # MASTERCARD Credit Card Number formation rule
        elif (firstDigits == "51" or firstDigits == "52" or firstDigits == "53"
                or firstDigits == "54" or firstDigits == "55") and (size == 16):
            return "MASTERCARD"
        # VISA Credit Card Number formation rule
        elif (firstDigits[0] == "4" and (size == 13 or size == 16)):
            return "VISA"
        else:
            return None


main()
