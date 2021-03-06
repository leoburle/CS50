#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // Checks if the candidate exist
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]] ++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count ++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // Initialize auxiliar variables
    pair winnerPair;
    int winnerPairId = 0;
    pair auxPair;
    int count = 0;
    // Selection Sorting the pairs array
    for (int k = 0; k < pair_count - 1; k++)
    {
        count = 0;
        for (int i = 0; i < pair_count; i++)
        {
            for (int j = 0; j < pair_count; j++) // Looks on the vector for the newst the 'winner' each turn, ignoring latest winner
            {
                if ((preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner]) >
                    (preferences[pairs[j].winner][pairs[j].loser] - preferences[pairs[j].loser][pairs[j].winner])) // comparing number of votes
                {
                    winnerPair = pairs[i];
                    winnerPairId = i;
                    count++;
                }
            }
        }
        // sorting
        if (count > 0)
        {
            auxPair = pairs[k];
            pairs[k] = winnerPair;
            pairs[winnerPairId] = auxPair;
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    int countLocks = 0;

    int winners[pair_count];
    int countWinners = 0;

    int losers[pair_count];
    int countLosers = 0;

    for (int i = 0; i < pair_count; i++)
    {
        locked[pairs[i].winner][pairs[i].loser] = true;

        winners[countWinners] = pairs[i].winner;
        countWinners++;

        losers[countLosers] = pairs[i].loser;
        countLosers++;

        for (int j = 0; j < countWinners; j++)
        {
            if (pairs[i].loser == winners[j])
            {
                for (int k = 0; k < countLosers; k++)
                {
                    if (pairs[i].winner == losers[k])
                    {
                        locked[pairs[i].winner][pairs[i].loser] = false;
                    }
                }
            }
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    int countWins = 0;
    int winner = 0;
    int count = 0;

    for (int i = 0; i < candidate_count; i++)
    {
        count = 0;
        countWins = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[i][j] == true)
            {
                countWins++;
                count++;
            }
            else if ((locked[i][j] == false) && (locked[j][i] == false))
            {
                count++;
            }
        }
        if ((countWins > 0) && (count == candidate_count))
        {
            winner = i;
        }
    }
    printf("%s\n", candidates[winner]);
    return;
}