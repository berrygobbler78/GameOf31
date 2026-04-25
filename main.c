#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "card_utils.h"
#ifdef _WIN32
#include <windows.h>
#endif

int end_game = -1;

// win conditions
#define NO_WIN (-1)
#define HAS_14 0
#define HAS_31 1
#define OVER_31 2

#define DRAWN (-1)

void draw(card *deck, card **hand, int *hand_len, int is_dealer) {
    if (*hand_len >= 52) {
        printf("No more cards in the deck.\n");
        return;
    }
    card *temp_hand = realloc(*hand, sizeof(card) * (*hand_len + 1));
    *hand = temp_hand;

    int val;
    card temp;

    do {
        val = rand() % 52;
        temp = deck[val];
    } while (temp.value == DRAWN);

    if (strcmp(temp.face, ACE) == 0) {
        // TODO: Add better interface for picking Ace val
        if (is_dealer) {
            int current = hand_value(*hand, *hand_len);
            temp.value = (current + 11 <= 31) ? 11 : 1;
        }
        else {
            int input;
            do {
                //REDO PLAYER DOES NOT GET TO CHOOSE EVEN ACE, ONLY THE FIRST ONE

                fast_printf("Enter value for ACE (1 or 11)\n");
                scanf("%d", &input);
            } while (input != 1 && input != 11);

            temp.value = input;
        }

    }

    deck[val].value = DRAWN;
    (*hand)[*hand_len] = temp;
    *hand_len += 1;
}

int dealer_turn(card *deck, card **hand, int *hand_len) {
    fast_printf("Dealer's turn... | ");

    while (1) {
        draw(deck, hand, hand_len,1);
        int result = hand_value(*hand,*hand_len);

        if (result > 31) {
            fast_printf("Dealer busts! All players win!\n");
            print_cards(*hand, *hand_len, -1);
            return OVER_31;
        }
        if (result == 31) {
            fast_printf("Dealer hits 31!\n");
            print_cards(*hand, *hand_len, -1);
            return HAS_31;
        }
        if (result == 14) {
            fast_printf("Dealer hits 14, must stop.\n");
            print_cards(*hand, *hand_len, -1);  // Reveal all
            return HAS_14;
        }

        // Dealer's strategy: stop at 26+
        if (hand_value(*hand, *hand_len) >= 26) {
            fast_printf("Dealer stands.\n");
            print_cards(*hand, *hand_len, 0);
            return NO_WIN;
        }
    }
}

int check_hand(const card *hand, int hand_len) {
    int total = 0;
    for (int i = 0; i < hand_len; i++) total += hand[i].value;

    if (total == 31) return HAS_31;
    if (total == 14) return HAS_14;
    if (total > 31) return OVER_31;

    return NO_WIN;
}

void run(int *total_money, card *players[], int player_count) {
    // deck setup
    card deck[52];
    init(deck);

    int player_len[player_count];
    int wagers[player_count];
    players[0] = NULL;
    player_len[0] = 0;
    draw(deck, &players[0], &player_len[0],1);
    print_cards(players[0], player_len[0], 0);

    for (int i = 1; i < player_count; i++) {
        players[i] = NULL;
        player_len[i] = 0;
        draw(deck, &players[i], &player_len[i],0);

    }


    for (int i = 1; i < player_count; i++) {
        print_cards(players[i], player_len[i], i);
        do {
            if (total_money[i] == 0) {
                fast_printf("Looks like you're broke! Here's 1 dollar to keep going!\n");
                total_money[i] += 1;
            }
            fast_printf("How much to wager? Total money:");
            printf( "%d\n", total_money[i]);
            scanf("%d", &wagers[i]);
        } while (wagers[i] > total_money[i]);
        total_money[i] -= wagers[i];
    }
    int win = NO_WIN;
    if (dealer_turn(deck,&players[0],&player_len[0]) == OVER_31) win = 1;
    int playerBreak;
    for (int i = 1; i < player_count; i++){
        if (win != -1) break;
        printf("--------------------\n");
        fast_printf("Player ");
        printf("%d's",i);
        fast_printf(" turn\n");
        while (1) {
            draw(deck, &players[i], &player_len[i],0);

            playerBreak = check_hand(players[i], player_len[i]);

            print_cards(players[i], player_len[i], i);
            fast_printf("Total Value: ");
            printf("%d\n", hand_value(players[i], player_len[i]));

            if (playerBreak != -1) break;

            int temp;
            fast_printf("Continue? (1 for yes, 0 for no)\n");
            if (scanf("%d", &temp) == 0 || temp == 0) break;
        }
        char buffer[50];
        switch (playerBreak) {
            case OVER_31:
                snprintf(buffer,sizeof(buffer),"Player %d busts",i);
                fast_printf(buffer);
                break;
            case HAS_14:
                snprintf(buffer,sizeof(buffer),"Player %d hit 14",i);
                fast_printf(buffer);
                break;
            case HAS_31:
                snprintf(buffer,sizeof(buffer),"Player %d hit 31",i);
                fast_printf(buffer);
                break;
            default: ;
        }
        fast_printf("\nPlayer ");
        printf("%d's",i);
        fast_printf(" turn is over\n");
    }
    compare_cards(players, player_len, total_money, wagers, player_count,win);
    for (int i = 0; i < player_count; i++) free(players[i]);
}
////BUG FIXES: STILL ASKS FOR DEALER'S FIRST ACE VALUE WHEN IT SHOULDN'T, THERE'S SOME BUGS WITH ACES WITH HIGH NUMBERS OF PLAYERS
int main(void) {
    #ifdef _WIN32
    SetConsoleOutputCP(65001);  // UTF-8
    SetConsoleCP(65001); //AI-Generated, icons work on Linux but not windows
    #endif

    srand(time(0));

    // player setup
    int player_count = 0;
    do {
        fast_printf("Enter number of players:\n");
        scanf("%d", &player_count);
    } while (player_count < 1);

    player_count++;

    card *players[player_count];

    int total_money[player_count];
    for (int i = 0; i < player_count; i++) {
        total_money[i] = 100;
    }

    char play = 'y';
    while (play == 'y') {
        run(total_money, players, player_count);
        do {
            fast_printf("Would you like to play again? (y/n) ");
            scanf(" %c", &play);
        } while (play != 'y' && play != 'n');
    }
    printf("Goodbye...");

    return 0;
}



