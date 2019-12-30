#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::Dialog)
{
  ui->setupUi(this);
}

Dialog::~Dialog()
{
  delete ui;
}

void Dialog::on_pushButton_clicked()
{
  ui->label->setPixmap(QPixmap(":/Images/tournament.jpg"));
  ui->pushButton->deleteLater();
  SLOT(clear());
}
