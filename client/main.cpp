#include <QApplication>
#include "mainwindow.h"

 #include "client.h"

 int main(int argc, char *argv[])
 {
     QApplication app(argc, argv);
     //Client client;
     //client.show();
     MainWindow w;
     w.show();
     return app.exec();
 }
