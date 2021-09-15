#ifndef TICTACTOEDATAACCESS_H
#define TICTACTOEDATAACCESS_H

#include <QString>
#include <QVector>

class TicTacToeDataAccess // az adatelérés típusa
{
public:
    explicit TicTacToeDataAccess() {}

    QVector<QString> saveGameList() const; // mentett játékok lekérdezése

    bool loadGame(int gameIndex, QVector<int> &saveGameData, int table); // játék betöltése
    bool saveGame(int gameIndex, const QVector<int> &saveGameData, int table); // játék mentése
};

#endif // TICTACTOEDATAACCESS_H
