#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  socket = new QLocalSocket(this);
  connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
  connect(socket, SIGNAL(error(QLocalSocket::LocalSocketError)),
          this, SLOT(displayError(QLocalSocket::LocalSocketError)));
  connect(socket, SIGNAL(connected()), this, SLOT(parserReady()));
  connect(socket, SIGNAL(disconnected()), this, SLOT(parserDown()));
  ui->pathToScanEdit->setText("/home/mguzeev/Downloads");
  connectServer();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::parserReady()
{
  ui->listWidget->addItem("Parser ready");
}

void MainWindow::parserDown()
{
  ui->listWidget->addItem("Parser Down");
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
  parseResponse(in);
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
  out << ui->pathToScanEdit->text();
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

void MainWindow::parseResponse(QDataStream &in)
{
  while(!in.atEnd())
  {
    QString resp;
    in >> resp;
    //qDebug()<<resp;
    if(resp == "startscaninfo")
      {
        int max;
        in >> max;
        ui->progressBar->setMaximum(max);
        ui->progressBar->setMinimum(0);
        ui->progressBar->setValue(0);
        qDebug()<<resp<<max;
        continue;
      }
    if(resp == "progress")
      {
        int progress;
        QString file;
        in >> file;
        in >> progress;
        QVariant v(progress);
        ui->listWidget->addItem(v.toString()+file);
        ui->progressBar->setValue(progress);
        continue;
      }
    if(resp == "info")
      {
        QString str;
        in >> str;
        //qDebug()<<resp<<str;
        ui->listWidget->addItem(str);
      }
    if(resp == "infected")
      {
        QString file;
        QString virus;
        in >> file;
        in >> virus;
        QListWidgetItem *item = new QListWidgetItem(QString("INFECTED\nFILE: %1\nVIR : %2").arg(file).arg(virus), ui->listWidget);
        item->setBackgroundColor(QColor("red"));
        ui->listWidget->scrollToItem(item);
        continue;
      }
    //qApp->processEvents();
  }
}
