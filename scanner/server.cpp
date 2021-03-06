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

   connect(clientConnection, SIGNAL(disconnected()),this, SLOT(uiDisonnected()));
   connect(clientConnection, SIGNAL(readyRead()),this, SLOT(uiData()));
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

   if (in.atEnd())
     {
       qDebug()<<"no data";
       return;
     }

   emit uicommand(in);
 }

  void Server::socketError()
 {
  qDebug()<<"socket error";
 }

 void Server::sendToUiString(QString str)
 {
   //qDebug()<<"SRV Tx: "<<str;
     QByteArray block;
     QDataStream out(&block, QIODevice::WriteOnly);
     out.setVersion(QDataStream::Qt_4_0);
     out << QString("info");
     out << str;

     clientConnection->write(block);
     clientConnection->flush();
 }

 void Server::sendToUiProgress(QString file, int perc)
 {
   QByteArray block;
   QDataStream out(&block, QIODevice::WriteOnly);
   out.setVersion(QDataStream::Qt_4_0);
   out << QString("progress");
   out << file;
   out << perc;

   clientConnection->write(block);
   clientConnection->flush();
 }

 void Server::sendScanInfo(int numfiles)
 {
   QByteArray block;
   QDataStream out(&block, QIODevice::WriteOnly);
   out.setVersion(QDataStream::Qt_4_0);
   out << QString("startscaninfo");
   out << numfiles;

   clientConnection->write(block);
   clientConnection->flush();
 }

 void Server::sendInfected(QString file, QString sig)
 {
   QByteArray block;
   QDataStream out(&block, QIODevice::WriteOnly);
   out.setVersion(QDataStream::Qt_4_0);
   out << QString("infected");
   out << file;
   out << sig;

   clientConnection->write(block);
   clientConnection->flush();
 }
