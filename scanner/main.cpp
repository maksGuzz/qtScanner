#include <QApplication>
 #include <QtCore>

 #include <stdlib.h>

 #include "server.h"
#include "commandreader.h"
#include "strategyselector.h"

 int main(int argc, char *argv[])
 {
     QApplication app(argc, argv);
     Server server;
     CommandReader reader;
     StrategySelector strategy;

     QObject::connect(&server, SIGNAL(uicommand(QDataStream&)), &reader, SLOT(readCommand(QDataStream&)));
     QObject::connect(&reader, SIGNAL(parseSignatures(QString)), &strategy, SLOT(parseSignatures(QString)));
     QObject::connect(&reader, SIGNAL(startScan(QString)), &strategy, SLOT(startScan(QString)));
     QObject::connect(&reader, SIGNAL(stopScan()), &strategy, SLOT(stopScan()));
     QObject::connect(&strategy, SIGNAL(sendToUiString(QString)), &server, SLOT(sendToUiString(QString)));
     QObject::connect(&strategy, SIGNAL(progressForUi(QString,int)), &server, SLOT(sendToUiProgress(QString,int)));
     QObject::connect(&strategy, SIGNAL(startScanInfo(int)), &server, SLOT(sendScanInfo(int)));
     QObject::connect(&strategy, SIGNAL(sendToUiInfect(QString, QString)), &server, SLOT(sendInfected(QString, QString)));

     return app.exec();
 }
