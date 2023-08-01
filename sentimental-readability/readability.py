from cs50 import get_string


def main():
    text = get_string("Text: ")
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)

    # Calculate the average number of letters and sentences per 100 words
    L = letters / words * 100
    S = sentences / words * 100

    # Calculate the Coleman-Liau index
    index = round(0.0588 * L - 0.296 * S - 15.8)

    # Print the grade level as defined by the Coleman-Liau index (e.g. "Grade 2" or "Grade 8" or the like)
    if index > 16:
        print("Grade 16+")
    elif index < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {index}")


def count_letters(text):
    # Initialize the counter variable
    counter = 0

    # Loop through each character in the text
    for char in text:
        # Check if the character is an alphabetic letter
        if char.isalpha():
            counter += 1

    # Return the count of letters
    return counter


def count_words(text):
    # Split the text into words based on whitespace characters
    words = text.split()

    # Return the count of words
    return len(words)


def count_sentences(text):
    # Initialize the counter variable
    counter = 0

    # Loop through each character in the text
    for char in text:
        # Check if the character is a period, exclamation mark, or question mark
        if char in [".", "!", "?"]:
            counter += 1

    # Return the count of sentences
    return counter


main()
