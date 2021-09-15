#include "tictactoewidget.h"
#include <QMessageBox>
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>

TicTacToeWidget::TicTacToeWidget(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(850, 500);
    setBaseSize(850,500);
    setWindowTitle(tr("Tic-Tac-Toe"));

    _loadGameWidget = NULL;
    _saveGameWidget = NULL;
    _newGameWidget = new newGameWidget();

    // mezők grafikája:
    /*_tableGraphics.append(QLineF(0, 66, 200, 66));
    _tableGraphics.append(QLineF(0, 132, 200, 132));
    _tableGraphics.append(QLineF(66, 0, 66, 200));
    _tableGraphics.append(QLineF(132, 0, 132, 200));*/

    // játékosok grafikája:
    _playerXGraphics.append(QLineF(5, 5, 20, 35));
    _playerXGraphics.append(QLineF(5, 35, 20, 5));  /// 8
    _playerOGraphics = QRectF(4.0, 4.0, 18.0, 30.0);/// 5

    // modell eseményeinek feldolgozása
    connect(&_model, SIGNAL(gameWon(TicTacToeModel::Player)), this, SLOT(model_gameWon(TicTacToeModel::Player)));
    connect(&_model, SIGNAL(gameOver()), this, SLOT(model_gameOver()));
    connect(&_model, SIGNAL(fieldChanged(int, int, TicTacToeModel::Player)), this, SLOT(model_fieldChanged(int, int, TicTacToeModel::Player)));
    connect(_newGameWidget->Button_8x5, SIGNAL(clicked()), this, SLOT(newGame()));
    connect(_newGameWidget->Button_10x6, SIGNAL(clicked()), this, SLOT(newGame()));
    connect(_newGameWidget->Button_12x7, SIGNAL(clicked()), this, SLOT(newGame()));

    update();
    _newGameWidget->show();
    isActive = true;
    /*_model.setCols(last);
    _model.setRows(first);
    _model.newGame();

    _rows = _model.getRows();
    _cols = _model.getCols();
    int tmp = 200/_cols;
    for(int i = 1; i < _cols; i++)
    {
        _tableGraphics.append(QLineF(0 , (i*tmp), 200, (i*tmp)));
    }
    tmp = 200/_rows;
    for(int i = 1; i < _rows; i++)
    {
        _tableGraphics.append(QLineF((i*tmp), 0, (i*tmp), 200));
    }
    _playerXGraphics.append(QLineF(5, 5, 20, 35));
    _playerXGraphics.append(QLineF(5, 35, 20, 5));  /// 8
    _playerOGraphics = QRectF(4.0, 4.0, 18.0, 30.0);/// 5*/
}

TicTacToeWidget::~TicTacToeWidget()
{
    if (_loadGameWidget != NULL) delete _loadGameWidget;
    if (_saveGameWidget != NULL) delete _saveGameWidget;
}

void TicTacToeWidget::paintEvent(QPaintEvent *)
{
    if (!isActive){
        QPainter painter(this); // rajzoló objektum
        painter.setRenderHint(QPainter::Antialiasing); // élsimítás használata
        painter.scale(width() / 200.0, height() / 200.0); // skálázás

        painter.setPen(QPen(Qt::black, 2)); // toll beállítása
        painter.setBrush(Qt::red); // ecset beállítása
        painter.drawLines(_tableGraphics); // tábla kirajzolása

        for(int i = 0; i < _rows; i++)
        {
            for(int j = 0; j < _cols; j++)
            {
                painter.save(); // elmentjük a rajztulajdonságokat
                painter.translate(i * 200.0 / _rows, j * 200.0 / _cols); // elmozdítjuk a rajzpontot a megfelelő mezőre

                // mező kirajzolása a játékos függvényében
                switch (_model.getField(i, j))
                {
                case TicTacToeModel::PlayerX:
                    painter.setPen(QPen(Qt::blue, 2)); // toll beállítása
                    painter.drawLines(_playerXGraphics); // megfelelõ grafika kirajzolása
                    break;
                case TicTacToeModel::PlayerO:
                    painter.setPen(QPen(Qt::black, 2)); // toll beállítása
                    painter.drawEllipse(_playerOGraphics);
                    break;
                }
                painter.restore(); // visszatöltjük a korábbi állapotot
            }
        }
    }
}

void TicTacToeWidget::keyPressEvent(QKeyEvent *event)
{       
    if (event->key() == Qt::Key_N && QApplication::keyboardModifiers() == Qt::ControlModifier)
    {
        // lekezeljük a Ctrl+N kombinációt
        _newGameWidget->show();
         isActive = true;
        _model.newGame();
        update();
    }

    // játék betöltése
    if (event->key() == Qt::Key_L && QApplication::keyboardModifiers() == Qt::ControlModifier)
    {


        if (_loadGameWidget == NULL) // ha még egyszer sem nyitották meg az ablakot
        {
            generateTable();
            _loadGameWidget = new LoadGameWidget();
            connect(_loadGameWidget, SIGNAL(accepted()), this, SLOT(loadGame()));
        }

        // beállítjuk a listát, és megnyitjuk az ablakot
        _loadGameWidget->setGameList(_model.saveGameList());
        _loadGameWidget->open();
    }

    // játék mentése
    if (event->key() == Qt::Key_S && QApplication::keyboardModifiers() == Qt::ControlModifier)
    {
        if (_saveGameWidget == NULL) // ha még egyszer sem nyitották meg az ablakot
        {
            _saveGameWidget = new SaveGameWidget();
            connect(_saveGameWidget, SIGNAL(accepted()), this, SLOT(saveGame()));
        }

        // beállítjuk a listát, és megnyitjuk az ablakot
        _saveGameWidget->setGameList(_model.saveGameList());
        _saveGameWidget->open();
    }
}

void TicTacToeWidget::mousePressEvent(QMouseEvent *event)
{
    if (!isActive){
        // az event->pos() megadja az egérpozíciót, ami QPoint típusú, ebbõl kiszámolható, melyik mezőn vagyunk:
        int x = event->pos().x() * _rows / width();
        int y = event->pos().y() * _cols / height();

        _model.stepGame(x, y); // játék léptetése
    }
}

void TicTacToeWidget::model_gameWon(TicTacToeModel::Player player)
{
    // az eredmény függvényében jelenítjük meg a győztest
    switch (player)
    {
    case TicTacToeModel::PlayerX:
        QMessageBox::information(this, trUtf8("Tic-Tac-Toe"), trUtf8("Játék vége! Az X nyerte a játékot!"));
        _newGameWidget->show();
        isActive = true;
        _model.newGame();
        update();
        break;
    case TicTacToeModel::PlayerO:
        QMessageBox::information(this, trUtf8("Tic-Tac-Toe"), trUtf8("Játék vége! A O nyerte a játékot!"));
        _newGameWidget->show();
        isActive = true;
        _model.newGame();
        update();
        break;
    }
}

void TicTacToeWidget::model_gameOver()
{
    QMessageBox::information(this, trUtf8("Tic-Tac-Toe"), trUtf8("Játék vége! A játék döntetlen lett!"));
    _newGameWidget->show();
    isActive = true;
    _model.newGame();
    update();
}

void TicTacToeWidget::model_fieldChanged(int, int, TicTacToeModel::Player)
{
    if (!isActive){
    update();
    }
}

void TicTacToeWidget::loadGame()
{
    // betöltjük a kiválasztott játékot
    if (_model.loadGame(_loadGameWidget->selectedGame()))
    {
        generateTable();
        update();
        QMessageBox::information(this, trUtf8("JTic-Tac-Toe"), trUtf8("Játék betöltve, következik: ") + ((_model.currentPlayer() == TicTacToeModel::PlayerX) ? "X" : "O") + "!");
    }
    else
    {
        QMessageBox::warning(this, trUtf8("Tic-Tac-Toe"), trUtf8("A játék betöltése sikertelen!"));
    }
}

void TicTacToeWidget::saveGame()
{
    // elmentjük a kiválasztott játékot

    if (_model.saveGame(_saveGameWidget->selectedGame()))
    {
        update();
        QMessageBox::information(this, trUtf8("Tic-Tac-Toe"), trUtf8("Játék sikeresen mentve!"));
    }
    else
    {
        QMessageBox::warning(this, trUtf8("Tic-Tac-Toe"), trUtf8("A játék mentése sikertelen!"));
    }
}

void TicTacToeWidget::newGame()
{
    QPushButton* senderButton = qobject_cast<QPushButton*>(sender());

    if (senderButton->text() == "10x6")
    {
        first = 10;
        last = 6;
    }
    else if (senderButton->text() == "12x7")
    {
        first = 12;
        last = 7;
    }
    else if (senderButton->text() == "8x5")
    {
        first = 8;
        last = 5;
    }
    _newGameWidget->close();
    _model.setCols(last);
    _model.setRows(first);
    _model.newGame();
    generateTable();

    isActive = false;
}

void TicTacToeWidget::generateTable()
{
    _playerXGraphics.clear();
    _tableGraphics.clear();
    _rows = _model.getRows();
    _cols = _model.getCols();
    int tmp = 200/_cols;
    for(int i = 1; i < _cols; i++)
    {
        _tableGraphics.append(QLineF(0 , (i*tmp), 200, (i*tmp)));
    }
    tmp = 200/_rows;
    for(int i = 1; i < _rows; i++)
    {
        _tableGraphics.append(QLineF((i*tmp), 0, (i*tmp), 200));
    }
    if (_cols == 5 && _rows == 8){
        _playerXGraphics.append(QLineF(5, 5, 20, 35));
        _playerXGraphics.append(QLineF(5, 35, 20, 5));  /// 8
        _playerOGraphics = QRectF(4.0, 4.0, 18.0, 30.0);/// 5
    }
    else if (_cols == 6 && _rows == 10)
    {
        _playerXGraphics.append(QLineF(5, 5, 16, 29));
        _playerXGraphics.append(QLineF(5, 29, 16, 5));  /// 8
        _playerOGraphics = QRectF(3.0, 3.0, 14.0, 25.0);/// 5
    }
    else if (_cols == 7 && _rows == 12)
    {
        _playerXGraphics.append(QLineF(2, 2, 8, 20));
        _playerXGraphics.append(QLineF(2, 20, 8, 2));  /// 8
        _playerOGraphics = QRectF(0.0, 1.0, 9.0, 22.0);/// 5
    }
}
