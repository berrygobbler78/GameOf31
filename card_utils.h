//
// Created by wgaris on 4/20/26.
//

#ifndef GAMEOF31_CARD_UTILS_H
#define GAMEOF31_CARD_UTILS_H

// default card vals
const char ACE[] = "ace", KING[] = "king", QUEEN[] = "queen", JACK[] = "jack", NONE[] = "none";
const char HEARTS[] = "hearts", SPADES[] = "spades", DIAMONDS[] = "diamonds", CLUBS[] = "clubs";

// card graphics
const char TOP[] = "┌─────┐", BOTTOM[] = "└─────┘";

typedef struct card_s {
    char suit[9], face[9];
    int value;
} card;

void assign_suit(card *deck, const char *suit, const int index) {
    int king = 0, queen = 0, jack = 0;

    for (int i = index; i < index + 13; i++) {
        strcpy(deck[i].suit, suit);
        deck[i].value = 1 + i - index;
        if (deck[i].value > 10) {
            deck[i].value = 10;
            if (jack == 0) {
                jack++;
                strcpy(deck[i].face, JACK);
                continue;
            }
            if (queen == 0) {
                queen++;
                strcpy(deck[i].face, QUEEN);
                continue;
            }
            if (king == 0) {
                king++;
                strcpy(deck[i].face, KING);
            }
        }
        else if (deck[i].value == 1) strcpy(deck[i].face, ACE);
        else strcpy(deck[i].face, NONE);
    }
}

void shuffle_deck(card *deck) {
    for (int i = 0; i < 52; i++) {
        const int oldPos = rand() % 52, newPos = rand() % 52;

        const card temp = deck[oldPos];
        deck[oldPos] = deck[newPos];
        deck[newPos] = temp;
    }
}

void init(card deck[]){
    assign_suit(deck, "hearts", 0);
    assign_suit(deck, "clubs", 13);
    assign_suit(deck, "spades", 26);
    assign_suit(deck, "diamonds", 39);
    shuffle_deck(deck);
}

int hand_value(card *hand, int len){
    int sum = 0;
    for(int i = 0 ;i < len;i++){
        sum += hand[i].value;
    }
    return sum;
}

void print_cards(card *cards, const int len, int player) {
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
        if (strcmp(cards[i].suit, HEARTS) == 0) icon = "♥";
        else if (strcmp(cards[i].suit, CLUBS) == 0) icon = "♣";
        else if (strcmp(cards[i].suit, SPADES) == 0) icon = "♠";
        else if (strcmp(cards[i].suit, DIAMONDS) == 0) icon = "♦";
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
        printf("Dealer's card(s)\n");
        return;
    }

    printf("Player %d's card(s)\n", player);
}

void print_value(card *cards, int len) {
    printf("Total value: %d\n", hand_value(cards, len));
}
/* Help with this pls */
void revealLastCard(card cards[]) {
    const char *icon;
    if (strcmp(cards[0].suit, HEARTS) == 0) icon = "♥";
    else if (strcmp(cards[0].suit, CLUBS) == 0) icon = "♣";
    else if (strcmp(cards[0].suit, SPADES) == 0) icon = "♠";
    else if (strcmp(cards[0].suit, DIAMONDS) == 0) icon = "♦";
    else icon = "?";
    int hand = hand_value(&cards[0],0);

    printf("%s\n│%d    │\n│  %c  │\n│    %d│\n%s\nDealer's last card.\n",TOP,hand,*icon,hand,BOTTOM);
}
void compare_cards(card *players, int *playerlen, int money[], int wager[], int playercount, int win) {
    if (win == 1) {
        for (int i = 1; i < playercount; i++) {
            money[i] += wager[i] * 2;
        }
        printf("All players win!");
        return;
    }
    int dealerval = hand_value(&players[0],playerlen[0]);

    if (dealerval == 14 || dealerval == 31) {
        printf("Dealer wins, dealer got %d\n",dealerval);
        return;
    }
    revealLastCard(players);
    for (int i = 1;i < playercount;i++) {
        const int hand = hand_value(&players[i],playerlen[i]);
        if (hand > dealerval) {
            printf("Player %d beats the dealer %d vs %d\n",i,hand,dealerval);
            money[i] += wager[i] * 2;
        }else {
            printf("Player %d loses to the dealer %d vs %d \n", i,dealerval, hand);
        }
    }
}

#endif //GAMEOF31_CARD_UTILS_H
