#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
    , a(0)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked(){
    QString size;
    size = ui->size_in->text();
    ui->size_in->setText("");
    a = size.toInt();
    this->close();
}
