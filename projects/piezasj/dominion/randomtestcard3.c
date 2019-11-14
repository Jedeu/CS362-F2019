#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CHECK_MARK "\u2713"

int hasCard(int cardToSearch, int tributeSupply[])
{
    for (int i = 0; i < 2; i++)
    {
        if (cardToSearch == tributeSupply[i])
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
        int otherPlayer = 1 - thisPlayer;
        int tributeRevealedCards[2] = {-1, -1};
        initializeGame(numPlayers, k, seed, &G);

        G.handCount[0] = rand() % 5 + 1;
        G.handCount[1] = rand() % 5 + 1;
        for (int j = 0; j < 2; j++)
        {
            for (int k = 0; k < G.handCount[j]; k++)
            {
                G.hand[j][k] = rand() % 25 + 1;
            }
        }

        G.discardCount[otherPlayer] = rand() % MAX_DECK;
        G.deckCount[otherPlayer] = rand() % MAX_DECK;

        int previousCoins = G.coins;
        int previousCardCount = G.handCount[otherPlayer];

        handleTributeEffect(&G, tributeRevealedCards, thisPlayer, otherPlayer);

        if (hasCard(copper, tributeRevealedCards) || hasCard(silver, tributeRevealedCards) || hasCard(gold, tributeRevealedCards))
        {
            if(G.coins != previousCoins + 2)
            {
                printf("TEST_GAIN_2_COINS FAILED: Coin count is incorrect. Expected coin count: %d. Actual coin count: %d\n", previousCoins + 2, G.coins);
            }
            else
            {
                printf("It should give the other player 2 more coins if their tribute includes a silver card: %s\n", CHECK_MARK);
            }
        }

        if (hasCard(estate, tributeRevealedCards) || hasCard(duchy, tributeRevealedCards) || hasCard(province, tributeRevealedCards))
        {
            if (G.handCount[otherPlayer] != previousCardCount + 2)
            {
                printf("TEST_TRIBUTE_EFFECT_VICTORY_CARD FAILED: hand count is incorrect. Expected hand count: %d. Actual hand count: %d\n", previousCardCount + 2, G.handCount[otherPlayer]);
            }
            else
            {
                printf("It should add two cards to the other player's hand if a Victory Card is added to the tribute pile: %s\n", CHECK_MARK);
            }
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
        }
    }
}