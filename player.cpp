#include "player.h"
#include "config.h"

Player::Player(int playerIndex, QGridLayout *gridLayout, QObject *parent) : QObject(parent), layout(gridLayout), index(playerIndex+1)
{
    createLineEdits();
    countPoints();
}

Player::~Player()
{
    while(!lineEdits.isEmpty()) delete lineEdits.takeLast();
}

void Player::createLineEdit(int row)
{
    QLineEdit* edit = new QLineEdit;
    lineEdits << edit;
    edit->setAlignment(Qt::AlignCenter);
    if(row == DEFAULT_NAME_ROW)
    {
        edit->setText(QString("%1 %2").arg(DEFAULT_PLAYER_NAME).arg(index));
    }
    else edit->setReadOnly(true);
    layout->addWidget(edit ,row ,index);
}

void Player::createLineEdits()
{
    for(int i = 0; i < SHEET_DESCRIPTIONS.length(); i++)
        createLineEdit(i);
}

void Player::fillTopField(const QList<int> &diceValues, int editIndex)
{
    int points = editIndex - INDEX_ACES + 1;
    int count = diceValues.count(points);
    fillEdit(editIndex, count * points);
}

void Player::fillBottomField(const QList<int> &diceValues, int editIndex)
{
    switch (editIndex) {
    case INDEX_THREE_OF_A_KIND:
        fillOfSameKindField(diceValues, 3);
        break;
    case  INDEX_FOUR_OF_A_KIND:
        fillOfSameKindField(diceValues, 4);
        break;
    case INDEX_FULL_HOUSE:
        fillFullHouse(diceValues);
        break;
    case INDEX_SMALL_STRAIGHT:
        fillSmallStraight(diceValues);
        break;
    case INDEX_LARGE_STRAIGHT:
        fillLargeStraight(diceValues);
        break;
    case INDEX_YAHTZEE:
        fillYahtzee(diceValues);
        break;
    case INDEX_CHANCE:
        fillChance(diceValues);
        break;
    default:
        break;
    }
}

void Player::fillOfSameKindField(const QList<int> &diceValues, int kindCount)
{
    int index = kindCount == 3 ? INDEX_THREE_OF_A_KIND : INDEX_FOUR_OF_A_KIND;
    fillEdit(index, isOfSameKind(diceValues, kindCount) ? getDiceValueSum(diceValues) : 0);
}

void Player::fillFullHouse(const QList<int> &diceValues)
{
    fillEdit(INDEX_FULL_HOUSE, isFullHouse(diceValues) ? getDiceValueSum(diceValues) : 0);
}

void Player::fillSmallStraight(const QList<int> &diceValues)
{
    fillEdit(INDEX_SMALL_STRAIGHT, isSmallStraight(diceValues) ? SMALL_STRAIGHT : 0);
}

void Player::fillLargeStraight(const QList<int> &diceValues)
{
    fillEdit(INDEX_LARGE_STRAIGHT, isLargeStraight(diceValues) ? LARGE_STRAIGHT : 0);
}

void Player::fillYahtzee(const QList<int> &diceValues)
{
    fillEdit(INDEX_YAHTZEE, isYahtzee(diceValues) ? YAHTZEE : 0);
}

void Player::fillChance(const QList<int> &diceValues)
{
    fillEdit(INDEX_CHANCE,getDiceValueSum(diceValues));
}

void Player::fillEdit(int editIndex, int points)
{
    lineEdits[editIndex]->setText(QString::number(points)); //lineEdits[editIndex]->setText(QString("%1").arg(points));
}

void Player::setActivePlayer(bool active)
{
    QPalette pal = lineEdits[DEFAULT_NAME_ROW]->palette();
    pal.setColor(QPalette::Base, active ? ACTIVE_PLAYER_COLOR : INACTIVE_PLAYER_COLOR);
    lineEdits[DEFAULT_NAME_ROW]->setPalette(pal);
}

void Player::countPoints()
{
    int sumTop = 0;
    int sumBottom = 0;
    int bonus = 0;
    QLocale l = QLocale::system();
    for(int i = 0; i < SHEET_BUTTON_INDEXES.length(); i++)
    {
        QString editText = lineEdits[SHEET_BUTTON_INDEXES[i]]->text();
        int value = l.toInt(editText);

        if(i < INDEX_SIXES)
            sumTop += value;
        else if (i > INDEX_FIVES)
            sumBottom += value;
        if(sumTop >= NEED_BONUS_POINTS)
            bonus = BONUS;
    }

    lineEdits[INDEX_TOP_SUM]->setText(l.toString(sumTop));
    lineEdits[INDEX_BONUS]->setText(l.toString(bonus));
    lineEdits[INDEX_BOTTOM_SUM]->setText(l.toString(sumBottom));
    lineEdits[INDEX_TOTAL_SUM]->setText(l.toString(sumTop + sumBottom + bonus));
}

void Player::resetPoints()
{
    QList< int > entries = SHEET_BUTTON_INDEXES;
    for(int i = 0; i < entries.length(); i++)
        lineEdits[entries[i]]->clear();
    //lineEdits[DEFAULT_NAME_ROW]->setText(QString("%1 %2").arg(DEFAULT_PLAYER_NAME).arg(index));
    countPoints();
}

void Player::setDice(const QList<int> &diceValues, int buttonIndex)
{
    int editIndex = SHEET_BUTTON_INDEXES[buttonIndex];
    if(editIndex >= INDEX_ACES && editIndex <= INDEX_SIXES)
        fillTopField(diceValues, editIndex);
    else
        fillBottomField(diceValues, editIndex);
    countPoints();
}

bool Player::isOfSameKind(const QList<int> &diceValues, int kindCount)
{
    for(int i = 0; i < diceValues.count() - kindCount + 1; i++)
        if(diceValues.count(diceValues[i]) >= kindCount) return true;
    return false;
}

bool Player::isFullHouse(const QList<int> &diceValues)
{
    for(int i = 0; i < diceValues.count();i++)
    {
        int count = diceValues.count(diceValues[i]);
        if(count != 2 && count != 3 && count != 5) return false;
    }
    return true;
}

bool Player::isSmallStraight(const QList<int> &diceValues)
{
    if(!diceValues.contains(3) || !diceValues.contains(4))
        return false;
    if (diceValues.contains(2) && (diceValues.contains(1) || diceValues.contains(5)))
        return true;
    return diceValues.contains(5) && diceValues.contains(6);
}

bool Player::isLargeStraight(const QList<int> &diceValues)
{
    for(int i = 2; i <= 5; i++ )
        if(!diceValues.contains(i))
            return false;
    return diceValues.contains(1) || diceValues.contains(6);
}

bool Player::isYahtzee(const QList<int> &diceValues)
{
    return diceValues.count(diceValues[0]) == diceValues.count();
}

int Player::getDiceValueSum(const QList<int> &diceValues)
{
    int summe = 0;
    for(int i = 0; i < diceValues.count(); i++)
        summe += diceValues[i];
    return summe;
}

bool Player::isEmpty(int rowIndex) const
{
    if(rowIndex >= lineEdits.count() || rowIndex < 0 || rowIndex == DEFAULT_NAME_ROW)
        return false;
    return lineEdits[rowIndex]->text().isEmpty();
}

bool Player::isReady() const
{
    for(int i = 0; i < lineEdits.length(); i++)
        if(lineEdits[i]->text().isEmpty())
            return false;
    return true;
}

int Player::getTotalPoints() const
{
    QLocale l = QLocale::system();
    return l.toInt(lineEdits[INDEX_TOTAL_SUM]->text());
}

QString Player::getName() const
{
    return lineEdits[DEFAULT_NAME_ROW]->text();
}
