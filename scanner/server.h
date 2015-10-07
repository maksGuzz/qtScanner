#ifndef SERVER_H
#define SERVER_H

#include <QDialog>


 #include <QLabel>
 #include <QPushButton>
 #include <QLocalServer>

 class Server : public QObject
 {
     Q_OBJECT

 public:
     Server(QObject *parent = 0);

 private slots:
     void uiConnected();
     void uiDisonnected();
     void socketError();

     void sendToUi(QString str);

     void uiData();
 private:

     QLocalServer *server;
     QLocalSocket *clientConnection;
     quint16 blockSize;
 };


#endif // SERVER_H
