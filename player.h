#ifndef PLAYER_H
#define PLAYER_H

#include <QtWidgets>

class Player : public QObject
{
private:
    QGridLayout* layout;
    QList< QLineEdit* > lineEdits;
    int index;

    void createLineEdit(int row);
    void createLineEdits();
    void calcAndDisplayPoints();
    void fillTopField(const QList< int >& diceValues, int editIndex);
    void fillBottomField(const QList< int >& diceValues, int editIndex);
    void fillOfSameKindField(const QList< int >& diceValues, int kindCount);
    void fillFullHouse(const QList< int >& diceValues);
    void fillSmallStraight(const QList< int >& diceValues);
    void fillLargeStraight(const QList< int >& diceValues);
    void fillYahtzee(const QList< int >& diceValues);
    void fillChance(const QList< int >& diceValues);
    void fillEdit(int editIndex, int points);

public:
    explicit    Player(int playerIndex, QGridLayout* gridLayout, QObject *parent = 0);
    virtual    ~Player();
    void setActivePlayer(bool active);
    void countPoints();
    void resetPoints();

    void setDice(const QList< int >& diceValues, int buttonIndex);
    static bool isOfSameKind(const QList< int >& diceValues, int kindCount);
    static bool isFullHouse(const QList< int >& diceValues);
    static bool isSmallStraight(const QList< int >& diceValues);
    static bool isLargeStraight(const QList< int >& diceValues);
    static bool isYahtzee(const QList< int >& diceValues);
    static int getDiceValueSum(const QList< int >& diceValues);
    bool isEmpty( int rowIndex ) const;
    bool isReady() const;
    int getTotalPoints() const;
    QString getName() const;
};
//Konstruktor: Anweisungen, wie eine Objekte initialisiert wird.
#endif // PLAYER_H
