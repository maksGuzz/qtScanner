#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
 #include <qlocalsocket.h>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void parserReady();
  void parserDown();
  void connectServer();
  void readData();
  void displayError(QLocalSocket::LocalSocketError socketError);
  void sendCommand(QByteArray cmd);
  void on_pushButton_6_clicked();

  void on_pushButton_7_clicked();

  void on_pushButton_3_clicked();

  void on_pushButton_2_clicked();

  void on_pushButton_5_clicked();

private:
  void parseResponse(QDataStream &in);
  Ui::MainWindow *ui;
  QLocalSocket *socket;
};

#endif // MAINWINDOW_H
