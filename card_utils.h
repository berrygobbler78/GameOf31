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

void delay(int milliseconds)
{
    clock_t then;

    long pause = milliseconds * (CLOCKS_PER_SEC / 1000);
    clock_t now = then = clock();
    while( (now-then) < pause ) now = clock();
}

void fast_printf(const char *message) {
    for (int i = 0; i < strlen(message); i++) {
        putchar(message[i]);
        if (message[i] != '\n') {
            //putchar('|'); // To mimic a cursor
            fflush(stdout); // Ensures char is printed immediately
            delay(50 + rand() % 51); // Delay between two vals
            //putchar('\b'); // 'backspace' removes '|'
            fflush(stdout);
        } else {
            fflush(stdout);
            delay(100 + rand() % 201);
        }
    }
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

int hand_value(const card *hand, int len){
    int sum = 0;
    for(int i = 0 ;i < len;i++){
        sum += hand[i].value;
    }
    return sum;
}

void print_cards(const card *cards, const int len, int player) {
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
        fast_printf("Dealer's card(s)\n");
        return;
    }
    fast_printf("Player ");
    printf("%d's",player);
    fast_printf(" cards(s)\n");
}

/*void print_value(card *cards, int len) {
    printf("Total value: %d\n", hand_value(cards, len));
}*/
/* Help with this pls don't know why it's not working*/
void revealLastCard(card cards[]) {
    const char *icon;
    if (strcmp(cards[0].suit, HEARTS) == 0) icon = "♥";
    else if (strcmp(cards[0].suit, CLUBS) == 0) icon = "♣";
    else if (strcmp(cards[0].suit, SPADES) == 0) icon = "♠";
    else if (strcmp(cards[0].suit, DIAMONDS) == 0) icon = "♦";
    else icon = "?";
    int hand = cards[0].value;
    if (hand == 10) {
        printf("%s\n│%d   │\n│  %s  │\n│   %d│\n%s\n",TOP,hand,icon,hand,BOTTOM);
        fast_printf("Dealer's last card.\n");
        return;
    }
    if (hand == 11) {
        printf("%s\n│%c    │\n│  %s  │\n│    %c│\n%s\n",TOP,'A',icon,'A',BOTTOM);
        fast_printf("Dealer's last card.\n");
        return;
    }
    printf("%s\n│%d    │\n│  %s  │\n│    %d│\n%s\n",TOP,hand,icon,hand,BOTTOM);
    fast_printf("Dealer's last card.\n");
}
void compare_cards(card *players[], int playerLen[], int money[], int wager[], int playerCount, int win) {
    if (win == 1) {
        for (int i = 1; i < playerCount; i++) {
            money[i] += wager[i] * 2;
        }
        fast_printf("All players win!\n");
        return;
    }
    int dealerVal = hand_value(players[0],playerLen[0]);

    if (dealerVal == 31) {
        fast_printf("Dealer wins, dealer got 31!\n");
        return;
    }
    if (dealerVal != 14) revealLastCard(players[0]);

    for (int i = 1;i < playerCount;i++) {
        char buffer[100];
        const int hand = hand_value(players[i],playerLen[i]);
        if ((hand == 14 || hand == 31) && dealerVal != 14) {
            snprintf(buffer,sizeof(buffer),"Player %d has %d and dealer does not have 14, Player %d beats the dealer!\n",i,hand,i);
            fast_printf(buffer);
            money[i] += wager[i] * 2;
        }else if (hand < 32 && hand > dealerVal) {
            snprintf(buffer,sizeof(buffer),"Dealer beats Player %d  %d vs %d!\n",i,hand,dealerVal);
            fast_printf(buffer);
            money[i] += wager[i] * 2;
        }else {
            snprintf(buffer,sizeof(buffer),"Player %d loses to the dealer %d vs %d!\n", i,dealerVal, hand);
            fast_printf(buffer);
        }
    }
}

#endif //GAMEOF31_CARD_UTILS_H
