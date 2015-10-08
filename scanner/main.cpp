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
     QObject::connect(&reader, SIGNAL(startScan()), &strategy, SLOT(startScan()));
     QObject::connect(&reader, SIGNAL(stopScan()), &strategy, SLOT(stopScan()));
     QObject::connect(&strategy, SIGNAL(stringForUi(QString)), &server, SLOT(sendToUi(QString)));

     return app.exec();
 }
