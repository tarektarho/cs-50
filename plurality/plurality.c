#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    // Iterate through the list of candidates to find a match with the given name
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i].name, name) == 0)
        {
            // Increment the vote count for the matched candidate
            candidates[i].votes++;
            return true;

        }
    }
    // If no match is found, return false indicating an invalid vote
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{

    // Initialize a variable to store the highest vote count
    int highest_vote = 0;

    // Iterate through the array to find the highest vote count
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > highest_vote)
        {
            highest_vote = candidates[i].votes;
        }
    }

    // Iterate through the array to print the name of candidates with the highest vote count
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == highest_vote)
        {
            printf("%s\n", candidates[i].name);
        }
    }

    return;
}