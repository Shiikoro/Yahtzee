#ifndef CONFIG_H
#define CONFIG_H

#define DICEAMMOUNT 5
#define DESCRIPTION_COLUMNE 0
#define DEFAULT_PLAYER_NAME "Player"
#define DEFAULT_NAME_ROW 0
#define DEFAULT_PLAYER_COUNT 2
#define DEFAULT_PLAYER_MIN 1
#define DEFAULT_PLAYER_MAX 10
#define INVALID_PLAYER_INDEX -1
#define DICE_ROLL_AMMOUNT 3

#define ACTIVE_PLAYER_COLOR QColor(Qt::darkGreen)
#define INACTIVE_PLAYER_COLOR QColor(Qt::darkRed)

#define NEED_BONUS_POINTS 63
#define BONUS 35

#define YAHTZEE 50
#define FULL_HOUSE 25
#define LARGE_STRAIGHT 40
#define SMALL_STRAIGHT 30

#define SHEET_DESCRIPTIONS QString("Name#1er#2er#3er#4er#5er#6er#"\
                                   "Summe oben#Bonus#Dreierpach#"\
                                   "Viererpach#Full House#Kleine Straße#"\
                                   "Große Straße#Kniffel#Chance#Summe unten#Gesamtsumme").split("#")

#define INDEX_ACES               1
#define INDEX_TWOS               2
#define INDEX_THREES             3
#define INDEX_FOURS              4
#define INDEX_FIVES              5
#define INDEX_SIXES              6
#define INDEX_TOP_SUM            7
#define INDEX_BONUS              8
#define INDEX_THREE_OF_A_KIND    9
#define INDEX_FOUR_OF_A_KIND    10
#define INDEX_FULL_HOUSE        11
#define INDEX_SMALL_STRAIGHT    12
#define INDEX_LARGE_STRAIGHT    13
#define INDEX_YAHTZEE           14
#define INDEX_CHANCE            15
#define INDEX_BOTTOM_SUM        16
#define INDEX_TOTAL_SUM         17
#define SHEET_BUTTON_INDEXES     ( QList< int >() << INDEX_ACES   << INDEX_TWOS\
                                                  << INDEX_THREES << INDEX_FOURS\
                                                  << INDEX_FIVES  << INDEX_SIXES\
                                                  << INDEX_THREE_OF_A_KIND\
                                                  << INDEX_FOUR_OF_A_KIND\
                                                  << INDEX_FULL_HOUSE\
                                                  << INDEX_SMALL_STRAIGHT\
                                                  << INDEX_LARGE_STRAIGHT\
                                                  << INDEX_YAHTZEE\
                                                  << INDEX_CHANCE )
#endif // CONFIG_H
