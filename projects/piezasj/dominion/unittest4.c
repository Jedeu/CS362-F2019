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
    int nextPlayer = 1;
	struct gameState G;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};
    int tributeRevealedCards[2] = {-1, -1};

    printf("######################################## BEGIN TEST: TRIBUTE ########################################\n");

    initializeGame(numPlayers, k, seed, &G);

    G.deckCount[nextPlayer] = 1;
    G.deck[nextPlayer][0] = silver;

    int expectedCoins = G.coins + 2;

    handleTributeEffect(&G, tributeRevealedCards, thisPlayer, nextPlayer);

    if(G.coins != expectedCoins)
    {
        printf("TEST_GAIN_2_COINS FAILED: Coin count is incorrect. Expected coin count: %d. Actual coin count: %d\n", expectedCoins, G.coins);
    }
    else
    {
        printf("It should give the other player 2 more coins if their tribute includes a silver card: %s\n", CHECK_MARK);
        numPassedTests++;
    }

    G = EmptyStruct;
    tributeRevealedCards[0] = -1;
    tributeRevealedCards[1] = -1;

    initializeGame(numPlayers, k, seed, &G);
    G.deckCount[nextPlayer] = 0;
    G.discardCount[nextPlayer] = 1;
    G.discard[nextPlayer][0] = duchy;

    int expectedCardCount = G.deckCount[nextPlayer] + 2;
    
    handleTributeEffect(&G, tributeRevealedCards, thisPlayer, nextPlayer);

    if (G.handCount[nextPlayer] != expectedCardCount)
    {
        printf("TEST_TRIBUTE_EFFECT_VICTORY_CARD FAILED: hand count is incorrect. Expected hand count: %d. Actual hand count: %d\n", expectedCardCount, G.handCount[nextPlayer]);
    }
    else
    {
        printf("It should add two cards to the other player's hand if a Victory Card is added to the tribute pile: %s\n", CHECK_MARK);
        numPassedTests++;
    }

    if (tributeRevealedCards[0] != duchy)
    {
        printf("TEST_TRIBUTE_EFFECT_VICTORY_CARD FAILED: card found in Tribute is incorrect. Expected card found in Tribute: %d. Actual card found in Tribute: %d\n", duchy, tributeRevealedCards[0]);
    }
    else
    {
        printf("It should add a card from the other player's discard or deck to the tribute pile: %s\n", CHECK_MARK);
        numPassedTests++;
    }

    G = EmptyStruct;
    tributeRevealedCards[0] = -1;
    tributeRevealedCards[1] = -1;

    initializeGame(numPlayers, k, seed, &G);
    G.deckCount[nextPlayer] = 2;
    G.deck[nextPlayer][0] = gold;
    G.deck[nextPlayer][1] = tribute;

    int expectedActions = G.numActions + 2;
    expectedCoins = G.coins + 2;    

    handleTributeEffect(&G, tributeRevealedCards, thisPlayer, nextPlayer);

    if (G.numActions != expectedActions)
    {
        printf("TEST_ACTION_CARD_EFFECT FAILED: Action count is incorrect. Expected action count: %d. Actual action count: %d\n", expectedCardCount, G.numActions);
    }
    else
    {
        printf("It should add 2 actions to the other player if an action card is sent to the tribute: %s\n", CHECK_MARK);
        numPassedTests++;
    }

    if (G.coins != expectedCoins)
    {
        printf("TEST_TREASURE_CARD_EFFECT FAILED: Coin count is incorrect. Expected coin count: %d. Actual coin count: %d\n", expectedCoins, G.coins);
    }
    else
    {
        printf("It should add 2 coins to the other player if an treasure card is sent to the tribute: %s\n", CHECK_MARK);
        numPassedTests++;
    }

    int tributeIsFull = 1;

    for (int k = 0; k < 2; k++)
    {
        if (tributeRevealedCards[k] == -1)
        {
            printf("TEST_CHECK_TRIBUTE_PILE_COUNT FAILED: Tribute pile does not have 2 cards in it\n");
            tributeIsFull = 0;
            break;
        }
    }

    if (tributeIsFull > 0)
    {
        printf("It should send two cards to the Tribute pile: %s\n", CHECK_MARK);
        numPassedTests++;
    }

    if (tributeRevealedCards[0] != gold || tributeRevealedCards[1] != tribute)
    {
        printf("TEST CHECK_TRIBUTE_PILE_CARDS FAILED: card found in Tribute is incorrect. Expected cards found in Tribute: %d, %d.\nActual card found in Tribute:\n", gold, tribute);
        for (int m = 0; m < 2; m++)
        {
            printf("%d\n", tributeRevealedCards[m]);
        }
    }
    else
    {
        printf("It should send a gold card and a tribute card to the tribute pile if those cards were drawn: %s\n", CHECK_MARK);
    }

    printf("%d out of %d tests passed\n", numPassedTests, totalTests);

    printf("########################################## END TEST: TRIBUTE ########################################\n\n");

}