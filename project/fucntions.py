from flask import *

def isPasswordValid(password):

    if hasDigit(password) and hasChar(password) and hasProperLength(password):
        return True
    else:
        return False


def apology(message, code=400):
    return render_template("apology.html", top=code, bottom=message)

# Checks if the string has one ore more digits
def hasDigit(inputString):

    for symbol in inputString:
        if symbol.isdigit():
            return True

    return False


# Checks if the string has one ore more letter
def hasChar(inputString):

    for symbol in inputString:
        if symbol.isalpha():
            return True

    return False

# Check if the string has more than 6 chars
def hasProperLength(inputString):

    if len(inputString) < 6:
        return False

    return True
