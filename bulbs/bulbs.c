#include <cs50.h>
#include <stdio.h>
#include <string.h>

const int BITS_IN_BYTE = 8;

// Function prototype to print bulb emoji based on bit value
void print_bulb(int bit);

int main(void)
{
    // Get user input message
    string message = get_string("Message: ");

    // Calculate the length of the message
    int length = strlen(message);

    // Array to store binary representation of each character (byte)
    int binary[BITS_IN_BYTE];

    // Variable to store the remainder when dividing by 2
    int remainder;

    // Loop through each character in the message
    for (int i = 0; i < length; i++)
    {
        // Convert the character to an unsigned char (1 byte)
        unsigned char byte = (int)message[i];

        // Temporary variable to store the quotient when dividing by 2
        int quotient = byte;

        // Loop to calculate the binary representation of the byte
        for (int j = 0; j < BITS_IN_BYTE; j++)
        {
            // Calculate the remainder when dividing by 2
            remainder = quotient % 2;

            // Store the remainder (bit) in the binary array
            binary[j] = remainder;

            // Update the quotient for the next iteration
            quotient /= 2;
        }

        // Loop to print the binary representation of the byte as bulb emojis
        for (int k = BITS_IN_BYTE - 1; k >= 0; k--)
        {
            print_bulb(binary[k]);
        }
        printf("\n");
    }

}


// Function to print the bulb emoji based on the bit value
void print_bulb(int bit)
{
    if (bit == 0)
    {
        // Dark emoji
        printf("\U000026AB");
    }
    else if (bit == 1)
    {
        // Light emoji
        printf("\U0001F7E1");
    }
}
