from cs50 import SQL
import sys
import csv

# Gets data from csv and save into DB
ls

def main():

    db = SQL("sqlite:///students.db")
    # Validate de number of inputs
    if len(sys.argv) != 2:
        return 1

    file = sys.argv[1]

    # Initialize variables
    data = []
    keys = []
    firstName = ''
    middleName = ''
    lastName = ''
    house = ''
    birth = ''

    # Open File
    with open(file, 'r') as file:

        # Read data and store into vatiables
        data = list(csv.DictReader(file))
        keys = list(data[0].keys())

        # Iterate over read data
        for dic in data:
            fullName = dic[keys[0]]
            fullName = fullName.split()

            if len(fullName) > 2:
                firstName = fullName[0]
                middleName = fullName[1]
                lastName = fullName[2]
            else:
                firstName = fullName[0]
                middleName = None
                lastName = fullName[1]

            house = dic[keys[1]]
            birth = dic[keys[2]]

            # Insert Query
            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)",
                       firstName, middleName, lastName, house, birth)


main()
