#Main function definition
from cs50 import get_int
def main():
    #ask for user input
    height = get_int("Height: ")

    #checks if the user inout is valid
    while (height < 1 or height > 8):
        #ask again for a valid inopt
        height = get_int("Height: ")

    #prints the pyramid
    for i in range(height):
        print(" " * (height - i - 1), end="")
        print("#" * (i + 1), end="")
        print("  ", end="")
        print("#" * (i + 1))
#Call main function
main()