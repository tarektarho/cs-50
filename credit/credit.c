#include <cs50.h>
#include <stdio.h>
#include <math.h>

int every_other_digit(long card_number);
int multiply_and_sum(int last_digit);
int number_of_digits(long card_number);
bool is_valid_amex(long card_number, int num_digit);
bool is_valid_master_card(long card_number, int num_digits);
bool is_valid_visa(long card_number, int num_digits);

int main(void)
{

    // Get the credit card number from the user
    long card_number = get_long("Credit Card: ");
    // Calculate the sum of the digits using Luhn's algorithm
    int sum_every_other_digit = every_other_digit(card_number);
    int num_digits = number_of_digits(card_number);
    // Check if the credit card number is valid and belongs to any of the recognized card types
    bool amex = is_valid_amex(card_number, num_digits);
    bool master = is_valid_master_card(card_number, num_digits);
    bool visa = is_valid_visa(card_number, num_digits);

    if (sum_every_other_digit % 10 != 0)
    {
        printf("INVALID\n");
        return 0;
    }
    else if (amex)
    {
        printf("AMEX\n");
    }
    else if (master)
    {
        printf("MASTERCARD\n");
    }
    else if (visa)
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
        return 0;
    }
}

// Declare the function is_valid_visa
bool is_valid_visa(long card_number, int num_digits)
{
    if (num_digits == 13)
    {
        int first_digit = card_number / pow(10, 12);
        if (first_digit == 4)
        {
            return true;
        }
    }
    else if (num_digits == 16)
    {
        int first_digit = card_number / pow(10, 15);
        if (first_digit == 4)
        {
            return true;
        }
    }

    return false;
}

// Declare the function is_valid_amex
bool is_valid_amex(long card_number, int num_digits)
{
    int first_two_digits = card_number / pow(10, 13);
    if ((num_digits == 15) && (first_two_digits == 34 || first_two_digits == 37))
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Declare the function is_valid_master_card
bool is_valid_master_card(long card_number, int num_digits)
{
    int first_two_digits = card_number / pow(10, 14);
    if ((num_digits == 16) && (first_two_digits > 50 && first_two_digits < 56 ))
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Declare the function number_of_digits
int number_of_digits(long card_number)
{
    int count = 0;
    while (card_number > 0)
    {
        count = count + 1;
        card_number = card_number / 10;
    }

    return count;

}

// Declare the function every_other_digit
int every_other_digit(long card_number)
{
    int sum = 0;
    bool is_alternate_digit = false;
    while (card_number > 0)
    {
        if (is_alternate_digit)
        {
            int last_digit = card_number % 10;
            int product = multiply_and_sum(last_digit);
            sum = sum + product;
        }
        else
        {
            int last_digit = card_number % 10;
            sum += last_digit;
        }
        is_alternate_digit = !is_alternate_digit;
        card_number = card_number / 10;
    }


    return sum;
}

// Declare the function multiply_and_sum
int multiply_and_sum(int last_digit)
{
    int multiply = last_digit * 2;
    int sum = 0;
    while (multiply > 0)
    {
        int last_digit_multiply = multiply % 10;
        sum = sum + last_digit_multiply;
        multiply = multiply / 10;
    }
    return sum;
}