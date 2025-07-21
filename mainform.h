#ifndef MAINFORM_H
#define MAINFORM_H

#include <QtWidgets>
#include "player.h"
#include "ui_mainform.h"

class DiceWidget;

class MainForm : public QWidget, private Ui::mainForm
{
    Q_OBJECT

private:
    QList< DiceWidget* > diceWidgets;
    QList< QPushButton* > sheetButtons;
    QList< Player* > players;
    bool gameRunning;
    int currentPlayerIndex;

    void createDiceWidget();
    void createDiceWidgets();
    void createSheetLabel(int row);
    void createSheetButton(int row);
    void createSheetDescription( int row );
    void createSheetDescriptions();
    void createPlayer();
    void deletePlayer();
    void updateGui();
    void activateSheetBtn();
    void deactivateSheetBtn();
    int getNextPlayerIndex();
    void selectNextPlayer();
    void diceRoll(int index);
    void markCurrentPlayer();
    QList<int> getDiceValues()const;
    void displayWinner();
    void canRollDice(bool diceRollStatus);
    void diceActivate();
    void diceDeactivate();

    //Würfel freischalten / blockieren
private slots:
    void playerAmmountChanged(int index);
    void setGameRunning();
    void on_btnRoll_clicked();
    void sheetButtonClicked();

public:
    MainForm(QWidget *parent = 0);
    ~MainForm();

};

#endif // MAINFORM_H
