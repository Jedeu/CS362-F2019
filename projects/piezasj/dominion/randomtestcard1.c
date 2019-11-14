#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CHECK_MARK "\u2713"

int hasEstateCard(struct gameState* G, int thisPlayer)
{
    for (int j = 0; j < G->handCount[thisPlayer]; j++)
    {
        if (G->hand[thisPlayer][j] == estate)
        {
            return 1;
        }
    }

    return 0;
}

int main()
{   
    int seed = 1000;
    int numPlayers = 2;
	struct gameState G;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};
    srand(time(0)); 
    
    for (int i = 0; i < 1000000; i++)
    {
        int thisPlayer = rand() % 2;
        initializeGame(numPlayers, k, seed, &G);
        int discardEstate = rand() % 2;
        G.handCount[thisPlayer] = rand() % 5;
        for (int k = 0; k < G.handCount[thisPlayer]; k++)
        {
            G.hand[thisPlayer][k] = rand() % 26;
        }
        G.supplyCount[estate] = rand() % 5;

        int beginningPlayerCoins = G.coins;
        int beginningPlayerCardCount = G.handCount[thisPlayer];

        handleBaronEffect(discardEstate, &G, thisPlayer);

        if (discardEstate > 0)
        {
            if(G.coins != beginningPlayerCoins + 4)
            {
                printf("TEST_GAIN_2_COINS FAILED: Coin count is incorrect. Expected coin count: %d. Actual coin count: %d\n", beginningPlayerCoins + 4, G.coins);
            }
            else
            {
                printf("It should give the player 4 more coins if an estate card is discarded: %s\n", CHECK_MARK);
            }

            if (G.handCount[thisPlayer] != beginningPlayerCardCount - 1)
            {
                printf("TEST_GAIN_2_COINS FAILED: Number of cards after discarding Estate card incorrected. Expected count: %d. Actual coin count: %d\n", beginningPlayerCardCount - 1, G.handCount[thisPlayer]);
            }
            else
            {
                printf("It should remove a card from the player's hand after discarding the estate card: %s\n", CHECK_MARK);
            }

            if (hasEstateCard(&G, thisPlayer) == 0)
            {
                if (G.handCount[thisPlayer] != beginningPlayerCardCount + 1)
                {
                    printf("TEST_GAIN_ESTATE_BECAUSE_HAS_NO_ESTATE FAILED: Number of cards in the player's hand is incorrect. Expected %d cards. Actual card count: %d\n", beginningPlayerCardCount + 1, G.handCount[thisPlayer]);
                }
                else
                {
                    printf("It should give the player an estate card choose to discard one but don't have one in their hand: %s\n", CHECK_MARK);
                }   
                if (G.hand[thisPlayer][G.handCount[thisPlayer]] != estate)
                {
                    printf("TEST_GAIN_ESTATE_FAILED: Did not obtain an estate card\n");
                }
                else 
                {
                    printf("It should give the player an estate card if they choose not to discard one: %s\n", CHECK_MARK);
                }
            }
        }
        else
        {
            if (G.handCount[thisPlayer] != beginningPlayerCardCount + 1)
            {
                printf("TEST_GAIN_ESTATE_FAILED: Number of cards in the player's hand is incorrect. Expected %d cards. Actual card count: %d\n", beginningPlayerCardCount + 1, G.handCount[thisPlayer]);
            }
            else
            {
                printf("It should give the player an estate card if they choose not to discard one: %s\n", CHECK_MARK);
            }

            if (G.hand[thisPlayer][G.handCount[thisPlayer]] != estate)
            {
                printf("TEST_GAIN_ESTATE_FAILED: Did not obtain an estate card\n");
            }
            else 
            {
                printf("It should give the player an estate card if they choose not to discard one: %s\n", CHECK_MARK);
            }

            if (G.supplyCount[estate] == 0)
            {
                if (G.handCount[thisPlayer] != beginningPlayerCardCount)
                {
                    printf("TEST_NO_ESTATE_SUPPLY FAILED: Hand count should not have changed when estate supply count is 0\n");
                }
                else
                {
                    printf("It should not add an estate card when there are no estate cards in the supply: %s\n", CHECK_MARK);
                }
            }
        }
    }
}