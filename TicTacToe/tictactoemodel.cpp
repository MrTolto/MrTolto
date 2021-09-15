#include "tictactoemodel.h"

TicTacToeModel::TicTacToeModel()
{

}

TicTacToeModel::~TicTacToeModel()
{
    //delete _gameTable;
}

void TicTacToeModel::newGame()
{
    if (!_gameTable){
        delete _gameTable;
    }
    _gameTable = new Player*[_rows];
    for (int i = 0; i < _rows; ++i)
    {
        _gameTable[i] = new Player[_cols];
    }
    for (int i = 0; i < _rows; ++i)
        for (int j = 0; j < _cols; ++j)
        {
            _gameTable[i][j] = NoPlayer; // a játékosok pozícióit töröljük
        }

    _stepNumber = 0;
    _currentPlayer = PlayerX; // először az X lép
}

void TicTacToeModel::stepGame(int x, int y)
{
    if (_stepNumber >= _cols * _rows) // ellenőrizzük a lépésszámot
        return;
    if (x < 0 || x > (_rows-1) || y < 0 || y > (_cols-1)) // ellenőrizzük a tartomány
        return;
    if (_gameTable[x][y] != 0) // ellenőrizzük a mezőt
        return;
    for (int i = 0; i < _cols ; i++)
    {
            if (_gameTable[x][i] != NoPlayer && i > 0){
                _gameTable[x][i-1] = _currentPlayer; // pozíció rögzítése
                fieldChanged(x, i-1, _currentPlayer);
                break;
            }
            else if (_gameTable[x][i] == NoPlayer && i == _cols-1)
            {
                _gameTable[x][i] = _currentPlayer; // pozíció rögzítése
                fieldChanged(x, i, _currentPlayer);
                break;
            }
    }
     // jelezzük egy eseménykiváltással, hogy változott a mező

    _stepNumber++;
    _currentPlayer = (Player)(_currentPlayer % 2 + 1);
    // egészként kezelhető az érték, de konvertálnunk kell

    checkGame();
}

bool TicTacToeModel::loadGame(int gameIndex)
{
    QVector<int> saveGameData;

    if (!_dataAccess.loadGame(gameIndex, saveGameData, _cols*_rows)) // az adatelérés végzi a tevékenységeket
        return false;

    // feldolgozttuk a kapott vektort
    _stepNumber = saveGameData[0];
    _currentPlayer = (Player)saveGameData[1];
    _rows = saveGameData[2];
    _cols = saveGameData[3];
        delete _gameTable;

    _gameTable = new Player*[_rows];
    for (int i = 0; i < _rows; ++i)
    {
        _gameTable[i] = new Player[_cols];
    }
    for (int i = 0; i < _rows; ++i)
        for (int j = 0; j < _cols; ++j)
        {
            _gameTable[i][j] = (Player)saveGameData[4 + i * _cols + j];
        }

    return true;
}

bool TicTacToeModel::saveGame(int gameIndex)
{
    QVector<int> saveGameData;

    // összerakjuk a megfelelő tartalmat
    saveGameData.push_back(_stepNumber);
    saveGameData.push_back((int)_currentPlayer);
    saveGameData.push_back(_rows);
    saveGameData.push_back(_cols);
    for (int i = 0; i < _rows; ++i)
        for (int j = 0; j < _cols; ++j)
        {
            saveGameData.push_back((int)_gameTable[i][j]);
        }

    return _dataAccess.saveGame(gameIndex, saveGameData, _cols*_rows); // az adatelérés végzi a tevékenységeket
}

QVector<QString> TicTacToeModel::saveGameList() const
{
    return _dataAccess.saveGameList(); // az adatelérés végzi a tevékenységeket
}

TicTacToeModel::Player TicTacToeModel::getField(int x, int y) const
{
    if (x < 0 || x > (_rows-1) || y < 0 || y > (_cols-1)) // ellenőrizzük a tartomány
        return NoPlayer; // már a felsorolt értéket adjuk vissza

    return _gameTable[x][y];
}


void TicTacToeModel::checkGame()
{
    Player won = NoPlayer;

    for(int i = 0; i < _rows; ++i) // ellenőrzések végrehajtása
    {
        for (int j = 0; j < (_cols - 3); j++){
            if (_gameTable[i][j] != 0 && _gameTable[i][j] == _gameTable[i][j+1] && _gameTable[i][j+1] == _gameTable[i][j+2] && _gameTable[i][j+2] == _gameTable[i][j+3] )
                won = _gameTable[i][j];
        }
    }
    for(int i = 0; i < _cols; ++i)
    {
        for (int j = 0; j < (_rows - 3) ; j++)
        {
            if (_gameTable[j][i] != 0 && _gameTable[j][i] == _gameTable[j+1][i] && _gameTable[j+1][i] == _gameTable[j+2][i] && _gameTable[j+2][i] == _gameTable[j+3][i])
                won = _gameTable[j][i];
        }
    }
    for (int i = 0;  i < (_rows - 3); i++){
        for (int j = 0; j < (_cols-3) ; j++)
        {
            if (_gameTable[i][j] != 0 && _gameTable[i][j] == _gameTable[i+1][j+1] && _gameTable[i+1][j+1] == _gameTable[i+2][j+2] && _gameTable[i+2][j+2] == _gameTable[i+3][j+3])
                won = _gameTable[i][j];
        }
    }
    for (int i = 0; i < (_rows - 3) ; i++)
    {
        for (int j = _cols; j > 2; j--)
        {
            if (_gameTable[i][j] != 0 && _gameTable[i][j] == _gameTable[i+1][j-1] && _gameTable[i+1][j-1] == _gameTable[i+2][j-2] && _gameTable[i+2][j-2] == _gameTable[i+3][j-3])
                won = _gameTable[i][j];
        }
    }
    if (won != NoPlayer) // ha valaki győzött
    {
        gameWon(won); // esemény kiváltása
    }
    else if (_stepNumber == _cols * _rows) // döntetlen játék
    {
        gameOver(); // esemény kiváltása
    }
}

void TicTacToeModel::setCols(int a)
{
    if (a == 5 || a == 6 || a == 7)
        _cols = a;
}

void TicTacToeModel::setRows(int a)
{
    if (a == 8 || a == 10 || a == 12)
        _rows = a;
}
