#include <QtGui>
#include <QtNetwork>
#include <QGridLayout>
#include <QMessageBox>
#include <QDebug>


#include "client.h"

Client::Client(QWidget *parent)
    : QDialog(parent)
{
    hostLabel = new QLabel(tr("&Server name:"));
    hostLineEdit = new QLineEdit("start");

    hostLabel->setBuddy(hostLineEdit);

    statusLabel = new QListWidget();//tr("Waiting for connection.\n\n"));

    connectServerButton = new QPushButton(tr("Connect"));
    connectServerButton->setDefault(true);

    sendServerButton = new QPushButton(tr("Send"));

    quitButton = new QPushButton(tr("Quit"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(connectServerButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(sendServerButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    socket = new QLocalSocket(this);

    connect(connectServerButton, SIGNAL(clicked()),
            this, SLOT(connectServer()));
    connect(sendServerButton, SIGNAL(clicked()),
            this, SLOT(sendCommand()));
    connect(sendServerButton, SIGNAL(clicked()),
            statusLabel, SLOT(clear()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(socket, SIGNAL(error(QLocalSocket::LocalSocketError)),
            this, SLOT(displayError(QLocalSocket::LocalSocketError)));

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(hostLabel, 0, 0);
    mainLayout->addWidget(hostLineEdit, 0, 1);
    mainLayout->addWidget(statusLabel, 2, 0, 1, 2);
    mainLayout->addWidget(buttonBox, 3, 0, 1, 2);
    setLayout(mainLayout);

    setWindowTitle(tr("Scaner Client"));
    hostLineEdit->setFocus();
    //QString directory_path = QFileDialog::getExistingDirectory(this, tr("Select your music directory"), QDir::currentPath());
}

void Client::sendCommand()
{
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_4_0);
  out << hostLineEdit->text();

  socket->write(block);
  socket->flush();
}

void Client::connectServer()
{
    connectServerButton->setEnabled(false);
    socket->abort();
    socket->connectToServer("scanner");
}

void Client::readData()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_0);

    if (in.atEnd())
        return;
    while(!in.atEnd())
    {
      QString resp;
      in >> resp;
      qDebug()<<resp;

      statusLabel->addItem(resp);
    }
}

void Client::displayError(QLocalSocket::LocalSocketError socketError)
{
    switch (socketError) {
    case QLocalSocket::ServerNotFoundError:
        QMessageBox::information(this, tr("Scaner Client"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QLocalSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Scaner Client"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the Scaner server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    case QLocalSocket::PeerClosedError:
        break;
    default:
        QMessageBox::information(this, tr("Scaner Client"),
                                 tr("The following error occurred: %1.")
                                 .arg(socket->errorString()));
    }

    connectServerButton->setEnabled(true);
}

