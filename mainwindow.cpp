#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include "ui_dialog.h"
#include "tile.h"
#include "validation.h"
#include "EConn.h"
#include "db_conn.h"

#include <string.h>
#include <QLayout>
#include <QWidget>
#include <QTextEdit>
#include <QString>
#include <QTableView>
#include <QSqlDatabase>
#include <QSqlQuery>

int count=0,exp[60],max=0, turn=1; //white begins
int wR,wC,bR,bC;
Tile *click1;
int sd;

int player_nr;

char opponentName[100];

extern QWidget *myWidget;

QString username;

Tile *tile[8][8] = { { NULL } };

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  QLabel *myLabel = new QLabel();
  this->layout()->addWidget(myLabel);
  QWidget *myWidget = new QWidget();

  QSqlDatabase history=QSqlDatabase::addDatabase("QSQLITE");
  history.setDatabaseName("/home/irina/Desktop/RC_Project/history.db");

  if(!history.open())
    {
      perror("Failed opening the database");
    }

}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::chessBoard(QWidget *baseWidget, Tile *tile[8][8])
{
  int i,j,k=0,hor,ver;
  // Background *Background[4]={ NULL };

  //Create 64 tiles (allocating memory to the objects of Tile class)
  ver=70;
  for(i=0;i<8;i++)
    {
      hor=215;
      for(j=0;j<8;j++)
        {
          tile[i][j] = new Tile(baseWidget);
          tile[i][j]->tileColor=(i+j)%2;
          tile[i][j]->piece=0;
          tile[i][j]->row=i; //this is what i have to send to the server
          tile[i][j]->col=j;
          tile[i][j]->tileNum=k++;
          tile[i][j]->tileDisplay();
          tile[i][j]->setGeometry(hor,ver,102,102);
          hor+=102;
        }
      ver+=102;
    }

  //white pawns
  for(j=0;j<8;j++)
    {
      tile[1][j]->piece=1;
      tile[1][j]->pieceColor=0;
      tile[1][j]->display('P');
    }

  //black pawns
  for(j=0;j<8;j++)
    {
      tile[6][j]->piece=1;
      tile[6][j]->pieceColor=1;
      tile[6][j]->display('P');
    }

  //white and black remaining elements
  for(j=0;j<8;j++)
    {
      tile[0][j]->piece=1;
      tile[0][j]->pieceColor=0;
      tile[7][j]->piece=1;
      tile[7][j]->pieceColor=1;
    }

  {
    tile[0][0]->display('R');
    tile[0][1]->display('H');
    tile[0][2]->display('B');
    tile[0][3]->display('Q');
    tile[0][4]->display('K');
    tile[0][5]->display('B');
    tile[0][6]->display('H');
    tile[0][7]->display('R');
  }


  {
    tile[7][0]->display('R');
    tile[7][1]->display('H');
    tile[7][2]->display('B');
    tile[7][3]->display('Q');
    tile[7][4]->display('K');
    tile[7][5]->display('B');
    tile[7][6]->display('H');
    tile[7][7]->display('R');
  }

  wR=7;
  wC=4;

  bR=0;
  bC=4;

}

void MainWindow::accessories(QWidget *baseWidget)
{
  QLabel *player2 = new QLabel(baseWidget);

  if(player_nr==0){
      QLabel *name2 = new QLabel(opponentName, baseWidget); //Player 2 -> username or
      QLabel *time2 = new QLabel("00:00:00", baseWidget);

      QLabel *player1 = new QLabel(baseWidget);
      QLabel *name1 = new QLabel(username, baseWidget); //Player 1 -> username depending on which player i am
      QLabel *time1 = new QLabel("00:00:00", baseWidget);

      QLabel *moves = new QLabel(baseWidget);

      name1->setGeometry(1425,730,80,20);
      name1->setStyleSheet("QLabel {color: white;}");
      time1->setGeometry(1420,750,80,20);
      time1->setStyleSheet("QLabel {color: white;}");
      player1->setGeometry(1400,630,100,100);
      player1->setPixmap(QPixmap(":/Images/profile1.jpg"));

      name2->setGeometry(1425,330,80,20);
      name2->setStyleSheet("QLabel {color: white;}");
      time2->setGeometry(1420,350,80,20);
      time2->setStyleSheet("QLabel {color: white;}");
      player2->setGeometry(1400,230,100,100);
      player2->setPixmap(QPixmap(":/Images/profile2.jpg"));

      moves->setGeometry(1100,200,250,550);
      moves->setPixmap(QPixmap(":/Images/pannel.jpg"));
    }

  else if(player_nr==1)
    {
      QLabel *name2 = new QLabel(username, baseWidget); //Player 2 -> username or
      QLabel *time2 = new QLabel("00:00:00", baseWidget);

      QLabel *player1 = new QLabel(baseWidget);
      QLabel *name1 = new QLabel(opponentName, baseWidget); //Player 1 -> username depending on which player i am
      QLabel *time1 = new QLabel("00:00:00", baseWidget);

      QLabel *moves = new QLabel(baseWidget);

      name1->setGeometry(1425,730,80,20);
      name1->setStyleSheet("QLabel {color: white;}");
      time1->setGeometry(1420,750,80,20);
      time1->setStyleSheet("QLabel {color: white;}");
      player1->setGeometry(1400,630,100,100);
      player1->setPixmap(QPixmap(":/Images/profile1.jpg"));

      name2->setGeometry(1425,330,80,20);
      name2->setStyleSheet("QLabel {color: white;}");
      time2->setGeometry(1420,350,80,20);
      time2->setStyleSheet("QLabel {color: white;}");
      player2->setGeometry(1400,230,100,100);
      player2->setPixmap(QPixmap(":/Images/profile2.jpg"));

      moves->setGeometry(1100,200,250,550);
      moves->setPixmap(QPixmap(":/Images/pannel.jpg"));
    }

}

void MainWindow::on_pushButton_clicked()
{
  ui->pushButton->deleteLater();
  ui->page->deleteLater();
  ui->page_7->show();
  SLOT(clear());
}

void MainWindow::on_pushButton_2_clicked() //on leaderboard page
{
  // ui->pushButton_2->deleteLater();
  ui->page_2->hide();
  ui->page_3->hide();
  ui->page_4->hide();
  ui->page_5->show();
  SLOT(clear());
}

void MainWindow::on_pushButton_3_clicked() //on play page
{
  //  ui->pushButton_3->deleteLater();
  //ui->page_2->hide();
  //ui->page_3->hide();
  //ui->page_4->hide();
  //ui->page_5->show();
  //SLOT(clear());

  /*QWidget *myWidget = new QWidget();
  myWidget->setGeometry(0,0,1700,956);
  QLabel *image= new QLabel(myWidget);
  image->setPixmap(QPixmap(":/Images/waiting.jpg")); //draw waiting screen
  */

  QWidget *myWidget2 = new QWidget();
  myWidget2->setGeometry(0,0,1700,956);
  QLabel *image2= new QLabel(myWidget2);
  image2->setPixmap(QPixmap(":/Images/waiting.jpg")); //draw waiting screen

  sd=conn();

  player_nr=player(sd, player_nr);

  if(player_nr==0) {
      myWidget2->show();
    }

  sendname(sd, username); //here starts the match
  getname(sd,opponentName);

  myWidget2->deleteLater();
  QWidget *myWidget = new QWidget();
  myWidget->setGeometry(0,0,1700,956);
  QLabel *image= new QLabel(myWidget);
  image->setPixmap(QPixmap(":/Images/tournament.jpg")); //draw tournament screen
  accessories(myWidget);
  chessBoard(myWidget,tile);
  myWidget->show();
}

void MainWindow::on_pushButton_4_clicked() //on history page
{
  ui->page_2->hide();
  ui->page_3->hide();
  //ui->page_4->hide();
  ui->page_4->show();
  SLOT(clear());
  //QTableView *view = new QTableView;
  //view->show();

  int count=0;

  QSqlQuery qry;
  qry.exec("select * from history where username ='"  + username + "'; " );

  ui->tableWidget->setRowCount(0);
  ui->tableWidget->setColumnCount(4);
  ui->tableWidget->setColumnWidth(0,200);
  ui->tableWidget->setColumnWidth(1,200);
  ui->tableWidget->setColumnWidth(2,200);
  ui->tableWidget->setColumnWidth(3,200);
  ui->tableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem("Username"));
  ui->tableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem("Date"));
  ui->tableWidget->setHorizontalHeaderItem(2,new QTableWidgetItem("Opponent"));
  ui->tableWidget->setHorizontalHeaderItem(3,new QTableWidgetItem("Result"));

  while (qry.next()) {
      QString name = qry.value(0).toString();
      QString date = qry.value(1).toString();
      QString opponent = qry.value(2).toString();
      QString result = qry.value(3).toString();
      ui->tableWidget->insertRow(count);
      ui->tableWidget->setItem(count,0,new QTableWidgetItem(name));
      ui->tableWidget->setItem(count,1,new QTableWidgetItem(date));
      ui->tableWidget->setItem(count,2,new QTableWidgetItem(opponent));
      ui->tableWidget->setItem(count,3,new QTableWidgetItem(result));
      count++;
    }
}

void MainWindow::on_pushButton_5_clicked() //get back to main menu
{
  ui->page_3->hide();
  ui->page_2->show();
  SLOT(clear());
}

void MainWindow::on_pushButton_6_clicked() //get back to main menu
{
  ui->page_4->hide();
  ui->page_2->show();
  SLOT(clear());
}

void MainWindow::on_pushButton_7_clicked() //get back to main menu
{
  ui->page_5->hide();
  ui->page_2->show();
  SLOT(clear());
}

void MainWindow::on_pushButton_8_clicked()
{
  exit(1);
}

void MainWindow::on_lineEdit_returnPressed()
{
  //ui->page->deleteLater();
  ui->page_7->deleteLater();
  ui->page_2->show();
  SLOT(clear());
  username=ui->lineEdit->text();
  ui->label->setText(username);
}

void MainWindow::on_label_linkActivated(const QString &link)
{
  ui->label->setText(username);
}

void MainWindow::on_label_3_linkActivated(const QString &link)
{

}
