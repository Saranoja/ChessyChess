#include "mainwindow.h"
#include "validation.h"
#include "tile.h"
#include "EConn.h"

#include <QApplication>
#include <QLabel>
#include <QDebug>
#include <QString>

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

using namespace std;

#define PORT 3490
#define PORT2 3590
#define LOCAL "127.0.0.1"

struct leaderboard{         // structure for receiving database from server
    char id[100];
    int score;
}myLB[100];

int conn()
{
  int sd; // descriptorul de socket
  struct sockaddr_in server; // structura folosita pentru conectare
  // int code;

  /* cream socketul */
  if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror("Eroare la socket().\n");
      //return errno;
    }

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(LOCAL);
  server.sin_port = htons(PORT);

  if (connect(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
      perror("[client]Eroare la connect().\n");
      //return errno;
    }

  return sd;
}

int leaderboard_conn()
{
  int sd; // descriptorul de socket
  struct sockaddr_in server; // structura folosita pentru conectare
  // int code;

  /* cream socketul */
  if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror("Eroare la socket().\n");
      //return errno;
    }

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(LOCAL);
  server.sin_port = htons(PORT2);

  if (connect(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
      perror("[client]Eroare la connect().\n");
      //return errno;
    }

  return sd;
}

int player(int sd, int player_nr)
{
  read(sd, &player_nr, sizeof(player_nr));
  return player_nr;
}

int move_signal(int sd)
{
  int signal;
  read(sd, &signal, sizeof(signal));
  return signal;
}

void finish_move_signal(int sd)
{
  int finish_signal=5;
  write(sd, &finish_signal, sizeof(finish_signal));
}

void sendname(int sd, QString username)
{
  int nameLen=username.size();
  if (write(sd, "0", sizeof(char)) <= 0)
  {
      perror("[client]Eroare la write() spre server.\n");
  }
  if (write(sd, &nameLen, sizeof(int)) <= 0)
    perror("[client]Eroare la write() spre server.\n");
  std::string str=username.toStdString();
  const char* name = str.c_str();
  if (write(sd, name, nameLen) <= 0) {
      perror("[client]Eroare la write() spre server.\n");
    }
}

void getname(int sd, char opponentName[100])
{
  int nameLen;
  bzero(opponentName, sizeof(opponentName));
  if (read(sd, &nameLen, sizeof(int) ) < 0) {
      perror("[client]Eroare la read() de la server.\n");
    }
  if (read(sd, opponentName, nameLen ) < 0) {
      perror("[client]Eroare la read() de la server.\n");
    }
}

void send_move(int sd, int *code)
{
  char c = '1';
  if (write(sd, &c, sizeof(char)) <= 0)
    {
      perror("[client]Eroare la write() spre server.\n");
    }
  if (write(sd, code, sizeof(int)) <= 0) {
      perror("[client]Eroare la write() spre server.\n");
    }
}

void get_move(int sd, int *opponentCode)
{
  if (read(sd, opponentCode, sizeof(int) ) < 0) {
      perror("[client]Eroare la read() de la server.\n");
    }
}

void useless(int sd)
{
  int c;
  if (read(sd, &c, sizeof(int) ) < 0) {
      perror("[client]Eroare la read() de la server.\n");
    }

}

void useless_char(int sd)
{
  char c;
  if (read(sd, &c, sizeof(char) ) < 0) {
      perror("[client]Eroare la read() de la server.\n");
    }
}

void receive_leaderboard(int sd)
{
  int LBSize;
  read(sd, &LBSize, sizeof(int));  // get leaderboard in special struct myLB
  read(sd, &myLB, sizeof(myLB));
}
