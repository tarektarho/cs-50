#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{

    // Iterate through each candidate
    for (int i = 0; i < candidate_count; i++)
    {
        // Check if the candidate name is valid and matches one of the candidates
        if (strcmp(name, candidates[i].name) == 0)
        {
            for (int j = 0; j < candidate_count; j++)
            {
                // Store the index of the candidate (i) in the voter's preferences array at position 'rank'
                preferences[voter][rank] = i;
            }
            // Return true to indicate a valid vote
            return true;
        }
    }
    // Return false if the candidate name is not valid or doesn't match any candidate
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    // Iterate through each voter
    for (int i = 0; i < voter_count; i++)
    {
        // Iterate through each voter's preferences
        for (int j = 0; j < candidate_count; j++)
        {
            // Check if the candidate at the jth preference of the ith voter is not eliminated
            if (!candidates[preferences[i][j]].eliminated)
            {
                // Increment the votes count for the non-eliminated candidate
                candidates[preferences[i][j]].votes++;
                // Break out of the inner loop since the vote has been tabulated
                break;
            }
        }
    }

    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    // Calculate the majority required to win the election
    int majority = (voter_count / 2) + 1;

    // Iterate through each candidate
    for (int i = 0; i < candidate_count; i++)
    {
        // Check if the candidate's votes count is greater than or equal to the majority
        if (candidates[i].votes >= majority)
        {
            // Print the name of the winner and return true to indicate a winner exists
            printf("%s\n", candidates[i].name);
            return true;
        }
    }

    // Return false if no candidate has won the majority of votes
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    // Initialize minimum_vote with the maximum possible votes a candidate can receive
    int minimum_vote = MAX_VOTERS;

    // Iterate through each candidate
    for (int i = 0; i < candidate_count; i++)
    {
        // Check if the candidate is not eliminated and has fewer votes than the current minimum_vote
        if (!candidates[i].eliminated && candidates[i].votes < minimum_vote)
        {
            // Update the minimum_vote to the candidate's votes count
            minimum_vote = candidates[i].votes;
        }
    }

    // Return the minimum_vote as the minimum number of votes any remaining candidate has
    return minimum_vote;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    // Initialize variables to keep track of remaining candidates and candidates with minimum votes
    int remaning_candidates = 0;
    int candidates_with_min_votes = 0;

    // Iterate through each candidate
    for (int i = 0; i < candidate_count; i++)
    {
        // Check if the candidate is not eliminated
        if (!candidates[i].eliminated)
        {
            // Increment the count of remaining candidates
            remaning_candidates++;

            // Check if the candidate's votes count is equal to or less than the minimum number of votes (min)
            if (candidates[i].votes <= min)
            {
                // Increment the count of candidates with minimum votes
                candidates_with_min_votes++;
            }
        }
    }

    // Check if all remaining candidates have the same number of votes (tied)
    if (remaning_candidates == candidates_with_min_votes)
    {
        // Return true to indicate a tie between all candidates
        return true;
    }

    // Return false if there is no tie between all candidates
    return false;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    // Iterate through each candidate
    for (int i = 0; i < candidate_count; i++)
    {
        // Check if the candidate is not eliminated and has votes equal to or less than the minimum number of votes (min)
        if (!candidates[i].eliminated && (candidates[i].votes <= min))
        {
            // Eliminate the candidate by setting the eliminated status to true
            candidates[i].eliminated = true;
        }
    }
    return;
}