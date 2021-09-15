#ifndef NEWGAMEWIDGET_H
#define NEWGAMEWIDGET_H

#include <QObject>
#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QMessageBox>

class newGameWidget : public QDialog
{
    Q_OBJECT
signals:
   void resize();
/*private slots:
   void onButtonClicked();*/

public:
    newGameWidget();
    QPushButton* Button_8x5;
    QPushButton* Button_10x6;
    QPushButton* Button_12x7;
private:

    QVBoxLayout* Vlayout;
    QGridLayout* Glayout;
    QLabel* label;
};

#endif // NEWGAMEWIDGET_H
