#include <stdio.h>
#include <cs50.h>


int main(void)
{
    //Store the user name 
    string name = get_string("What's your name? ");

    //Print hello with user name
    printf("hello, %s\n", name);
}