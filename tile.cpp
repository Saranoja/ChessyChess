#include "tile.h"
#include "validation.h"
#include "mainwindow.h"
#include "EConn.h"

#include <QTextEdit>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>

validation *valid = new validation();

extern QWidget *myWidget;
QWidget *layer;

int x1,x2,y1,y2,code;
extern int turn;
extern int player_nr;
extern int sd;

int line, col, line_to, col_to;

int opponentCode=0;

int counter=0;

extern int count;
extern Tile *click1;
Tile *tester[8][8];

Tile *change;
Tile *replace;

Tile *op_to, *op_from;

void validate(Tile *temp,int c);
void disOrange();

void Tile::mousePressEvent(QMouseEvent *event)
{
  validate(this,++count);
}

void Tile::display(char elem)
{
  this->pieceName=elem;
  this->setAlignment(Qt::AlignCenter | Qt::AlignCenter);

  if(this->pieceColor && this->piece)
    {
      switch(elem)
        {
        case 'P': this->setPixmap(QPixmap(":/Images/pawn_white.svg"));
          break;
        case 'R': this->setPixmap(QPixmap(":/Images/rook_white.svg"));
          break;
        case 'H': this->setPixmap(QPixmap(":/Images/knight_white.svg"));
          break;
        case 'K': this->setPixmap(QPixmap(":/Images/king_white.svg"));
          break;
        case 'Q': this->setPixmap(QPixmap(":/Images/queen_white.svg"));
          break;
        case 'B': this->setPixmap(QPixmap(":/Images/bishop_white.svg"));
          break;
        }
    }

  else if(this->piece)
    {
      switch(elem)
        {
        case 'P': this->setPixmap(QPixmap(":/Images/pawn_black.svg"));
          break;
        case 'R': this->setPixmap(QPixmap(":/Images/rook_black.svg"));
          break;
        case 'H': this->setPixmap(QPixmap(":/Images/knight_black.svg"));
          break;
        case 'K': this->setPixmap(QPixmap(":/Images/king_black.svg"));
          break;
        case 'Q': this->setPixmap(QPixmap(":/Images/queen_black.svg"));
          break;
        case 'B': this->setPixmap(QPixmap(":/Images/bishop_black.svg"));
          break;
        }
    }
  else
    this->clear();
}

void validate(Tile *temp, int c)
{
  int retValue,i;

  if(player_nr==0)
    turn=1;
  else
    turn=0;

  if(player_nr==0) //if i begin
    {

      if(c==1) //if it's the first click
        {
          if(temp->piece && (temp->pieceColor==turn)) //i click on a piece and it's my turn
            {
              //exp[max++]=temp->tileNum;
              retValue=valid->chooser(temp);

              if(retValue) //good click
                {
                  click1= new Tile();
                  temp->setStyleSheet("QLabel {background-color: green;}");
                  click1=temp;
                }
              else //bad click
                {
                  //temp->setStyleSheet("QLabel {background-color: red;}");
                  count=0;
                }
            }
          else
            {
              //qDebug()<<"Rascel, clicking anywhere";
              count=0;
            }
        }

      else //second click
        {

          if(temp->tileNum==click1->tileNum) //click on the same tile again
            {
              click1->tileDisplay();
              disOrange();
              max=0;
              count=0;
            }

          for(i=0;i<max;i++)
            {
              if(temp->tileNum==exp[i]) //exchange the two tiles
                {
                  click1->piece=0;
                  temp->piece=1;

                  temp->pieceColor=click1->pieceColor;
                  temp->pieceName=click1->pieceName;

                  click1->display(click1->pieceName);
                  temp->display(click1->pieceName);

                  click1->tileDisplay();
                  temp->tileDisplay();

                  y1=click1->row;
                  x1=click1->col;
                  y2=temp->row;
                  x2=temp->col;

                  code=y1*1000+x1*100+y2*10+x2; //this is what i have to send to the server

                  send_move(sd,&code);

                  retValue=valid->check(click1);

                  disOrange();

                  max=0;

                  count=0;

                }

              else
                count=1;
            }


          get_move(sd,&opponentCode);

          line=(opponentCode/1000)%10;
          col=(opponentCode/100)%10;
          line_to=(opponentCode/10)%10;
          col_to=opponentCode%10;

          char piece;
          int colour;

          if(opponentCode!=5) {

              replace = new Tile();
              replace=tile[line_to][col_to];
              //change->tileColor=1;
              replace->piece=1;
              //change->pieceName='K';
              piece=tile[line][col]->pieceName;
              colour=tile[line][col]->pieceColor;
              replace->pieceColor=colour;
              replace->tileNum=25;
              replace->display(piece);
              replace->tileDisplay();


              change = new Tile();
              change=tile[line][col];
              change->piece=0;
              change->display('Q'); //irrelevant
              change->tileDisplay();
            }

        }
    }

  else //if im the 2nd player
    {

      get_move(sd,&opponentCode);
      line=(opponentCode/1000)%10;
      col=(opponentCode/100)%10;
      line_to=(opponentCode/10)%10;
      col_to=opponentCode%10;

      char piece;
      int colour;

      if(opponentCode!=5) {

          replace = new Tile();
          replace=tile[line_to][col_to];
          replace->piece=1;
          piece=tile[line][col]->pieceName;
          colour=tile[line][col]->pieceColor;
          replace->pieceColor=colour;
          replace->tileNum=25; //irrelevant
          replace->display(piece);
          replace->tileDisplay();


          change = new Tile();
          change=tile[line][col];
          change->piece=0;
          change->display('Q');
          change->tileDisplay();
        }


      if(c==1) //if it's the first click
        {
          if(temp->piece && (temp->pieceColor==turn)) //i click on a piece and it's my turn
            {
              //exp[max++]=temp->tileNum;
              retValue=valid->chooser(temp);

              if(retValue) //good click
                {
                  click1= new Tile();
                  temp->setStyleSheet("QLabel {background-color: green;}");
                  click1=temp;
                }
              else //bad click
                {
                  //temp->setStyleSheet("QLabel {background-color: red;}");
                  count=0;
                }
            }
          else
            {
              //qDebug()<<"Rascel, clicking anywhere";
              count=0;
            }
        }

      else //second click
        {

          if(temp->tileNum==click1->tileNum) //click on the same tile again
            {
              click1->tileDisplay();
              disOrange();
              max=0;
              count=0;
            }

          for(i=0;i<max;i++)
            {
              if(temp->tileNum==exp[i]) //exchange the two tiles
                {
                  click1->piece=0;
                  temp->piece=1;

                  temp->pieceColor=click1->pieceColor;
                  temp->pieceName=click1->pieceName;

                  click1->display(click1->pieceName);
                  temp->display(click1->pieceName);

                  click1->tileDisplay();
                  temp->tileDisplay();

                  y1=click1->row;
                  x1=click1->col;
                  y2=temp->row;
                  x2=temp->col;

                  code=y1*1000+x1*100+y2*10+x2; //this is what i have to send to the server

                  send_move(sd,&code);

                  retValue=valid->check(click1);

                  disOrange();

                  max=0;

                  count=0;
                }

              else
                count=1;
            }
        }
      //if(code)
      //send_move(sd,&code);

      //get_move(sd,&opponentCode);

      //get_move(sd,&opponentCode);


    }
}

void Tile::tileDisplay()
{

  if(this->tileColor)
    this->setStyleSheet("QLabel {background-color: rgb(120, 120, 90);}:hover{background-color: rgb(170,85,127);}");
  else
    this->setStyleSheet("QLabel {background-color: rgb(211, 211, 158);}:hover{background-color: rgb(170,95,127);}");
}

void disOrange()
{
  int i;

  for(i=0;i<max;i++)
    tile[exp[i]/8][exp[i]%8]->tileDisplay();

}
