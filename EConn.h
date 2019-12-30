#ifndef ECONN_H
#define ECONN_H

#include "mainwindow.h"
#include "validation.h"
#include "tile.h"


void *get_in_addr(struct sockaddr *sa);

int conn();

void player(int sd, int player_nr);

int move_signal(int sd);

void finish_move_signal(int sd);

void sendname(int sd, QString username);

void getname(int sd, char opponentName[100]); //is supposed to get the name of the opponent and save it for the database etc.

void signal_zero(int sd);

struct game_data
{
  char opponentName;
  char date;
  char result;
};

#endif // ECONN_H
