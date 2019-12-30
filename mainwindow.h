#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include "validation.h"
#include "tile.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  QSqlDatabase history;

private slots:

  //QString username;

  void on_pushButton_clicked();

  void chessBoard(QWidget *baseWidget, Tile *tile[8][8]);

  void accessories(QWidget *baseWidget);

  void on_label_linkActivated(const QString &link);

  void on_pushButton_2_clicked();

  void on_pushButton_3_clicked();

  void on_pushButton_4_clicked();

  void on_pushButton_5_clicked();

  void on_pushButton_6_clicked();

  void on_pushButton_7_clicked();

  void on_pushButton_8_clicked();

  void on_lineEdit_returnPressed();

  void on_tableView_activated(const QModelIndex &index);
  
  void on_label_3_linkActivated(const QString &link);

private:
  Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
