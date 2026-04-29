#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "card_utils.h"
#ifdef _WIN32
#include <windows.h>
#endif

// win conditions
#define NO_WIN (-1)
#define HAS_14 0
#define HAS_31 1
#define OVER_31 2

// colors
#define RED     "\033[31m"
#define BLACK   "\033[90m"
#define GREEN  "\033[32m"
#define GOLD   "\033[33m"
#define RESET   "\033[0m"


#define DRAWN (-1)

void draw(card *deck, card **hand, int *hand_len, int playerNum, int *ace_count,int *ace_last_val) {
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
        if (playerNum == 0) {
            int current = hand_value(*hand, *hand_len);
            temp.value = (current + 11 <= 31) ? 11 : 1;
        }
        else if (*ace_count % 2 == 0) {
            int input;
            char buffer[100];
            do {
                snprintf(buffer,sizeof(buffer),GOLD "Player %d pulled an ACE! Enter value for ACE (1 or 11): " RESET,playerNum);
                fast_printf(buffer);
                scanf("%d", &input);
            } while (input != 1 && input != 11);
            temp.value = input;
            *ace_last_val = input;
            (*ace_count)++;
        } else {
            temp.value = (*ace_last_val == 11) ? 1 : 11;
            (*ace_count)++;
        }

    }

    deck[val].value = DRAWN;
    (*hand)[*hand_len] = temp;
    *hand_len += 1;
}

int dealer_turn(card *deck, card **hand, int *hand_len) {
    fast_printf("Dealer's turn... | ");
    while (1) {
        draw(deck, hand, hand_len,0,0,0);
        int result = hand_value(*hand,*hand_len);

        if (result > 31) {
            fast_printf(GOLD "Dealer busts! All players win!\n" RESET);
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

int check_hand(const card *hand, const int hand_len) {
    const int total = hand_value(hand, hand_len);

    if (total == 31) return HAS_31;
    if (total == 14) return HAS_14;
    if (total > 31) return OVER_31;
    return NO_WIN;
}

void run(int *total_money, card *players[], int player_count) {
    // deck setup
    card deck[52];
    init(deck);

    int player_len[player_count], wagers[player_count], ace_count[player_count], ace_last_val[player_count];
    players[0] = NULL;
    player_len[0] = 0;

    for (int i = 0; i < player_count; i++) {
        ace_count[i] = 0;
        ace_last_val[i] = 0;
    }

    draw(deck, &players[0], &player_len[0],0,0,0);
    print_cards(players[0], player_len[0], 0);

    for (int i = 1; i < player_count; i++) {
        players[i] = NULL;
        player_len[i] = 0;
        draw(deck, &players[i], &player_len[i],i,&ace_count[i],&ace_last_val[i]);
        print_cards(players[i], player_len[i], i);
        do {
            if (total_money[i] == 0) {
                fast_printf("Looks like you're broke! Here's 1 dollar to keep going!\n");
                total_money[i] += 1;
            }
            fast_printf("How much to wager? Total money:");
            printf( "%d\n", total_money[i]);
            if (scanf("%d", &wagers[i]) == 0) {
                while (getchar() != '\n');
                    wagers[i] = -1;
            }
        } while ((wagers[i] > total_money[i]) || wagers[i] < 0 );
        total_money[i] -= wagers[i];
    }
    int win = NO_WIN;
    if (dealer_turn(deck,&players[0],&player_len[0]) == OVER_31) win = 1;
    int playerBreak;
    char buffer[50];
    for (int i = 1; i < player_count; i++){
        if (win != -1) break;
        printf("--------------------\n");
        snprintf(buffer,sizeof(buffer), BLACK "\nPlayer %d's turn!\n" RESET,i);
        fast_printf(buffer);
        while (1) {
            draw(deck, &players[i], &player_len[i],i,&ace_count[i],&ace_last_val[i]);

            playerBreak = check_hand(players[i], player_len[i]);

            print_cards(players[i], player_len[i], i);
            fast_printf( GREEN "Total Value: " RESET);
            printf(GREEN "%d\n" RESET, hand_value(players[i], player_len[i]));

            if (playerBreak != -1) break;

            int temp;
            fast_printf("Continue? (1 for yes, 0 for no)\n");
            if (scanf("%d", &temp) == 0 || temp == 0) break;
        }

        switch (playerBreak) {
            case OVER_31:
                snprintf(buffer,sizeof(buffer),RED "Player %d busts" RESET,i);
                fast_printf(buffer);
                break;
            case HAS_14:
                snprintf(buffer,sizeof(buffer),GOLD "Player %d hit 14" RESET,i);
                fast_printf(buffer);
                break;
            case HAS_31:
                snprintf(buffer,sizeof(buffer),GOLD "Player %d hit 31" RESET,i);
                fast_printf(buffer);
                break;
            default: ;
        }
        snprintf(buffer,sizeof(buffer), BLACK "\nPlayer %d's turn is over!\n" RESET,i);
        fast_printf(buffer);
    }
    compare_cards(players, player_len, total_money, wagers, player_count,win);
    for (int i = 0; i < player_count; i++) free(players[i]);
}

int main(void) {
    #ifdef _WIN32
    SetConsoleOutputCP(65001);  // UTF-8
    SetConsoleCP(65001); //AI-Generated, icons work on Linux but not windows
    #endif

    srand(time(0));

    // player setup
    int player_count = 0;
    fast_printf("Welcome to " BLACK "GAME " RED "OF " BLACK "3" RED "1\n" RESET);
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
    fast_printf("Goodbye...");
    delay(1000);

    return 0;
}



