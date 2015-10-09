#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_pushButton_6_clicked()// signatures
{
  QString signFile = QFileDialog::getOpenFileName(this, tr("Open Signatures file"), QDir::currentPath());
  ui->signturesLineEdit->setText(signFile);
  qDebug()<<signFile;
}

void MainWindow::on_pushButton_7_clicked() // scan dir
{
  QString directoryPath = QFileDialog::getExistingDirectory(this, tr("Select directory to scan"), QDir::currentPath());
  ui->pathToScanEdit->setText(directoryPath);
  qDebug()<<directoryPath;
}
