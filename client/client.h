#ifndef CLIENT_H
 #define CLIENT_H

 #include <QDialog>
 #include <qlocalsocket.h>

 #include <QDialogButtonBox>
 #include <QLabel>
 #include <QLineEdit>
 #include <QPushButton>
 #include <QLocalSocket>

 class Client : public QDialog
 {
     Q_OBJECT

 public:
     Client(QWidget *parent = 0);

 private slots:
     void connectServer();
     void readData();
     void displayError(QLocalSocket::LocalSocketError socketError);
     void sendCommand();

 private:
     QLabel *hostLabel;
     QLineEdit *hostLineEdit;
     QLabel *statusLabel;
     QPushButton *connectServerButton;
     QPushButton *sendServerButton;
     QPushButton *quitButton;
     QDialogButtonBox *buttonBox;

     QLocalSocket *socket;
 };

 #endif
