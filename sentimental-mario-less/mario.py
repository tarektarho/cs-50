# Define the main function
def main():
    # Get the height of the pyramid from the user
    height = get_height()
    # Build the pyramid using the given height
    build_pyramid(height)


# Define a function to get the height of the pyramid from the user
def get_height():
    # Keep prompting the user until a valid height within the desired range is entered
    while True:
        try:
            # Prompt user for height
            height = int(input("Height: "))
            # Check if the height is within the desired range (1 to 8)
            if 1 <= height <= 8:
                # Return the valid height
                return height
        except ValueError:
            # If the user enters a non-integer value, print an error message
            print("Not an Int")


# Define a function to build the pyramid
def build_pyramid(height):
    # Loop to add a new line for each row of the pyramid
    for i in range(height):
        # Loop to add spaces before the hashes on each row
        for k in range(height - i, 1, -1):
            print(" ", end="")
        # Loop to add hashes on each row
        for j in range(i + 1):
            print("#", end="")
        # Move to the next line after each row is printed
        print()


# Call the main function to start the program
main()
