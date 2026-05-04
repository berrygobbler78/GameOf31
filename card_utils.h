//
// Created by wgaris on 4/20/26.
//
#pragma once
#include <ctype.h>

#ifndef GAMEOF31_CARD_UTILS_H
#define GAMEOF31_CARD_UTILS_H

// colors
#define RED     "\033[31m"
#define BLACK   "\033[90m"
#define GOLD   "\033[33m"
#define RESET   "\033[0m"

// default card vals
const char ACE[] = "ace", KING[] = "king", QUEEN[] = "queen", JACK[] = "jack", NONE[] = "none";
const char HEARTS[] = "hearts", SPADES[] = "spades", DIAMONDS[] = "diamonds", CLUBS[] = "clubs";

// card graphics
const char TOP[] = "┌─────┐", BOTTOM[] = "└─────┘";

typedef struct card_s {
    char suit[9], face[9];
    int value;
} card;
/*
 * Delay function used for the fast print and to delay the end in main
 * takes in milliseconds to delay
 */
void delay(const int milliseconds) {
    clock_t then;
    clock_t now = then = clock();
    while( (now-then) < milliseconds * (CLOCKS_PER_SEC / 1000) ) now = clock();
}
/*
 * Similar to printf, prints characters from left to right sequentially instead of instantly
 * takes in a message
 */
void fast_printf(const char *message) {
    for (int i = 0; i < strlen(message); i++) {
        putchar(message[i]);
        if (message[i] != '\n') {
            fflush(stdout);
            delay(30 + rand() % 31);
            fflush(stdout);
        } else {
            fflush(stdout);
            delay(30 + rand() % 31);
        }
    }
}

void assign_suit(card *deck, const char *suit, const int index) {
    int i;

    strcpy(deck[index].suit, suit);
    strcpy(deck[index].face, ACE);
    deck[index].value = 1;

    // 2 - 10
    for (i = index + 1; i < index + 10; i++) {
        strcpy(deck[i].suit, suit);
        strcpy(deck[i].face, NONE);
        deck[i].value = 1 + i - index;
    }

    strcpy(deck[i].suit, suit);
    strcpy(deck[i].face, JACK);
    deck[i].value = 10;
    i++;

    strcpy(deck[i].suit, suit);
    strcpy(deck[i].face, QUEEN);
    deck[i].value = 10;
    i++;

    strcpy(deck[i].suit, suit);
    strcpy(deck[i].face, KING);
    deck[i].value = 10;
}
/*
 * Shuffles the deck randomly
 * takes in the deck
 */
void shuffle_deck(card *deck) {
    for (int i = 0; i < 52; i++) {
        const int oldPos = rand() % 52, newPos = rand() % 52;
        const card temp = deck[oldPos];
        deck[oldPos] = deck[newPos];
        deck[newPos] = temp;
    }
}
/*
 * Initializes the cards and assigns the suits
 * takes in the deck
 */
void init(card deck[]){
    assign_suit(deck, "hearts", 0);
    assign_suit(deck, "clubs", 13);
    assign_suit(deck, "spades", 26);
    assign_suit(deck, "diamonds", 39);
    shuffle_deck(deck);
}
/*
 * Calculates the value of the hand
 * takes in the hand and the amount of cards in hand
 * returns the value of the hand
 */
int hand_value(const card *hand, const int len){
    int sum = 0;
    for(int i = 0 ;i < len;i++){
        sum += hand[i].value;
    }
    return sum;
}
/*
 * Prints cards to the screen side by side
 * takes in the cards, the amount of cards, and the player ID
 */
void print_cards(const card *cards, const int len, const int player) {
    printf("\n");
    for (int i = 0; i < len; i++) {
        printf("%s ", TOP);
    }

    printf("\n");

    for (int i = 0; i < len; i++) {
        if (!player && i == 0) {
            printf("│     │ ");
            continue;
        }

        if (strcmp(cards[i].face,NONE) != 0) printf("│%c    │ ", toupper(cards[i].face[0]));
        else if (cards[i].value == 10) printf("│%d   │ ", cards[i].value);
        else printf("│%d    │ ", cards[i].value);
    }

    printf("\n");

    for (int i = 0; i < len; i++) {
        if (!player && i == 0) {
            printf("│     │ ");
            continue;
        }

        const char *icon;
        if (strcmp(cards[i].suit, HEARTS) == 0) icon = RED "♥" RESET;
        else if (strcmp(cards[i].suit, CLUBS) == 0) icon = BLACK "♣" RESET;
        else if (strcmp(cards[i].suit, SPADES) == 0) icon = BLACK "♠" RESET;
        else if (strcmp(cards[i].suit, DIAMONDS) == 0) icon = RED "♦" RESET;
        else icon = "?";

        printf("│  %s  │ ", icon);
    }

    printf("\n");

    for (int i = 0; i < len; i++) {
        if (!player && i == 0) {
            printf("│     │ ");
            continue;
        }

        if (strcmp(cards[i].face,NONE) != 0) printf("│    %c│ ", toupper(cards[i].face[0]));
        else if (cards[i].value == 10) printf("│   %d│ ", cards[i].value);
        else printf("│    %d│ ", cards[i].value);
    }

    printf("\n");

    for (int i = 0; i < len; i++) {
        printf("%s ", BOTTOM);
    }

    int total = 0;
    for (int i = 0; i < len; i++) total += cards[i].value;
    printf("\n");

    if (player < 1) {
        fast_printf("Dealer's card(s)\n");
        return;
    }

    fast_printf("Player ");
    printf("%d's",player);
    fast_printf(" cards(s)\n");
}
/*
 * Prints out the dealer's last card
 * takes in cards and the value of the dealer's hand
 */
void dealer_reveal(card cards[], int dealerVal) {
    const char *icon;

    if (strcmp(cards[0].suit, HEARTS) == 0) icon = RED "♥" RESET;
    else if (strcmp(cards[0].suit, CLUBS) == 0) icon = "♣";
    else if (strcmp(cards[0].suit, SPADES) == 0) icon = "♠";
    else if (strcmp(cards[0].suit, DIAMONDS) == 0) icon = RED "♦" RESET;
    else icon = "?";

    const int hand = cards[0].value;
    if (hand == 10) {
        printf("%s\n│%d   │\n│  %s  │\n│   %d│\n%s\n",TOP,hand,icon,hand,BOTTOM);
        fast_printf("Dealer's hidden card.\n");
        return;
    }
    if (hand == 11) {
        printf("%s\n│%c    │\n│  %s  │\n│    %c│\n%s\n",TOP,'A',icon,'A',BOTTOM);
        fast_printf("Dealer's hidden card.\n");
        return;
    }
    printf("%s\n│%d    │\n│  %s  │\n│    %d│\n%s\n",TOP,hand,icon,hand,BOTTOM);
    fast_printf("Dealer's hidden card.\n" RED "Dealer's total Value: ");
    printf("%d\n" RESET,dealerVal);

}
/*
 * Compares the player's card to the dealers cards, prints out each situation and adds money to player accordingly
 * takes in the players cards, the amount of cards each player has, the total money for each player,
 * the total wager for each player, and the number of players
 */
void compare_cards(card *players[], int playerLen[], int money[], const int wager[], const int playerCount) {
    const int dealerVal = hand_value(players[0],playerLen[0]);
    if (dealerVal != 14) dealer_reveal(players[0],dealerVal);

    for (int i = 1;i < playerCount;i++) {
        char buffer[100];
        const int hand = hand_value(players[i],playerLen[i]);
        if ((hand == 14 || hand == 31) && dealerVal != 14) {
            snprintf(buffer,sizeof(buffer), GOLD "Player %d has %d and dealer does not have 14, Player %d beats the dealer!\n" RESET,i,hand,i);
            fast_printf(buffer);
            money[i] += wager[i] * 2;
        } else if (hand < 32 && hand > dealerVal) {
            snprintf(buffer,sizeof(buffer),GOLD "Player %d beats the dealer %d vs %d!\n" RESET,i,hand,dealerVal);
            fast_printf(buffer);
            money[i] += wager[i] * 2;
        } else {
            snprintf(buffer,sizeof(buffer),RED "Player %d loses to the dealer %d vs %d!\n" RESET, i,dealerVal, hand);
            fast_printf(buffer);
        }
    }
}

#endif //GAMEOF31_CARD_UTILS_H
