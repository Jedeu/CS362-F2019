#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CHECK_MARK "\u2713"

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
        int otherPlayer = 1 - thisPlayer;
        int getTwoCoinsChoice = rand() % 2;
        int discardAndRedrawChoice = rand() % 2;
        initializeGame(numPlayers, k, seed, &G);

        int currentPlayerInitialCoins = G.coins;
        int currentPlayerInitialActions = G.numActions;
        int expectedDiscardCount = G.discardCount[thisPlayer] + G.handCount[thisPlayer];
        int expectedDiscardBothPlayers = G.discardCount[0] + G.discardCount[1] + G.handCount[0] + G.handCount[1];
        G.discardCount[0] = 0;
        G.discardCount[1] = 0;
        G.handCount[0] = rand() % 5 + 1;
        G.handCount[1] = rand() % 5 + 1;
        for (int j = 0; j < 2; j++)
        {
            for (int k = 0; k < G.handCount[j]; k++)
            {
                G.hand[j][k] = rand() % 26;
            }
        }

        int handPosition = rand() % G.handCount[thisPlayer];

        handleMinionEffect(&G, handPosition, thisPlayer, getTwoCoinsChoice, discardAndRedrawChoice);

        if (getTwoCoinsChoice > 0)
        {
            if(G.coins != currentPlayerInitialCoins + 2)
            {
                printf("TEST_GAIN_2_COINS FAILED: Coin count is incorrect. Expected coin count: %d. Actual coin count: %d\n", currentPlayerInitialCoins + 2, G.coins);
            }
            else
            {
                printf("It should give the player 2 more coins if the player makes that choice: %s\n", CHECK_MARK);
            }
            if(G.numActions != currentPlayerInitialActions + 1)
            {
                printf("TEST_ADD_NUMACTIONS FAILED: Expected action count: %d. Actual action count: %d\n", currentPlayerInitialActions + 1, G.numActions);
            }
            else
            {   
                printf("It increase the number of actions: %s\n", CHECK_MARK);
            }
        }
        else if (discardAndRedrawChoice > 0)
        {
            if (G.handCount[otherPlayer] < 4)
            {
                if (G.discardCount[thisPlayer] != expectedDiscardCount)
                {
                    printf("TEST_DISCARD_AND_REDRAW_SELF FAILED: Discard count is incorrect. Expected discard count: %d. Actual discard count: %d\n", expectedDiscardCount, G.discardCount[thisPlayer]);
                }
                else
                {   
                    printf("It only discards the player's hand if the other players have less than 4 cards: %s\n", CHECK_MARK);
                }
                
            }
            else
            {
                int numDiscardedCards = G.discardCount[0] + G.discardCount[1];
                if (numDiscardedCards != expectedDiscardBothPlayers)
                {
                    printf("TEST_DISCARD_AND_REDRAW_ALL FAILED: Discard count is incorrect. Expected discard count: %d. Actual discard count: %d\n", expectedDiscardBothPlayers, numDiscardedCards);
                }
                else
                {   
                    printf("It discards the current player and the other player's cards and then redraws for both players: %s\n", CHECK_MARK);
                }
            }
        }

    }
}