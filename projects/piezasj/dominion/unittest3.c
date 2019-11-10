#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define CHECK_MARK "\u2713"

static const struct gameState EmptyStruct;

int main()
{
    int numPassedTests = 0;
    int totalTests = 6;
    int seed = 1000;
    int numPlayers = 2;
    int thisPlayer = 0;
	struct gameState G;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

    /* Test that other players gain an adventurer card 
       if the current player gives theirs up */
    initializeGame(numPlayers, k, seed, &G);

    G.hand[thisPlayer][0] = adventurer;
    G.hand[thisPlayer][1] = ambassador;
    G.hand[thisPlayer][2] = adventurer;

    int adventurerPosition = 0;
    int ambassadorPosition = 1;

    int expectedCardCountOtherPlayer = G.handCount[1] + 1;

    int expectedDiscardCount = G.discardCount[thisPlayer] + 1;

    int expectedHandCount = G.handCount[thisPlayer] - 3;

    handleAmbassadorEffect(adventurerPosition, 1, ambassadorPosition, thisPlayer, &G);
    int cardCountOtherPlayer = G.handCount[1];

    if(cardCountOtherPlayer != expectedCardCountOtherPlayer)
    {
        printf("FAIL: Other player card count is incorrect. Expected card count: %d. Actual card count: %d\n", expectedCardCountOtherPlayer, cardCountOtherPlayer);
    }
    else
    {
        printf("It should give the other player an adventurer card if the current player gives it up with the ambassador card: %s\n", CHECK_MARK);
        numPassedTests++;
    }

    int discardCount = G.discardCount[thisPlayer];

    if(discardCount != expectedDiscardCount)
    {
        printf("FAIL: Current player discard count is incorrect. Expected discard count: %d. Actual discard count: %d\n", expectedDiscardCount, discardCount);
    }
    else
    {
        printf("It should discard the ambassador card after being played: %s\n", CHECK_MARK);
        numPassedTests++;
    }

    int handCount = G.handCount[thisPlayer];

    if(handCount != expectedHandCount)
    {
        printf("FAIL: Current player hand count is incorrect. Expected hand count: %d. Actual hand count: %d\n", expectedHandCount, handCount);
    }
    else
    {
        printf("It should remove all cards that were used in play: %s\n", CHECK_MARK);
        numPassedTests++;
    }

    G = EmptyStruct;

    initializeGame(numPlayers, k, seed, &G);

    G.hand[thisPlayer][0] = adventurer;
    G.hand[thisPlayer][1] = ambassador;
    G.hand[thisPlayer][2] = adventurer;

    adventurerPosition = 0;
    ambassadorPosition = 1;

    int result = handleAmbassadorEffect(adventurerPosition, -1, ambassadorPosition, thisPlayer, &G);

    if(result == 0)
    {
        printf("FAIL: Unexpected successful return from function");
    }
    else
    {
        printf("It should fail if the number to discard is not between 1 and 2: %s\n", CHECK_MARK);
        numPassedTests++;
    }

    G = EmptyStruct;

    initializeGame(numPlayers, k, seed, &G);

    G.hand[thisPlayer][0] = adventurer;
    G.hand[thisPlayer][1] = ambassador;
    adventurerPosition = 0;
    ambassadorPosition = 1;

    result = handleAmbassadorEffect(adventurerPosition, 1, ambassadorPosition, thisPlayer, &G);

    if(result == 0)
    {
        printf("FAIL: Unexpected successful return from function");
    }
    else
    {
        printf("It should fail if the player does not have copies of the chosen card: %s\n", CHECK_MARK);
        numPassedTests++;
    }

    G = EmptyStruct;

    initializeGame(numPlayers, k, seed, &G);

    G.hand[thisPlayer][0] = adventurer;
    G.hand[thisPlayer][1] = ambassador;

    result = handleAmbassadorEffect(adventurerPosition, 0, ambassadorPosition, thisPlayer, &G);

    if(result == 0)
    {
        printf("FAIL: Unexpected successful return from function\n");
    }
    else
    {
        printf("It should fail if the player chooses to discard 0 copies: %s\n", CHECK_MARK);
        numPassedTests++;
    }

    G = EmptyStruct;

    initializeGame(numPlayers, k, seed, &G);

    G.hand[thisPlayer][1] = ambassador;
    ambassadorPosition = 1;

    result = handleAmbassadorEffect(ambassadorPosition, 0, ambassadorPosition, thisPlayer, &G);

    if(result == 0)
    {
        printf("FAIL: Unexpected successful return from function\n");
    }
    else
    {
        printf("It should fail if the player chooses to use the same ambassador card to discard that they played: %s\n", CHECK_MARK);
        numPassedTests++;
    }

    printf("%d out of %d tests passed\n", numPassedTests, totalTests);
}