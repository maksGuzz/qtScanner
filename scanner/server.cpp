 #include <QtNetwork>

 #include <stdlib.h>

 #include "server.h"
 #include <qlocalserver.h>
 #include <qlocalsocket.h>
#include <QDebug>

 Server::Server(QObject *parent)
     : QObject(parent)
     , clientConnection(NULL)
 {
     server = new QLocalServer(this);
     QString listenSockName("scanner");
     QLocalServer::removeServer(listenSockName);
     if (!server->listen(listenSockName)) {
         qDebug()<<"Scanner: Unable to start the server: "<<server->errorString();
         return;
     }

     qDebug()<<tr("The server is running");

     connect(server, SIGNAL(newConnection()), this, SLOT(uiConnected()));
 }

 void Server::uiConnected()
 {
   qDebug()<<"UI connected";
   clientConnection = server->nextPendingConnection();
   server->close();
    qDebug()<<"serv closed";
   connect(clientConnection, SIGNAL(disconnected()),
           this, SLOT(uiDisonnected()));
   connect(clientConnection, SIGNAL(readyRead()),
           this, SLOT(uiData()));
 }

 void Server::uiDisonnected()
 {
   qDebug()<<"UI disconnected";
    clientConnection->deleteLater();
    clientConnection = NULL;
qDebug()<<"serv started";
    QString listenSockName("scanner");
    QLocalServer::removeServer(listenSockName);
    if (!server->listen(listenSockName)) {
        qDebug()<<tr("Scanner Server")<<tr("Unable to start the server: ")<<server->errorString();
        return;
    }
 }

 void Server::uiData()
 {
   qDebug()<<"ui Data";
   QDataStream in(clientConnection);
   in.setVersion(QDataStream::Qt_4_0);

   /*if (blockSize == 0) {
       if (clientConnection->bytesAvailable() < (int)sizeof(quint16))
         {
           qDebug()<<"wrong block size";
           return;
         }
       in >> blockSize;
   }*/

   if (in.atEnd())
     {
       qDebug()<<"no data";
       return;
     }

   QString command;
   in >> command;

  qDebug()<<"UI Rx: "<< command;
  emit sendToUi(command);
 }

 void Server::socketError()
 {
qDebug()<<"socket error";
 }

 void Server::sendToUi(QString str)
 {
   qDebug()<<"SRV Tx: "<<str;
     QByteArray block;
     QDataStream out(&block, QIODevice::WriteOnly);
     out.setVersion(QDataStream::Qt_4_0);
     //out << (quint16)0;
     //out << fortunes.at(qrand() % fortunes.size());
     out << str;
     //out.device()->seek(0);
     //out << (quint16)(block.size() - sizeof(quint16));

     /*QLocalSocket *clientConnection = server->nextPendingConnection();
     connect(clientConnection, SIGNAL(disconnected()),
             clientConnection, SLOT(deleteLater()));*/

     clientConnection->write(block);
     clientConnection->flush();
     //clientConnection->disconnectFromServer();
 }
