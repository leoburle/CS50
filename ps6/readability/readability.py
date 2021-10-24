from cs50 import get_string


# main
def main():
    # get user input
    text = get_string("Text: ")
    # call function to return the grade.
    index = avgNumLetter(text)

    # print the grade of the text
    if index < 1:
        print("Before Grade 1")
    elif index > 16:
        print("Grade 16+")
    else:
        print("Grade "+str(index))


# function that calculates grade based in Coleman-Liau formula
def avgNumLetter(text):
    countWords = 0
    countSentences = 0
    countLetters = 0
    # iterates to each letter
    for letter in text:
        # calculates number of words
        if letter == " ":
            countWords += 1
        # calculates number of sentences
        elif letter == "." or letter == "!" or letter == "?":
            countSentences += 1
        # calculates number of letters
        elif (ord(letter) > 64 and ord(letter) < 91) or (ord(letter) > 96
                                                         and ord(letter) < 123):
            countLetters += 1
        else:
            continue

    countWords += 1
    # calculate the average number of letters and sentences
    # per 100 words
    L = (countLetters * 100) / countWords
    S = (countSentences * 100) / countWords
    # Coleman-Liau formula
    index = 0.0588 * L - 0.296 * S - 15.8

    return round(index)


main()
