#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char ACE[] = "ace";
const char KING[] = "king";
const char QUEEN[] = "queen";
const char JACK[] = "jack";
const char NONE[] = "none";


typedef struct card_s {
    char suit[9];
    int value;
    char face[9];
} card;

void assign_suit(card *deck, const char *suit, const int index) {
    int king = 0;
    int queen = 0;
    int jack = 0;

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

void print_deck(card *deck) {
    for (int i = 0; i < 52; i++) {
        if (strcmp(deck[i].face, NONE) != 0) {
            printf("%s of %s\n", deck[i].face, deck[i].suit);
        } else {
            printf("%d of %s\n", deck[i].value, deck[i].suit);
        }
    }
}

void shuffle_deck(card *deck) {
    for (int i = 0; i < 52; i++) {
        int oldPos = rand() % 52;
        int newPos = rand() % 52;

        const card temp = deck[oldPos];
        deck[oldPos] = deck[newPos];
        deck[newPos] = temp;
    }
}


int main(void) {
    card deck[52];

    assign_suit(deck, "hearts", 0);
    assign_suit(deck, "clubs", 13);
    assign_suit(deck, "spades", 26);
    assign_suit(deck, "diamonds", 39);

    print_deck(deck);
    shuffle_deck(deck);
    print_deck(deck);

    return 0;
}



