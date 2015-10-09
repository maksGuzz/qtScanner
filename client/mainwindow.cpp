#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  socket = new QLocalSocket(this);
  connectServer();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::connectServer()
{
  socket->abort();
  socket->connectToServer("scanner");
}

void MainWindow::readData()
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

    ui->listWidget->addItem(resp);
  }
}

void MainWindow::sendCommand(QByteArray cmd)
{
  socket->write(cmd);
  socket->flush();
}

void MainWindow::displayError(QLocalSocket::LocalSocketError socketError)
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

}

void MainWindow::on_pushButton_6_clicked()// signatures
{
  QString signFile = QFileDialog::getOpenFileName(this, tr("Open Signatures file"), QDir::currentPath());
  ui->signturesLineEdit->setText(signFile);
  ui->listWidget->addItem(signFile);
  qDebug()<<signFile;
}

void MainWindow::on_pushButton_7_clicked() // scan dir
{
  QString directoryPath = QFileDialog::getExistingDirectory(this, tr("Select directory to scan"), QDir::currentPath());
  ui->pathToScanEdit->setText(directoryPath);
  ui->listWidget->addItem(directoryPath);
  qDebug()<<directoryPath;
}

void MainWindow::on_pushButton_3_clicked()
{
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_4_0);
  out << QString("start");
  sendCommand(block);
}

void MainWindow::on_pushButton_2_clicked()
{
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_4_0);
  out << QString("stop");
  sendCommand(block);
}

void MainWindow::on_pushButton_5_clicked() //parse signatures
{
    if(ui->signturesLineEdit->text().isEmpty())
      {
        ui->listWidget->addItem("Select file with sinatures");
        return;
      }
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << QString("parsesignatures");
    out << ui->signturesLineEdit->text();
    sendCommand(block);
}
