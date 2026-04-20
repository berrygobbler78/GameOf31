#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>

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

void delay(int milliseconds)
{
    long pause;
    clock_t now,then;

    pause = milliseconds*(CLOCKS_PER_SEC/1000);
    now = then = clock();
    while( (now-then) < pause ) now = clock();
}

void slow_printf(char *message) {
    for (int i = 0; i < strlen(message); i++) {
        putchar(message[i]);
        if (message[i] != '\n') {
            putchar('|'); // To mimic a cursor
            fflush(stdout); // Ensures char is printed immediately
            delay(100 + rand() % 101); // Delay between two vals
            putchar('\b'); // 'backspace' removes '|'
            fflush(stdout);
        } else {
            fflush(stdout);
            delay(100 + rand() % 201);
        }
    }
}

void fast_printf(char *message) {
    for (int i = 0; i < strlen(message); i++) {
        putchar(message[i]);
        if (message[i] != '\n') {
            putchar('|'); // To mimic a cursor
            fflush(stdout); // Ensures char is printed immediately
            delay(50 + rand() % 51); // Delay between two vals
            putchar('\b'); // 'backspace' removes '|'
            fflush(stdout);
        } else {
            fflush(stdout);
            delay(100 + rand() % 201);
        }
    }
}
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

void print_cards(card *cards, const int deck_len) {
    for (int i = 0; i < deck_len; i++) {
        
    }
    for(int i = 0; i < deck_len;i++){
        char icon[10];
        char msg[100];
       
        if(strcmp(cards[i].suit,"hearts") != 0) strcpy(icon,"\u2665");
        if(strcmp(cards[i].suit,"clubs") != 0) strcpy(icon,"\u2663");
        if(strcmp(cards[i].suit,"spades") != 0) strcpy(icon,"\u2660");
        if(strcmp(cards[i].suit,"diamonds") != 0) strcpy(icon,"\u2666");
        if(strcmp(cards[i].face,NONE) != 0){
            printf("┌─────┐\n│%c    │\n│  %s  │\n│    %c│\n└─────┘\n",toupper(cards[i].face[0]),icon,toupper(cards[i].face[0]));
        }else{
           printf("┌─────┐\n│%d    │\n│  %s  │\n│    %d│\n└─────┘\n",cards[i].value,icon,cards[i].value); 
        }
        if (strcmp(cards[i].face, NONE) != 0) sprintf(msg, "%s of %s\n", cards[i].face, cards[i].suit);
        else sprintf(msg, "%d of %s\n", cards[i].value, cards[i].suit);
        slow_printf(msg);
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
    if (*hand_len >= 52) {
        slow_printf("No more cards in the deck.\n");
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
        int input;
        do {
            slow_printf("Enter value for ACE (1 or 11)\n");
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
void init(card deck[]){
    assign_suit(deck, "hearts", 0);
    assign_suit(deck, "clubs", 13);
    assign_suit(deck, "spades", 26);
    assign_suit(deck, "diamonds", 39);
    shuffle_deck(deck);
}

int main(void) {
    srand(time(0));
    card deck[52];
    card *p1_hand = NULL, *p2_hand = NULL;
    int p1_len = 0, p2_len = 0;
    init(deck);
   
    //slow_printf("Hello world\n");

    

    while (1) {
        draw(deck, &p1_hand, &p1_len);
        switch (check_hand(p1_hand, p1_len)) {
            case NO_WIN: slow_printf("No win\n"); break;
            case HAS_14: slow_printf("Hit 14\n"); break;
            case HAS_31: slow_printf("Hit 31\n"); break;
            case OVER_31: slow_printf("Over 31\n"); break;
        }

        print_cards(p1_hand, p1_len);
        printf("Total value: %d\n",hand_value(p1_hand,p1_len));

        int temp;
        fast_printf("Continue? (1 for yes, 0 for no)\n");
        if (scanf("%d", &temp) == 0 || temp == 0) break;
    }

    free(p1_hand);
    if (p2_hand) free(p2_hand);

    return 0;
}



