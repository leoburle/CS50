import sys
import csv


def main():
    # Check if the number of inputs in correct
    if len(sys.argv) != 3:
        print("Error")
        return 0

    databaseFile = sys.argv[1]
    dna = sys.argv[2]
    strs = []

    # Open the database file
    with open(databaseFile, 'r') as csvfile:
        database = csv.DictReader(csvfile)
        for row in database:
            # get all STRs on the database
            strs = list(row.keys())[1:]

    # opens the sequence file
    file2 = open(dna, "r")
    dna = file2.read()
    maxStrDic = getMaxStr(strs, dna)

    person = findPerson(databaseFile, csvfile, maxStrDic, strs)

    # prints the result
    if person:
        print(person)
    else:
        print("No match")


# get the maximum number of times each str is repeated on the dna
def getMaxStr(strs, dna):
    maxStrDic = {}
    for STR in strs:
        i = 0
        isInSequence = False
        numRepeats = 0
        maxRepeats = 0
        sizeOfStr = len(STR)
        while i < len(dna):
            if dna[i:i+sizeOfStr] == STR:
                i += sizeOfStr
                if (isInSequence):
                    numRepeats += 1
                else:
                    numRepeats = 1

                if numRepeats > maxRepeats:
                    maxRepeats = numRepeats

                isInSequence = True
            else:
                i += 1
                isInSequence = False

        maxStrDic[STR] = maxRepeats

    return maxStrDic


# Searches for the person on the database file that matches the max strs
def findPerson(databaseFile, csvfile, maxStrDic, strs):
    # Open the csv
    with open(databaseFile, 'r') as csvfile:
        # read the csv
        database = csv.DictReader(csvfile)
        # iterate over the csv data

        numberOfStrs = len(strs)

        for row in database:
            numberOfMatchingStrs = 0
            for key in maxStrDic.keys():
                # increments the number of matching strs
                if int(row[key]) == maxStrDic[key]:
                    numberOfMatchingStrs += 1
                # return the person that has the same max numbers of strs of maxStrDic
                if numberOfMatchingStrs == numberOfStrs:
                    return row['name']
    return None


main()
