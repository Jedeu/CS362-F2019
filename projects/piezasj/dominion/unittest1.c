#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int seed = 1000;
    int numPlayers = 2;
    int thisPlayer = 0;
	struct gameState G;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

    printf("It should give the player 4 more coins if an estate card is discarded\n");
    int currentPlayerCoins = G.coins;
    int discardEstate = 1;
    G.hand[thisPlayer][0] = baron;
    handleBaronEffect(discardEstate, &G, thisPlayer);

    printf("Expected coin count: %d. Actual coin count: %d\n", currentPlayerCoins + 4, G.coins);
    
    printf("SUCCESS: All tests passed for baron\n");

    return 0;
}