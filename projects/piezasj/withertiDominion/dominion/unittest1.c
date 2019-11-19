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
    int totalTests = 7;
    int seed = 1000;
    int numPlayers = 2;
    int thisPlayer = 0;
	struct gameState G;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

    printf("######################################## BEGIN TEST: BARON ########################################\n");

	// Test that the player gains 4 coins if they choose to discard an estate
	initializeGame(numPlayers, k, seed, &G);
    int currentPlayerCoins = G.coins;
    int discardEstate = 1;
    int currentPlayerCardCount = G.handCount[thisPlayer];
    int expectedCardCount = G.handCount[thisPlayer] - 1;
    G.hand[thisPlayer][0] = estate;
    cardBaron(&G, thisPlayer, discardEstate);

    if(G.coins != currentPlayerCoins + 4)
    {
        printf("TEST_GAIN_2_COINS FAILED: Coin count is incorrect. Expected coin count: %d. Actual coin count: %d\n", currentPlayerCoins + 4, G.coins);
    }
    else
    {
        printf("It should give the player 4 more coins if an estate card is discarded: %s\n", CHECK_MARK);
        numPassedTests++;
    }

    if (currentPlayerCoins != expectedCardCount)
    {
        printf("TEST_GAIN_2_COINS FAILED: Number of cards after discarding Estate card incorrected. Expected count: %d. Actual coin count: %d\n", expectedCardCount, currentPlayerCardCount);
    }
    else
    {
        printf("It should remove a card from the player's hand after discarding the estate card: %s\n", CHECK_MARK);
        numPassedTests++;
    }

    G = EmptyStruct;

    // Test that the player gains an estate card if they choose not to discard an estate
    initializeGame(numPlayers, k, seed, &G);
    discardEstate = 0;

    int thisPlayerNumCards = G.handCount[thisPlayer];
    expectedCardCount = thisPlayerNumCards + 1;

    cardBaron(&G, thisPlayer, discardEstate);

    if(G.handCount[thisPlayer] != expectedCardCount)
    {
        printf("TEST_GAIN_ESTATE_FAILED: Number of cards in the player's hand is incorrect. Expected %d cards. Actual card count: %d\n", expectedCardCount, G.handCount[thisPlayer]);
    }
    else
    {
        printf("It should give the player an estate card if they choose not to discard one: %s\n", CHECK_MARK);
        numPassedTests++;
    }

    int latestCardInHand = G.hand[thisPlayer][G.handCount[thisPlayer]];
    
    if (latestCardInHand != estate)
    {
        printf("TEST_GAIN_ESTATE_FAILED: Did not obtain an estate card\n");
    }
    else 
    {
        printf("It should give the player an estate card if they choose not to discard one: %s\n", CHECK_MARK);
        numPassedTests++;
    }

    G = EmptyStruct;

    // Test that the game gives the player an estate card if they make an erroneous choice (INSERTED BUG)
    initializeGame(numPlayers, k, seed, &G);
    discardEstate = 1;
    thisPlayerNumCards = G.handCount[thisPlayer];
    expectedCardCount = thisPlayerNumCards + 1;

    /* On the off-chance that the player drew an estate card
       during intializeGame, substitute it with village card for this test */
    for (int j = 0; j < thisPlayerNumCards; j++)
    {
        if (G.hand[thisPlayer][j] == estate)
        {
            G.hand[thisPlayer][j] = village;
        }
    }

    cardBaron(&G, thisPlayer, discardEstate);

    if (thisPlayerNumCards != expectedCardCount)
    {
        printf("TEST_GAIN_ESTATE_BECAUSE_HAS_NO_ESTATE FAILED: Number of cards in the player's hand is incorrect. Expected %d cards. Actual card count: %d\n", expectedCardCount, G.handCount[thisPlayer]);
    }
    else
    {
        printf("It should give the player an estate card choose to discard one but don't have one in their hand: %s\n", CHECK_MARK);
        numPassedTests++;
    }

    if (G.hand[thisPlayer][thisPlayerNumCards] != estate)
    {
        printf("TEST_GAIN_ESTATE_BECAUSE_HAS_NO_ESTATE FAILED: Did not acquire an estate card\n");
    }
    else
    {
        printf("It should give the player an estate card choose to discard one but don't have one in their hand: %s\n", CHECK_MARK);
        numPassedTests++;
    }

    G = EmptyStruct;

    // Test that the game does not give the player an estate card if there are none left in the supply (INSERTED BUG)
    initializeGame(numPlayers, k, seed, &G);
    G.supplyCount[estate] = 0;
    discardEstate = 0;

    int expectedHandCount = G.handCount[thisPlayer];

    cardBaron(&G, thisPlayer, discardEstate);

    if (G.handCount[thisPlayer] != expectedHandCount)
    {
        printf("TEST_NO_ESTATE_SUPPLY FAILED: Hand count should not have changed when estate supply count is 0\n");
    }
    else
    {
        printf("It should not add an estate card when there are no estate cards in the supply: %s\n", CHECK_MARK);
        numPassedTests++;
    }
    

    printf("%d out of %d tests passed for cardBaron\n", numPassedTests, totalTests);

    printf("########################################## END TEST: BARON ########################################\n\n");

    return 0;
}