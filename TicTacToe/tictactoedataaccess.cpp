#include "tictactoedataaccess.h"
#include <QDateTime>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>

QVector<QString> TicTacToeDataAccess::saveGameList() const
{
    QVector<QString> result(5);

    // végigmegyünk az 5 helyen
    for (int i = 0; i < 5; i++)
    {
        if (QFile::exists("game" + QString::number(i) + ".sav")) // ha az adott mentés létezik
        {
            QFileInfo info("game"+ QString::number(i) + ".sav");
            result[i] = "[" + QString::number(i + 1) + "] " + info.lastModified().toString("yyyy.MM.dd HH:mm:ss");
            // akkor betöltjük a fájl módosítás időpontját
        }
    }

    return result;
}

bool TicTacToeDataAccess::loadGame(int gameIndex, QVector<int> &saveGameData, int table)
{
    QFile file("game" + QString::number(gameIndex) + ".sav");
    if (!file.open(QFile::ReadOnly))
        return false;

    QTextStream stream(&file);
    int T[4];
    T[0] = stream.readLine().toInt();
    T[1] = stream.readLine().toInt();
    T[2] = stream.readLine().toInt();
    T[3] = stream.readLine().toInt();
    saveGameData.resize(T[2]*T[3]+4);
    saveGameData[0] = T[0];
    saveGameData[1] = T[1];
    saveGameData[2] = T[2];
    saveGameData[3] = T[3];
    // soronként beolvassuk az adatokat a fájlból
    for (int i = 4;i < (T[2]*T[3]+4); i++)
        saveGameData[i] = stream.readLine().toInt();

    file.close();

    return true;
}

bool TicTacToeDataAccess::saveGame(int gameIndex, const QVector<int> &saveGameData, int table)
{
    QFile file("game" + QString::number(gameIndex) + ".sav");
    if (!file.open(QFile::WriteOnly))
        return false;

    QTextStream stream(&file);

    // soronként egy adatot írunk ki
    for (int i = 0; i < (table+4); i++)
        stream << saveGameData[i] << endl;

    file.close();

    return true;
}

