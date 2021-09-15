#include "newgamewidget.h"

newGameWidget::newGameWidget()
{
    Button_10x6 = new QPushButton("10x6",this);
    Button_12x7 = new QPushButton("12x7",this);
    Button_8x5 = new QPushButton("8x5",this);
    Vlayout = new QVBoxLayout();
    Glayout = new QGridLayout();
    Glayout->addWidget(Button_10x6,0,0);
    Glayout->addWidget(Button_12x7,0,1);
    Glayout->addWidget(Button_8x5,0,2);
    label = new QLabel("Válasszon méretet:");
    Vlayout->addWidget(label);
    Vlayout->addLayout(Glayout);
    setLayout(Vlayout);

}

/*void newGameWidget::onButtonClicked()
{
    QMessageBox error;
    error.setText("itt jártam");
    error.exec();
    resize();
    QMessageBox error1;
    error1.setText("itt jártam, megint");
    error1.exec();
}*/
