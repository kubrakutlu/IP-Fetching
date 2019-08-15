#include "widget.h"
#include "ui_widget.h"
#include "ip-fetcher.h"
#include <QLabel>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->label->setText(addr);
}

Widget::~Widget()
{
    delete ui;
}





