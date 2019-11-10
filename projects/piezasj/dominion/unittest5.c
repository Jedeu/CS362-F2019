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
    int totalTests = 2;
    int seed = 1000;
    int numPlayers = 2;
    int thisPlayer = 0;
	struct gameState G;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

    printf("######################################## BEGIN TEST: BARON ########################################\n");

    initializeGame(numPlayers, k, seed, &G);

    G.hand[thisPlayer][0] = copper;
    G.hand[thisPlayer][1] = mine;

    int expectedHandCount = G.handCount[thisPlayer] - 1;
    
    handleMineEffect(&G, thisPlayer, 0, gold, 1);

    if (G.handCount[thisPlayer] != expectedHandCount)
    {
        printf("Testing TRADE_COPPER_FOR_GOLD FAILED: Expected card count: %d. Actual card count: %d\n", expectedHandCount, G.handCount[thisPlayer]);
    }
    else
    {
        printf("It should remove the mine card after being played: %s\n", CHECK_MARK);
        numPassedTests++;
    }

    if (G.hand[thisPlayer][G.handCount[thisPlayer - 1]] != gold)
    {
        printf("Testing TRADE_COPPER_FOR_GOLD FAILED: Did not receive gold card. Current card in last position: %d\n", G.hand[thisPlayer][G.handCount[thisPlayer - 1]]);
    }
    else
    {
        printf("It should trade a copper card for a gold card: %s\n", CHECK_MARK);
        numPassedTests++;
    }

    printf("%d out of %d tests passed\n", numPassedTests, totalTests);

    printf("######################################## END TEST: BARON ##########################################\n");
}