#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>

// default card vals
const char ACE[] = "ace", KING[] = "king", QUEEN[] = "queen", JACK[] = "jack", NONE[] = "none";
const char HEARTS[] = "hearts", SPADES[] = "spades", DIAMONDS[] = "diamonds", CLUBS[] = "clubs";

// card graphics
const char TOP[] = "┌─────┐", BOTTOM[] = "└─────┘";

// win conditions
#define NO_WIN (-1)
#define HAS_14 0
#define HAS_31 1
#define OVER_31 2

#define DRAWN (-1)

typedef struct card_s {
    char suit[9], face[9];
    int value;
} card;

int hand_value(card *hand, int len){
    int sum = 0;
    for(int i = 0 ;i < len;i++){
        sum += hand[i].value;
    }
    return sum;
}
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

void print_cards(card *cards, const int len) {
    for (int i = 0; i < len; i++) {
        printf("%s ", TOP);
    }

    printf("\n");

    for (int i = 0; i < len; i++) {
        if (strcmp(cards[i].face,NONE) != 0) printf("|%c    │ ", toupper(cards[i].face[0]));
        else if (cards[i].value == 10) printf("|%d   │ ", cards[i].value);
        else printf("|%d    │ ", cards[i].value);
    }

    printf("\n");

    for (int i = 0; i < len; i++) {
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
        if (strcmp(cards[i].face,NONE) != 0) printf("|    %c│ ", toupper(cards[i].face[0]));
        else if (cards[i].value == 10) printf("|   %d│ ", cards[i].value);
        else printf("|    %d│ ", cards[i].value);
    }

    printf("\n");

    for (int i = 0; i < len; i++) {
        printf("%s ", BOTTOM);
    }

    int total = 0;
    for (int i = 0; i < len; i++) total += cards[i].value;
    printf("\nTotal Value: %d\n", total);
}

void shuffle_deck(card *deck) {
    for (int i = 0; i < 52; i++) {
        const int oldPos = rand() % 52, newPos = rand() % 52;

        const card temp = deck[oldPos];
        deck[oldPos] = deck[newPos];
        deck[newPos] = temp;
    }
}

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
                printf("Enter value for ACE (1 or 11)\n");
                scanf("%d", &input);
            } while (input != 1 && input != 11);

            temp.value = input;
        }

    }

    deck[val].value = DRAWN;
    (*hand)[*hand_len] = temp;
    *hand_len += 1;
}

void dealer_turn(card *deck, card **hand, int *hand_len) {
    printf("Dealer's turn...\n");

    while (1) {
        draw(deck, hand, hand_len,1);
        int result = hand_value(*hand,*hand_len);

        if (result == OVER_31) {
            printf("Dealer busts! All players win!\n");
            print_cards(*hand, *hand_len);
            return;
        }
        if (result == HAS_31) {
            printf("Dealer hits 31!\n");
            print_cards(*hand, *hand_len);
            return;
        }
        if (result == HAS_14) {
            printf("Dealer hits 14, must stop.\n");
            print_cards(*hand, *hand_len);  // Reveal all
            return;
        }

        // Dealer's strategy: stop at 26+
        if (hand_value(*hand, *hand_len) >= 26) {
            printf("Dealer stands.\n");
            // Reveal all except last card
            print_cards(*hand, *hand_len - 1);
            printf("┌─────┐\n│     │\n│     │\n│     │\n└─────┘\n");
            printf("[ FACE DOWN ]\n");
            return;
        }
    }
}

int check_hand(card *hand, int hand_len) {
    int total = 0;
    for (int i = 0; i < hand_len; i++) total += hand[i].value;

    if (total == 31) return HAS_31;
    if (total == 14) return HAS_14;
    if (total > 31) return OVER_31;

    return NO_WIN;
}

void init(card deck[]){
    assign_suit(deck, "hearts", 0);
    assign_suit(deck, "clubs", 13);
    assign_suit(deck, "spades", 26);
    assign_suit(deck, "diamonds", 39);
    shuffle_deck(deck);
}

int main(void) {
    srand(time(0));

    // deck setup
    card deck[52];
    init(deck);

    // player setup
    int player_count = 0;
    do {
        printf("Enter number of players:\n");
        scanf("%d", &player_count);
    } while (player_count < 1);

    player_count++;

    card *players[player_count];
    int player_len[player_count];

    for (int i = 0; i < player_count; i++) {
        players[i] = NULL;
        player_len[i] = 0;
    }

    for (int i = 0; i < player_count; i++){
        if(i == 0){
            dealer_turn(deck,&players[i],&player_len[i]);
            continue;
        }

        printf("Player %d's turn:\n",i);
        int win = -1;
        while (1) {
            draw(deck, &players[i], &player_len[i],0);
            switch (check_hand(players[i], player_len[i])) {
                case HAS_14: printf("Hit 14\n"); win = 1; break;
                case HAS_31: printf("Hit 31\n"); win = 1; break;
                case OVER_31: printf("Over 31\n"); win = 1; break;
            }

            if (win != -1) break;

            print_cards(players[i], player_len[i]);

            int temp;
            printf("Continue? (1 for yes, 0 for no)\n");
            if (scanf("%d", &temp) == 0 || temp == 0) break;
        }
        printf("Player %d's turn is over\n",i);
    }

    for (int i = 0; i < player_count; i++) free(players[i]);

    return 0;
}



