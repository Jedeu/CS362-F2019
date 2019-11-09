#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define CHECK_MARK "\u2713"
#define TRUE 1
#define FALSE 0

static const struct gameState EmptyStruct;

int main()
{
    int numPassedTests = 0;
    int totalTests = 2;
    int seed = 1000;
    int numPlayers = 2;
    int thisPlayer = 0;
	struct gameState G;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

    // Test that a player gains 2 coins from playing the minion card
    initializeGame(numPlayers, k, seed, &G);
    int currentPlayerCoins = G.coins;
    G.hand[thisPlayer][2] = minion;

    handleMinionEffect(&G, G.hand[thisPlayer][2], thisPlayer, TRUE, FALSE);

    if(G.coins != currentPlayerCoins + 2)
    {
        printf("FAIL: Coin count is incorrect. Expected coin count: %d. Actual coin count: %d\n", currentPlayerCoins + 2, G.coins);
    }
    else
    {
        printf("It should give the player 2 more coins if the player makes that choice: %s\n", CHECK_MARK);
        numPassedTests++;
    }

    G = EmptyStruct;

    // Test that numActions increase when playing this card
    initializeGame(numPlayers, k, seed, &G);
    int numActionsBefore = G.numActions;
    G.hand[thisPlayer][2] = minion;

    handleMinionEffect(&G, G.hand[thisPlayer][2], thisPlayer, TRUE, FALSE);

    if(G.numActions != numActionsBefore + 1)
    {
        printf("FAIL: Action count is incorrect. Expected action count: %d. Actual action count: %d\n", numActionsBefore + 1, G.numActions);
    }
    else
    {   
        printf("It increase the number of actions: %s\n", CHECK_MARK);
        numPassedTests++;
    }

    G = EmptyStruct;

    /* Test the discardAndRedraw option logic */

    initializeGame(numPlayers, k, seed, &G);

    /* Normally I would simply call discardCard enough times here to ensure a handCount of 2
       but there's a high chance that method is buggy so I'm manually adjusting the game state */
    G.handCount[1] = 2;

    int expectedDiscardCount = G.discardCount + G.handCount[0];

    handleMinionEffect(&G, G.hand[thisPlayer][0], thisPlayer, FALSE, TRUE);

    int numDiscardedCards = G.discardCount;
    
    if (numDiscardedCards != expectedDiscardCount)
    {
        printf("FAIL: Discard count is incorrect. Expected discard count: %d. Actual discard count: %d\n", expectedDiscardCount, numDiscardedCards);
    }
    else
    {   
        printf("It only discards the player's hand if the other players have less than 4 cards: %s\n", CHECK_MARK);
        numPassedTests++;
    }

    printf("%d out of %d tests passed\n", numPassedTests, totalTests);
}