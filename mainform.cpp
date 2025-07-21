#include "mainform.h"
#include "dicewidget.h"
#include "config.h"

MainForm::MainForm(QWidget *parent)
    : QWidget(parent), gameRunning(false), currentPlayerIndex(INVALID_PLAYER_INDEX)
{
    setupUi(this);
    connect(btnGameStart, SIGNAL(clicked()), this, SLOT(setGameRunning()));
    connect(btnGameBreak, SIGNAL(clicked()), this, SLOT(setGameRunning()));
    createDiceWidgets();
    createSheetDescriptions();
    connect(spinPlayerAmmount, SIGNAL(valueChanged(int)), this,SLOT(playerAmmountChanged(int)));
    spinPlayerAmmount->setMinimum(DEFAULT_PLAYER_MIN);
    spinPlayerAmmount->setMaximum(DEFAULT_PLAYER_MAX);
    spinPlayerAmmount->setValue(DEFAULT_PLAYER_COUNT);
    playerAmmountChanged(spinPlayerAmmount->value());
    lcdRolleAmmount->display(DICE_ROLL_AMMOUNT);
    updateGui();
}

MainForm::~MainForm()
{
}

void MainForm::setGameRunning()
{
    gameRunning = !gameRunning;
    lcdRolleAmmount->display(DICE_ROLL_AMMOUNT);
    currentPlayerIndex = rand() % players.count();
    if(gameRunning)
    {
        selectNextPlayer();
        markCurrentPlayer();
    }
    else currentPlayerIndex = INVALID_PLAYER_INDEX;
    updateGui();

    for(int i = 0; i < players.length(); i++)
        players[i]->resetPoints();
}

void MainForm::on_btnRoll_clicked()
{
    int rolls = lcdRolleAmmount->value();
    if(rolls > 0)
    {
        for(int i = 0; i < diceWidgets.length(); i++)
        {
            diceWidgets[i]->rollDice();
        }
    }
    rolls = lcdRolleAmmount->value() - 1;
    lcdRolleAmmount->display(rolls);
    if(gameRunning && rolls < DICE_ROLL_AMMOUNT)
    {
        activateSheetBtn();
        canRollDice(true);
    }
    else deactivateSheetBtn();
}

void MainForm::sheetButtonClicked()
{
    //sender ermitteln -> QObjext zeiger ein QPushbuttonzeiger, Static cast -> in der Liste schauen, wo der knopf ist.
    int buttonIndex = sheetButtons.indexOf(static_cast<QPushButton*>(sender()));
    players[currentPlayerIndex]->setDice(getDiceValues(), buttonIndex);
    selectNextPlayer();
    diceActivate();
    canRollDice(false);

    if(players[currentPlayerIndex]->isReady())
        displayWinner();
}

void MainForm::createDiceWidget()
{
    DiceWidget* dice = new DiceWidget;
    dice->setMayChange(false);
    diceLayout->addWidget(dice);
    connect(dice, SIGNAL(clicked()), dice, SLOT(toggleMayRoll()));
    diceWidgets << dice;
}

void MainForm::createDiceWidgets()
{
    for(int i = 0; i < DICEAMMOUNT; i++)
    {
        createDiceWidget();
    }
}

void MainForm::createSheetLabel(int row)
{
    QLabel* label = new QLabel(SHEET_DESCRIPTIONS[row]);
    gameFieldLayout->addWidget(label,row,DESCRIPTION_COLUMNE);
    label->setAlignment(Qt::AlignCenter);
    label->setFrameShape(QFrame::Box);
}

void MainForm::createSheetButton(int row)
{
    QPushButton* sheetbtn = new QPushButton(SHEET_DESCRIPTIONS[row]);
    gameFieldLayout->addWidget(sheetbtn,row,DESCRIPTION_COLUMNE);
    connect(sheetbtn, SIGNAL(clicked()),this , SLOT(sheetButtonClicked()));
    sheetButtons << sheetbtn;

}

void MainForm::createSheetDescription(int row)
{
    if( SHEET_BUTTON_INDEXES.contains(row)) createSheetButton( row );
    else createSheetLabel( row );
}

void MainForm::createSheetDescriptions()
{
    for(int i = 0; i < SHEET_DESCRIPTIONS.length(); i++)
        createSheetDescription(i);
}

void MainForm::createPlayer()
{
    Player* player = new Player(players.count(), gameFieldLayout);
    players << player;
    player->countPoints();
}

void MainForm::deletePlayer()
{
    delete players.takeLast();
}

void MainForm::updateGui()
{
    btnGameStart->setVisible(!gameRunning);
    btnGameBreak->setVisible(gameRunning);
    btnRoll->setDisabled(!gameRunning);
    spinPlayerAmmount->setDisabled(gameRunning);
    deactivateSheetBtn();
}

void MainForm::activateSheetBtn()
{
    for(int i = 0; i < SHEET_BUTTON_INDEXES.length(); i++)
        if(players[currentPlayerIndex]->isEmpty(SHEET_BUTTON_INDEXES[i]))
            sheetButtons[i]->setDisabled(false);
}

void MainForm::deactivateSheetBtn()
{
    for(int i = 0; i < SHEET_BUTTON_INDEXES.length(); i++)
        sheetButtons[i]->setDisabled(true);
}

int MainForm::getNextPlayerIndex()
{
    int nextPlayerIndex = currentPlayerIndex;
    if(nextPlayerIndex == players.length() - 1)
        return 0;
    else
        return nextPlayerIndex + 1;
    // return (currentPlayerIndex + 1) % players.count();
}

void MainForm::selectNextPlayer()
{
    currentPlayerIndex = getNextPlayerIndex();
    lcdRolleAmmount->display(DICE_ROLL_AMMOUNT);
    markCurrentPlayer();
    deactivateSheetBtn();
}

void MainForm::diceRoll(int index)
{
    diceWidgets[index]->rollDice();
}

void MainForm::markCurrentPlayer()
{
    for(int i = 0; i < players.length(); i++)
    {
        players[i]->setActivePlayer(false);
        if(currentPlayerIndex>-1)
            players[currentPlayerIndex]->setActivePlayer(true);
    }
}

QList<int> MainForm::getDiceValues() const
{
    QList< int > diceValues;
    for(int i = 0; i < diceWidgets.length(); i++)
        diceValues << diceWidgets[i]->getValue();
    return diceValues;
}

void MainForm::displayWinner()
{
    QList< Player* > ranking = players;
    std::sort(ranking.begin(), ranking.end(), [](Player* a, Player* b)
    {
        return a->getTotalPoints() > b->getTotalPoints();
    });
    QMessageBox winners;
    QString place;
    int p = 1;
    for(int i = 0; i < ranking.count(); i++)
    {
        place += QString ("%1. %2 mit %3 Punkten\n").arg(p).arg(ranking[i]->getName()).arg(ranking[i]->getTotalPoints());
        if(i < ranking.count()-1)
            if(ranking[i]->getTotalPoints() != ranking[i+1]->getTotalPoints())
                p++;
    }
    winners.setText(place);
    winners.exec();
}

void MainForm::canRollDice(bool diceRollStatus)
{
    for(int i = 0; i < diceWidgets.count(); i++)
        diceWidgets[i]->setMayChange(diceRollStatus);
}

void MainForm::diceActivate()
{
    for(int i = 0; i < diceWidgets.count(); i++)
        diceWidgets[i]->setMayRollOn();
}

void MainForm::playerAmmountChanged(int index)
{
    while(index > players.length())
        createPlayer();
    while(index < players.length())
        deletePlayer();
}
