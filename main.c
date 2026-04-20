#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const char ACE[] = "ace", KING[] = "king", QUEEN[] = "queen", JACK[] = "jack", NONE[] = "none";
#define NO_WIN -1
#define HAS_14 0
#define HAS_31 1
#define OVER_31 2

const int DRAWN = -1;


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

void print_cards(card *cards, const int deck_len) {
    for (int i = 0; i < deck_len; i++) {
        if (strcmp(cards[i].face, NONE) != 0) printf("%s of %s\n", cards[i].face, deck[i].suit);
        else printf("%d of %s\n", cards[i].value, cards[i].suit);
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

void draw(card *deck, card **hand, int *hand_len) {
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
        int input;
        do {
            printf("Enter value for ACE (1 or 11)\n");
            scanf("%d", &input);
        } while (input != 1 && input != 11);

        temp.value = input;

    }

    deck[val].value = DRAWN;
    (*hand)[*hand_len] = temp;
    *hand_len += 1;
}

int check_hand(card *hand, int hand_len) {
    int total = 0;
    for (int i = 0; i < hand_len; i++) total += hand[i].value;

    if (total == 31) return HAS_31;
    if (total == 14) return HAS_14;
    if (total > 31) return OVER_31;

    return NO_WIN;
}


int main(void) {
    srand(time(0));
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



