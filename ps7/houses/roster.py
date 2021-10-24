from cs50 import SQL
import sys

# Gets a roster by house name


def main():
    # Checks number of input arguments
    if len(sys.argv) != 2:
        return 1

    # Connects to database
    db = SQL("sqlite:///students.db")

    house = sys.argv[1]

    # Query the database
    students = db.execute("SELECT first, middle, last, birth FROM students WHERE house = ? ORDER BY last, first", house)

    # Print the result
    for student in students:
        if student['middle']:
            print(student['first'] + " " + student['middle'] + " "
                  + student['last'] + ", born " + str(student['birth']))
        else:
            print(student['first'] + " " + student['last'] +
                  ", born " + str(student['birth']))


main()
