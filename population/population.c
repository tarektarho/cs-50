#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //start size
    int start;
    //end size
    int end;


    do
    {
        //Prompt the use for start size
        start = get_int("Start size: ");
    }
    while(start < 9);

    do
    {
        //Prompt the user for end size
        end = get_int("End size: ");
    }
    while(end < start);

    int years = 0;


    while (start < end)
    {
        start = start + (start / 3) - (start / 4);
        years++;
    }

    printf("Years: %d\n", years);
}
