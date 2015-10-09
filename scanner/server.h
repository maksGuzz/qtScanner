#ifndef SERVER_H
#define SERVER_H

 #include <QLocalServer>

 class Server : public QObject
 {
     Q_OBJECT

 public:
     Server(QObject *parent = 0);

 signals:
     void uicommand(QDataStream &in);
 private slots:
     void uiConnected();
     void uiDisonnected();
     void socketError();

     void sendToUiString(QString str);
     void sendToUiProgress(QString file, int perc);
     void sendScanInfo(int);

     void uiData();
 private:

     QLocalServer *server;
     QLocalSocket *clientConnection;
     quint16 blockSize;
 };


#endif // SERVER_H
